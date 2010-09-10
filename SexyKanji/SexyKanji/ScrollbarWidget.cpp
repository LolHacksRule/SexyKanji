// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "ScrollbarWidget.h"
#include "WidgetManager.h"
#include "ScrollListener.h"
#include "ScrollbuttonWidget.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theId
// @param     	theScrollListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ScrollbarWidget::ScrollbarWidget(int theId, ScrollListener* theScrollListener) 
{
	mId = theId;
	mScrollListener = theScrollListener;		
		
	SetDisabled(true);
						
	mUpButton = new ScrollbuttonWidget(0, this);
	mUpButton->SetDisabled(true);
				
	mDownButton = new ScrollbuttonWidget(1, this);		
	mDownButton->SetDisabled(true);
		
	mInvisIfNoScroll = false;
	mPressedOnThumb = false;
	mValue = 0;
	mMaxValue = 0;
	mPageSize = 0;
	mUpdateAcc = 0;
	mButtonAcc = 0;
	mUpdateMode = 0;
	mHorizontal = false;

	AddWidget(mUpButton);
	AddWidget(mDownButton);
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ScrollbarWidget::~ScrollbarWidget()
{
	RemoveAllWidgets(true);
}

/*!***********************************
// @return    	void
// @param     	invisIfNoScroll
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetInvisIfNoScroll(bool invisIfNoScroll)
{
	mInvisIfNoScroll = invisIfNoScroll;

	if (mInvisIfNoScroll) 
	{
		SetVisible(false);
		mDownButton->SetVisible(false);
		mUpButton->SetVisible(false);
	}
}

/*!***********************************
// @return    	void
// @param     	isHorizontal
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetHorizontal(bool isHorizontal)
{
	mHorizontal = isHorizontal;
	mDownButton->mHorizontal = mHorizontal;
	mUpButton->mHorizontal = mHorizontal;
}


/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::ResizeScrollbar(int theX, int theY, int theWidth, int theHeight) 
{
	Resize(theX, theY, theWidth, theHeight);
	if (mHorizontal)
	{
		theX = theY = 0; // Use New Container Stuff
		mUpButton->Resize(theX, theY, theHeight, theHeight);
		mDownButton->Resize(theX + theWidth - theHeight, theY, theHeight, theHeight);
	}
	else
	{
		theX = theY = 0; // Use New Container Stuff
		mUpButton->Resize(theX, theY, theWidth, theWidth);
		mDownButton->Resize(theX, theY + theHeight - theWidth, theWidth, theWidth);
	}
}

/*!***********************************
// @return    	void
// @param     	theNewMaxValue
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetMaxValue(double theNewMaxValue) 
{
	mMaxValue = theNewMaxValue;
	ClampValue();
	MarkDirty();
}
	
/*!***********************************
// @return    	void
// @param     	theNewPageSize
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetPageSize(double theNewPageSize) 
{
	mPageSize = theNewPageSize;
	ClampValue();
	MarkDirty();
}
	
/*!***********************************
// @return    	void
// @param     	theNewValue
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetValue(double theNewValue) 
{
	mValue = theNewValue;		
	ClampValue();
	mScrollListener->ScrollPosition(mId, mValue);
	MarkDirty();
}
	
/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool ScrollbarWidget::AtBottom() 
{
	return mMaxValue - mPageSize - mValue <= 1.0;
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::GoToBottom() 
{
	mValue = mMaxValue - mPageSize;
	ClampValue();
	SetValue(mValue);
}
	
/*!***********************************
// @return    	void
// @param     	g
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::DrawThumb(Graphics *g, int theX, int theY, int theWidth, int theHeight) 
{
	g->SetColor(SexyColor(212, 212, 212));
	g->FillRect(theX, theY, theWidth, theHeight);
		
	g->SetColor(SexyColor(255, 255, 255));
	g->FillRect(theX + 1, theY + 1, theWidth-2, 1);
	g->FillRect(theX + 1, theY + 1, 1, theHeight-2);
			
	g->SetColor(SexyColor::Black);
	g->FillRect(theX, theY + theHeight - 1, theWidth, 1);
	g->FillRect(theX + theWidth - 1, theY, 1, theHeight);						
		
	g->SetColor(SexyColor(132, 132, 132));
	g->FillRect(theX + 1, theY + theHeight - 2, theWidth - 2, 1);
	g->FillRect(theX + theWidth - 2, theY + 1, 1, theHeight - 2);					
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ScrollbarWidget::GetTrackSize() 
{
	if (mHorizontal)
		return mWidth-2*mUpButton->mWidth;
	else
		return mHeight-2*mUpButton->mWidth;
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ScrollbarWidget::GetThumbSize() 
{
	if (mPageSize > mMaxValue)
		return 0;
		
	int aThumbSize = (int) ((double) ((GetTrackSize() * mPageSize / mMaxValue)) + 0.5);
	return K_MAX(8, aThumbSize);
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ScrollbarWidget::GetThumbPosition() 
{
	if (mPageSize > mMaxValue) return mUpButton->mWidth;
	return (int) ((double) (((mValue * (GetTrackSize() - GetThumbSize())) / (mMaxValue - mPageSize))) + 0.5) + mUpButton->mWidth;
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::Draw(Graphics *g) 
{
	int aThumbSize = GetThumbSize();
	int aThumbPosition = GetThumbPosition();
	
	if (mHorizontal)
	{
		if (mUpdateMode == UPDATE_MODE_PGUP) 
			g->SetColor(SexyColor(48, 48, 48));
		else 
			g->SetColor(SexyColor(232, 232, 232));				
		g->FillRect(0, 0, aThumbPosition, mHeight);
			
		if (aThumbSize > 0)	
			DrawThumb(g, aThumbPosition, 0, aThumbSize, mHeight);
			
		if (mUpdateMode == UPDATE_MODE_PGDN) 
			g->SetColor(SexyColor(48, 48, 48));
		else 
			g->SetColor(SexyColor(232, 232, 232));
		g->FillRect(aThumbPosition + aThumbSize, 0, mWidth - aThumbPosition - aThumbSize, mHeight);
	}
	else
	{
		if (mUpdateMode == UPDATE_MODE_PGUP) 
			g->SetColor(SexyColor(48, 48, 48));
		else 
			g->SetColor(SexyColor(232, 232, 232));				
		g->FillRect(0, 0, mWidth, aThumbPosition);
			
		if (aThumbSize > 0)	
			DrawThumb(g, 0, aThumbPosition, mWidth, aThumbSize);
			
		if (mUpdateMode == UPDATE_MODE_PGDN) 
			g->SetColor(SexyColor(48, 48, 48));
		else 
			g->SetColor(SexyColor(232, 232, 232));
		g->FillRect(0, aThumbPosition + aThumbSize, mWidth, mHeight - aThumbPosition - aThumbSize);
	}
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::ClampValue() 
{
	double anOldValue = mValue;
		
	if (mValue > mMaxValue - mPageSize)
		mValue = mMaxValue - mPageSize;
	if (mValue < 0.0)
		mValue = 0.0;
		
	bool canScroll = mPageSize < mMaxValue;
		
	SetDisabled(!canScroll);
	mUpButton->SetDisabled(!canScroll);
	mDownButton->SetDisabled(!canScroll);
				
	if (mInvisIfNoScroll) 
	{
		SetVisible(canScroll);
		mDownButton->SetVisible(canScroll);
		mUpButton->SetVisible(canScroll);
	}
		
	if (mValue != anOldValue) mScrollListener->ScrollPosition(mId, mValue);
}		
	
/*!***********************************
// @return    	void
// @param     	thePosition
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::SetThumbPosition(int thePosition) 
{
	SetValue(((thePosition-mUpButton->mWidth) * (mMaxValue - mPageSize)) / (GetTrackSize() - GetThumbSize()));				
}
	
/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::ButtonPress(int theId) 
{
	mButtonAcc = 0;
		
	if (theId == 0) 
		SetValue(mValue-1);
	else 
		SetValue(mValue+1);
}
	
/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::ButtonDepress(int theId)
{
}
	
/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::ButtonDownTick(int theId)
{
	if (theId == 0) 
	{
		if (++mButtonAcc >= 25)	
		{
			SetValue(mValue-1);
			mButtonAcc = 24;	
		}
	}
	else 
	{
		if (++mButtonAcc >= 25)	
		{
			SetValue(mValue+1);
			mButtonAcc = 24;
		}
	}
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::Update()
{
	Widget::Update();

	switch (mUpdateMode)
	{
	case UPDATE_MODE_PGUP:
		if (ThumbCompare(mLastMouseX, mLastMouseY) != -1) 
		{
			mUpdateMode = UPDATE_MODE_IDLE;
			MarkDirty();
		}
		else if (++mUpdateAcc >= 25)
		{
			SetValue(mValue - mPageSize);
			mUpdateAcc = 20;
		}
		break;
	case UPDATE_MODE_PGDN:
		if (ThumbCompare(mLastMouseX, mLastMouseY) != 1) 
		{
			mUpdateMode = UPDATE_MODE_IDLE;
			MarkDirty();
		}
		else if (++mUpdateAcc >= 25) 
		{
			SetValue(mValue + mPageSize);
			mUpdateAcc = 20;
		}
		break;
	}
}
	
/*!***********************************
// @return    	int
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ScrollbarWidget::ThumbCompare(int x, int y) 
{
	int aTrackPosition; 

	if (mHorizontal)
		aTrackPosition = x;
	else
		aTrackPosition = y;
		
	if (aTrackPosition < GetThumbPosition()) 
		return -1;

	if (aTrackPosition >= GetThumbPosition() + GetThumbSize()) 
		return 1;

	return 0;
}
	
/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theBtnNum
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseDown(x, y, theBtnNum, theClickCount);
		
	if (!mDisabled)
	{
		switch (ThumbCompare(x, y))	
		{
		case -1:
			SetValue(mValue - mPageSize);
			mUpdateMode = UPDATE_MODE_PGUP;
			mUpdateAcc = 0;
			break;
		case 0:
			mPressedOnThumb = true;
			mMouseDownThumbPos = GetThumbPosition();
			mMouseDownX = x;
			mMouseDownY = y;
			break;
		case 1:
			SetValue(mValue + mPageSize);
			mUpdateMode = UPDATE_MODE_PGDN;
			mUpdateAcc = 0;
			break;
		}
	}
		
	mLastMouseX = x;
	mLastMouseY = y;
}		
	
/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theBtnNum
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::MouseUp(int x, int y, int theBtnNum, int theClickCount) 
{
	Widget::MouseUp(x, y, theBtnNum, theClickCount);

	mUpdateMode = 0;
	mPressedOnThumb = false;
	MarkDirty();
}
	
/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbarWidget::MouseDrag(int x, int y) 
{
	Widget::MouseDrag(x, y);
		
	if (mPressedOnThumb) 
	{
		if (mHorizontal)
			SetThumbPosition(mMouseDownThumbPos + x - mMouseDownX);
		else
			SetThumbPosition(mMouseDownThumbPos + y - mMouseDownY);
	}
		
	mLastMouseX = x;
	mLastMouseY = y;
}
