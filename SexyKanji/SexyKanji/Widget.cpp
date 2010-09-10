// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Widget.h"

#include "WidgetManager.h"
#include "Graphics.h"
#include "SexyFont.h"
#include "SexyImage.h"
#include "SexyAppBase.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Widget::Widget()
{
	mWidgetManager = NULL;	
	mVisible = true;
	mDisabled = false;
	mIsDown = false;
	mIsOver = false;
	mDoFinger = false;	
	mMouseVisible = true;		
	mHasFocus = false;
	mHasTransparencies = false;	
	mWantsFocus = false;
	mTabPrev = NULL;
	mTabNext = NULL;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Widget::~Widget()
{	
	mColors.clear();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::WidgetRemovedHelper()
{
	if (mWidgetManager==NULL)
		return;

	// Call RemovedFromManager on all child widgets and disable them and stuff like that
	for (WidgetList::iterator aWidgetItr = mWidgets.begin(); aWidgetItr != mWidgets.end(); ++aWidgetItr)
	{
		Widget *aWidget = *aWidgetItr;
		aWidget->WidgetRemovedHelper();
	}	

	mWidgetManager->DisableWidget(this);

	PreModalInfoList::iterator anItr = mWidgetManager->mPreModalInfoList.begin();
	while (anItr != mWidgetManager->mPreModalInfoList.end())
	{
		PreModalInfo* aPreModalInfo = &(*anItr);
		if (aPreModalInfo->mPrevBaseModalWidget == this)
			aPreModalInfo->mPrevBaseModalWidget = NULL;
		if (aPreModalInfo->mPrevFocusWidget == this)
			aPreModalInfo->mPrevFocusWidget = NULL;
		++anItr;
	}
	
	RemovedFromManager(mWidgetManager);
	MarkDirtyFull(this);

	mWidgetManager = NULL;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::OrderInManagerChanged()
{
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
//
// \brief		Per-Pixel Hit Test
// \details 	<b>Description:</b> Override this method in your custom widget class
//				to affect per-pixel handling
//************************************/
bool Widget::IsPointVisible(int x, int y)
{
	return true;
}

/*!***********************************
// @return    	void
// @param     	isVisible
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::SetVisible(bool isVisible)
{
	if (mVisible == isVisible)
		return;
	
	mVisible = isVisible;
	
	if (mVisible)
		MarkDirty();
	else
		MarkDirtyFull();

	if (mWidgetManager != NULL)
		mWidgetManager->RehupMouse();
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::Draw(Graphics* g) // Already translated
{
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::DrawOverlay(Graphics* g)
{
}

/*!***********************************
// @return    	void
// @param     	g
// @param     	thePriority
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::DrawOverlay(Graphics* g, int thePriority)
{
	DrawOverlay(g);
}

/*!***********************************
// @return    	void
// @param     	theColors
// @param     	theNumColors
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::SetColors(int theColors[][3], int theNumColors)
{
	mColors.clear();

	for (int i = 0; i < theNumColors; i++)
		SetColor(i, SexyColor(theColors[i][0], theColors[i][1], theColors[i][2]));
	MarkDirty();
}

/*!***********************************
// @return    	void
// @param     	theColors
// @param     	theNumColors
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::SetColors(int theColors[][4], int theNumColors)
{	
	mColors.clear();

	for (int i = 0; i < theNumColors; i++)
		SetColor(i, SexyColor(theColors[i][0], theColors[i][1], theColors[i][2], theColors[i][3]));		

	MarkDirty();
}

/*!***********************************
// @return    	void
// @param     	theIdx
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::SetColor(int theIdx, const SexyColor& theColor)
{
	if (theIdx >= (int)mColors.size())
		mColors.resize(theIdx + 1);

	mColors[theIdx] = theColor;
	MarkDirty();
}

/*!***********************************
// @return    	const SexyColor&
// @param     	theIdx
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
const SexyColor& Widget::GetColor(int theIdx)
{
	static SexyColor aColor;
	if (theIdx < (int) mColors.size())
		return mColors[theIdx];
	return aColor;
}

/*!***********************************
// @return    	Sexy::SexyColor
// @param     	theIdx
// @param     	theDefaultColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor Widget::GetColor(int theIdx, const SexyColor& theDefaultColor)
{
	if (theIdx < (int) mColors.size())
		return mColors[theIdx];
	return theDefaultColor;
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
void Widget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	if ((mX == theX) && (mY == theY) && (mWidth == theWidth) && (mHeight == theHeight))
		return;

	// Mark everything dirty that is over or under the old position
	MarkDirtyFull();
	
	mX = theX;
	mY = theY;
	if (theWidth >= 0)
	    mWidth = theWidth;
	if (theHeight >= 0)
	    mHeight = theHeight;
		
	// Mark things dirty that are over the new position
	MarkDirty();

	if (mWidgetManager != NULL)
		mWidgetManager->RehupMouse();
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::Resize(const SexyRect& theRect)
{
	Resize(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theNewX
// @param     	theNewY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::Move(int theNewX, int theNewY)
{
	Resize(theNewX, theNewY, mWidth, mHeight);
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Widget::WantsFocus()
{
	return mWantsFocus;
}

/*!***********************************
// @return    	void
// @param     	isDisabled
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::SetDisabled(bool isDisabled)
{
	if (mDisabled == isDisabled)
		return;

	mDisabled = isDisabled;

	if ((isDisabled) && (mWidgetManager != NULL))
		mWidgetManager->DisableWidget(this);
		
	MarkDirty();
	
	// Incase a widget is enabled right under our cursor
	if ((!isDisabled) && (mWidgetManager != NULL) && (Contains(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY)))
		mWidgetManager->MousePosition(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::GotFocus()
{
	mHasFocus = true;		
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::LostFocus()
{
	mHasFocus = false;		
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::Update()
{
	WidgetContainer::Update();
}

/*!***********************************
// @return    	void
// @param     	theFrac
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::UpdateF(float theFrac)
{
}

/*!***********************************
// @return    	void
// @param     	theChar
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::KeyChar(SexyChar theChar)
{
}

/*!***********************************
// @return    	void
// @param     	theKey
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::KeyDown(EKeyboardLayout theKey)
{
	if (theKey == K_VK_TAB)
	{
		if (mWidgetManager->mKeyDown[K_VK_L_SHIFT] || mWidgetManager->mKeyDown[K_VK_R_SHIFT])
		{
			if (mTabPrev != NULL)
				mWidgetManager->SetFocus(mTabPrev);
		}
		else
		{
			if (mTabNext != NULL)
				mWidgetManager->SetFocus(mTabNext);
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theKey
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::KeyUp(EKeyboardLayout theKey)
{		
}

/*!***********************************
// @return    	void
// @param     	on
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::ShowFinger(bool on)
{
	if (mWidgetManager == NULL)
		return;

	if (on)
		mWidgetManager->mApp->SetCursor(SEXY_CURSOR_HAND);
	else
		mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseEnter()
{
	
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseLeave()
{
	
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseMove(int x, int y)
{
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount == 3)
		MouseDown(x, y, 2, 1);
	else if (theClickCount >= 0)
		MouseDown(x, y, 0, theClickCount);
	else
		MouseDown(x, y, 1, -theClickCount);
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
void Widget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseUp(int x, int y)
{
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theLastDownButtonId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseUp(int x, int y, int theLastDownButtonId)
{
	MouseUp(x, y);

	if (theLastDownButtonId == 3)
		MouseUp(x, y, 2, 1);
	else if (theLastDownButtonId >= 0)
		MouseUp(x, y, 0, theLastDownButtonId);
	else
		MouseUp(x, y, 1, -theLastDownButtonId);
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
void Widget::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseDrag(int x, int y)
{
}

/*!***********************************
// @return    	void
// @param     	theDelta
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::MouseWheel(int theDelta)
{
}

/*!***********************************
// @return    	int
// @param     	theNumber
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Widget::GetNumDigits(int theNumber)
{		
	int aDivisor = 10;
	int aNumDigits = 1;
	while (theNumber >= aDivisor)
	{
		aNumDigits++;
		aDivisor *= 10;
	}			
		
	return aNumDigits;
}
								 
/*!***********************************
// @return    	bool
// @param     	theX
// @param     	theY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Widget::Contains(int theX, int theY)
{
	return ((theX >= mX) && (theX < mX + mWidth) &&
			(theY >= mY) && (theY < mY + mHeight));
}

/*!***********************************
// @return    	Sexy::SexyRect
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyRect Widget::GetInsetRect()
{
	return SexyRect(mX + mMouseInsets.mLeft, mY + mMouseInsets.mTop, 
						 mWidth - mMouseInsets.mLeft - mMouseInsets.mRight,
						 mHeight - mMouseInsets.mTop - mMouseInsets.mBottom);
}

/*!***********************************
// @return    	void
// @param     	thePriority
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::DeferOverlay(int thePriority)
{
	mWidgetManager->DeferOverlay(this, thePriority);
}

/*!***********************************
// @return    	void
// @param     	theLayoutFlags
// @param     	theRelativeWidget
// @param     	theLeftPad
// @param     	theTopPad
// @param     	theWidthPad
// @param     	theHeightPad
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Widget::Layout(int theLayoutFlags, Widget *theRelativeWidget, int theLeftPad, int theTopPad, int theWidthPad, int theHeightPad)
{
	int aRelLeft = theRelativeWidget->Left();
	int aRelTop = theRelativeWidget->Top();
	if (theRelativeWidget==mParent)
	{
		aRelLeft = 0;
		aRelTop = 0;
	}

	int aRelWidth = theRelativeWidget->Width();
	int aRelHeight = theRelativeWidget->Height();
	int aRelRight = aRelLeft + aRelWidth;
	int aRelBottom = aRelTop + aRelHeight;

	int aLeft = Left();
	int aTop = Top();
	int aWidth = Width();
	int aHeight = Height();

	int aType = 1;
	while(aType<LAY_Max)
	{
		if(theLayoutFlags&aType)
		{
			switch(aType)
			{
				case LAY_SameWidth: aWidth = aRelWidth+theWidthPad; break;
				case LAY_SameHeight: aHeight = aRelHeight+theHeightPad; break;
	
				case LAY_Above: aTop = aRelTop-aHeight+theTopPad; break;
				case LAY_Below: aTop = aRelBottom+theTopPad; break;
				case LAY_Right: aLeft = aRelRight+theLeftPad; break;
				case LAY_Left:  aLeft = aRelLeft-aWidth+theLeftPad; break;
			
				case LAY_SameLeft: aLeft = aRelLeft+theLeftPad; break;
				case LAY_SameRight: aLeft = aRelRight-aWidth+theLeftPad; break;
				case LAY_SameTop: aTop = aRelTop+theTopPad; break;
				case LAY_SameBottom: aTop = aRelBottom-aHeight+theTopPad; break;

				case LAY_GrowToRight: aWidth = aRelRight-aLeft+theWidthPad; break;
				case LAY_GrowToLeft: aWidth = aRelLeft-aLeft+theWidthPad; break;
				case LAY_GrowToTop: aHeight = aRelTop-aTop+theHeightPad; break;
				case LAY_GrowToBottom: aHeight = aRelBottom-aTop+theHeightPad; break;

				case LAY_SetLeft: aLeft = theLeftPad; break;
				case LAY_SetTop: aTop = theTopPad; break;
				case LAY_SetWidth: aWidth = theWidthPad; break;
				case LAY_SetHeight: aHeight = theHeightPad; break;

				case LAY_HCenter: aLeft = aRelLeft+(aRelWidth-aWidth)/2 + theLeftPad; break;
				case LAY_VCenter: aTop = aRelTop+(aRelHeight-aHeight)/2 + theTopPad; break;
			}
		}

		aType<<=1;
	}

	Resize(aLeft,aTop,aWidth,aHeight);
}
