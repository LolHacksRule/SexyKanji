// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "ListWidget.h"
#include "SexyFont.h"
#include "WidgetManager.h"
#include "ScrollbarWidget.h"
#include "ListListener.h"
#include "SexyAppBase.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

static int gInitialListWidgetColors[][3] = {{255, 255, 255}, {255, 255, 255}, {0, 0, 0}, {0, 192, 0}, {0, 0, 128}, {255, 255, 255}};

/*!***********************************
// @return    	
// @param     	theId
// @param     	theFont
// @param     	theListListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ListWidget::ListWidget(int theId, SexyFont *theFont, ListListener *theListListener) 
{
	mJustify = JUSTIFY_LEFT;
	mHiliteIdx = -1;
	mSelectIdx = -1;

	if (theFont != NULL)
		mItemHeight = theFont->GetHeight();
	else
		mItemHeight = -1;

	SetColors(gInitialListWidgetColors, 6);
		
	mId = theId;
	mFont = theFont;
	mListListener = theListListener;
	mParent = NULL;
	mChild = NULL;
	mScrollbar = NULL;
	mPosition = 0;
	mPageSize = 0;
	mSortFromChild = false;
	mDrawOutline = true;
	mMaxNumericPlaces = 0;	
	mDrawSelectWhenHilited = false;
	mDoFingerWhenHilited = true;
}
	
/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ListWidget::~ListWidget() 
{
}

/*!***********************************
// @return    	void
// @param     	theManager
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::RemovedFromManager(WidgetManager *theManager)
{
	Widget::RemovedFromManager(theManager);
	if (mListListener)
		mListListener->ListClosed(mId);
}

/*!***********************************
// @return    	SexyString
// @param     	theIdx
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString ListWidget::GetSortKey(int theIdx) 
{
	SexyString aString = mLines[theIdx];
	
	while (aString.length() < (ulong) mMaxNumericPlaces) 
		aString = _S("0") + aString;

	if (mSortFromChild) 
		return mChild->GetSortKey(theIdx) + aString;
	else 
	{
		if (mChild == NULL)	
			return aString;
		else 
			return aString + mChild->GetSortKey(theIdx);
	}

	return _S("");
}
	
/*!***********************************
// @return    	void
// @param     	ascending
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::Sort(bool ascending) 
{
	int aCount = mLines.size();
	int* aMap = new int[aCount];
	SexyString* aKeys = new SexyString[aCount];

	int i;
	for (i = 0; i < aCount; i++) 
	{
		aMap[i] = i;
		aKeys[i] = GetSortKey(i);		
	}
		
	for (i = 1; i < aCount; i++) 
		for (int j = 0; j < aCount - i; j++) 
		{
			int aComp = aKeys[j].compare(aKeys[j+1]);
			if ((ascending && (aComp > 0)) || (!ascending && (aComp < 0)))
			{
				int aSwapInt = aMap[j];
				aMap[j] = aMap[j+1];
				aMap[j+1] = aSwapInt;
						
				SexyString aSwapKey = aKeys[j];
				aKeys[j] = aKeys[j+1];
				aKeys[j+1] = aSwapKey;
			}
		}
		
	ListWidget *aListWidget = this;
	while (aListWidget->mParent != NULL)	
		aListWidget = aListWidget->mParent;

	while (aListWidget != NULL) 
	{
		SexyStringVector aNewLines;
		ColorVector aNewLineColors;
			
		for (int i = 0; i < aCount; i++)
		{
			aNewLines.push_back(aListWidget->mLines[aMap[i]]);
			aNewLineColors.push_back(aListWidget->mLineColors[aMap[i]]);
		}
		
		aListWidget->mLines = aNewLines;
		aListWidget->mLineColors = aNewLineColors;
		
		aListWidget->MarkDirty();
						
		aListWidget = aListWidget->mChild;
	}

	delete aMap;
	delete aKeys;
}
	
/*!***********************************
// @return    	SexyString
// @param     	theIdx
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString ListWidget::GetStringAt(int theIdx) 
{
	return
		mLines[theIdx];
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
void ListWidget::Resize(int theX, int theY, int theWidth, int theHeight) 
{
	Widget::Resize(theX, theY, theWidth, theHeight);
	
	double aPageSize = 1;			
	int anItemHeight = (mItemHeight != -1) ? mItemHeight : mFont->GetHeight();
	
	if (mHeight > anItemHeight+8) 
		aPageSize = (mHeight - 8.0) / anItemHeight;
		
	mPageSize = aPageSize;
		
	if (mScrollbar != NULL) 
		mScrollbar->SetPageSize(aPageSize);
}
	
/*!***********************************
// @return    	int
// @param     	theLine
// @param     	alphabetical
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ListWidget::AddLine(const SexyString& theLine, bool alphabetical) 
{	
	int anIdx = -1;
	bool inserted = false;

	if (alphabetical) 
	{	
		for (int i = 0;	i < (int) mLines.size(); i++) 		
			if (sexystrcmp(theLine.c_str(), mLines[i].c_str()) < 0) 
			{
				anIdx = i;
						
				ListWidget *aListWidget = this;

				while (aListWidget->mParent != NULL) 
					aListWidget = aListWidget->mParent;

				while (aListWidget != NULL) 
				{
					if (aListWidget == this)
						aListWidget->mLines.insert(aListWidget->mLines.begin() + i, theLine);
					else 
						aListWidget->mLines.insert(aListWidget->mLines.begin() + i, _S("-"));
					
					aListWidget->mLineColors.insert(aListWidget->mLineColors.begin() + i, mColors[COLOR_TEXT]);
					aListWidget->MarkDirty();
					
					aListWidget = aListWidget->mChild;
				}
						
				inserted = true;
				break;
			}
	}
		
	if (!inserted) 
	{
		anIdx = mLines.size();
			
		ListWidget *aListWidget = this;
		
		while (aListWidget->mParent!=NULL) 
			aListWidget = aListWidget->mParent;

		while (aListWidget!=NULL) 
		{
			if (aListWidget==this) 
				aListWidget->mLines.push_back(theLine);
			else 
				aListWidget->mLines.push_back(_S("-"));
						
			aListWidget->mLineColors.push_back(mColors[COLOR_TEXT]);
			aListWidget->MarkDirty();
				
			aListWidget = aListWidget->mChild;
		}
	}
		
	if (mScrollbar!=NULL)
		mScrollbar->SetMaxValue(mLines.size());		
		
	return anIdx;
}

	
/*!***********************************
// @return    	void
// @param     	theIdx
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetLine(int theIdx, const SexyString& theString) 
{
	mLines[theIdx] = theString;	
	MarkDirty();
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ListWidget::GetLineCount() 
{
	return mLines.size();	
}
	
/*!***********************************
// @return    	int
// @param     	theLine
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ListWidget::GetLineIdx(const SexyString& theLine) 
{	
	for (ulong i = 0; i < mLines.size(); i++)	
		if (sexystrcmp(mLines[i].c_str(), theLine.c_str()) == 0)
			return i;
	
	return -1;
}
	
/*!***********************************
// @return    	void
// @param     	theLine
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetColor(const SexyString& theLine, const SexyColor& theColor) 
{
	int anIdx = GetLineIdx(theLine);
	SetLineColor(anIdx, theColor);	
}

/*!***********************************
// @return    	void
// @param     	theIdx
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetColor(int theIdx, const SexyColor& theColor)
{
	Widget::SetColor(theIdx, theColor);
}
		
/*!***********************************
// @return    	void
// @param     	theIdx
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetLineColor(int theIdx, const SexyColor& theColor)
{
	if ((theIdx >= 0) && (theIdx < (int)mLines.size()))
	{
		ListWidget *aListWidget = this;

		while (aListWidget->mParent != NULL) 
			aListWidget = aListWidget->mParent;

		while (aListWidget != NULL) 
		{
			aListWidget->mLineColors[theIdx] = theColor;
			aListWidget->MarkDirty();
				
			aListWidget = aListWidget->mChild;
		}			
	}
}

/*!***********************************
// @return    	void
// @param     	theIdx
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::RemoveLine(int theIdx) 
{
	if (theIdx != -1) 
	{
		ListWidget *aListWidget = this;

		while (aListWidget->mParent != NULL) 
			aListWidget = aListWidget->mParent;

		while (aListWidget != NULL) 
		{
			aListWidget->mLines.erase(aListWidget->mLines.begin() + theIdx);
			aListWidget->mLineColors.erase(aListWidget->mLineColors.begin() + theIdx);
				
			aListWidget->MarkDirty();
			aListWidget = aListWidget->mChild;	
		}
	}
		
	if (mScrollbar != NULL) 
		mScrollbar->SetMaxValue(mLines.size());
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::RemoveAll() 
{
	ListWidget *aListWidget = this;

	while (aListWidget->mParent != NULL) 
		aListWidget = aListWidget->mParent;			

	while (aListWidget != NULL) 
	{
		aListWidget->mLines.clear();
		aListWidget->mLineColors.clear();
		aListWidget->mSelectIdx = -1;
		aListWidget->mHiliteIdx = -1;
				
		aListWidget->MarkDirty();
		aListWidget = aListWidget->mChild;	
	}

	if (mScrollbar != NULL) 
		mScrollbar->SetMaxValue(mLines.size());
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ListWidget::GetOptimalWidth() 
{
	int aMaxWidth = 0;
	
	for (ulong i = 0; i < mLines.size(); i++) 
		aMaxWidth = K_MAX(aMaxWidth, mFont->StringWidth(mLines[i]));

	return aMaxWidth + 16;
}
	
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int ListWidget::GetOptimalHeight() 
{
	int anItemHeight = (mItemHeight != -1) ? mItemHeight : mFont->GetHeight();

	return anItemHeight * mLines.size() + 8;
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::OrderInManagerChanged()
{
	Widget::OrderInManagerChanged();

	if (mChild != NULL)
		gSexyAppBase->GetWidgetManager()->PutInfront(mChild, this);
	if (mScrollbar != NULL)
		gSexyAppBase->GetWidgetManager()->PutInfront(mScrollbar, this);
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::Draw(Graphics *g) 
{
	g->SetColor(mColors[COLOR_BKG]);
	g->FillRect(0, 0, mWidth, mHeight);

	Graphics aClipG(*g);
	aClipG.ClipRect(4, 4, mWidth - 8, mHeight - 8);

	Graphics aSelectClipG(*g);
	aSelectClipG.ClipRect(0, 4, mWidth, mHeight - 8);

	aClipG.SetFont(mFont);

	int aFirstLine = (int) mPosition;
	int aLastLine = K_MIN((int) mLines.size()-1, (int) mPosition + (int) mPageSize + 1);

	int anItemHeight, anItemOffset;
	if (mItemHeight != -1)
	{
		anItemHeight = mItemHeight;
		anItemOffset = (anItemHeight - mFont->GetHeight()) / 2;
	}
	else
	{
		anItemHeight = mFont->GetHeight();
		anItemOffset = 0;
	}

	for (int i = aFirstLine; i <= aLastLine; i++)
	{
		int aDrawY = 4 + (int) ((i - mPosition) * anItemHeight);

		if (i == mSelectIdx || (i==mHiliteIdx && mDrawSelectWhenHilited))
		{
			aSelectClipG.SetColor(mColors[COLOR_SELECT]);
			aSelectClipG.FillRect(0, aDrawY, mWidth, anItemHeight);
		}

		if (i == mHiliteIdx)
			aClipG.SetColor(mColors[COLOR_HILITE]);
		else if ((i == mSelectIdx) && (mColors.size() > COLOR_SELECT_TEXT))
			aClipG.SetColor(mColors[COLOR_SELECT_TEXT]);
		else
			aClipG.SetColor(mLineColors[i]);

		SexyString aString = mLines[i];
		int aFontX;
		switch (mJustify)
		{
		case JUSTIFY_LEFT:
			aFontX = 4;
			break;
		case JUSTIFY_CENTER:
			aFontX = (mWidth - mFont->StringWidth(aString))/2;
			break;
		default:
			aFontX = mWidth - mFont->StringWidth(aString) - 4;
			break;
		}
		aClipG.DrawStringClipped(aString, aFontX, aDrawY += anItemOffset);
	}

	if (mDrawOutline)
	{
		g->SetColor(mColors[COLOR_OUTLINE]);
		g->DrawRect(0, 0, mWidth - 1, mHeight - 1);
	}
}

/*!***********************************
// @return    	void
// @param     	theId
// @param     	thePosition
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::ScrollPosition(int theId, double thePosition) 
{
	if (mChild != NULL)	
		mChild->ScrollPosition(theId, thePosition);
		
	mPosition = thePosition;						
	MarkDirty();
}

/*!***********************************
// @return    	void
// @param     	theHiliteIdx
// @param     	notifyListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetHilite(int theHiliteIdx, bool notifyListener)
{
	int anOldIdx = mHiliteIdx;
	mHiliteIdx = theHiliteIdx;
	if (anOldIdx!=mHiliteIdx && notifyListener && mListListener!=NULL)
		mListListener->ListHiliteChanged(mId,anOldIdx,mHiliteIdx);
}

	
/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::MouseMove(int x, int y) 
{
	int anItemHeight = (mItemHeight != -1) ? mItemHeight : mFont->GetHeight();
		
	int aNewHilite = (int) (((y - 4) / (double) anItemHeight) + mPosition);
	if ((aNewHilite < 0) || (aNewHilite >= (int) mLines.size())) 
		aNewHilite = -1;
		
	if (aNewHilite != mHiliteIdx) 
	{
		ListWidget* aListWidget = this;

		while (aListWidget->mParent != NULL) 
			aListWidget = aListWidget->mParent;

		while (aListWidget != NULL) 
		{
			aListWidget->SetHilite(aNewHilite,true);
			aListWidget->MarkDirty();
			aListWidget = aListWidget->mChild;	
		}
									
		if (mHiliteIdx == -1 || !mDoFingerWhenHilited) 
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
		else 
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_HAND);
	}
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
void ListWidget::MouseDown(int x, int y, int theBtnNum, int theClickCount) 
{
	if ((mHiliteIdx != -1) && (mListListener != NULL))
		mListListener->ListClicked(mId, mHiliteIdx, theClickCount);
}
	
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::MouseLeave() 
{
	ListWidget *aListWidget = this;

	while (aListWidget->mParent != NULL)
		aListWidget = aListWidget->mParent;			

	while (aListWidget != NULL) 
	{
		aListWidget->SetHilite(-1,true);	
		aListWidget->MarkDirty();	
		aListWidget = aListWidget->mChild;
	}
		
	mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
}

/*!***********************************
// @return    	void
// @param     	theSelectIdx
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::SetSelect(int theSelectIdx)
{
	ListWidget *aListWidget = this;

	while (aListWidget->mParent != NULL)
		aListWidget = aListWidget->mParent;

	while (aListWidget != NULL)	
	{
		aListWidget->mSelectIdx = theSelectIdx;
		aListWidget->MarkDirty();	
		aListWidget = aListWidget->mChild;
	}
}


/*!***********************************
// @return    	void
// @param     	theDelta
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ListWidget::MouseWheel(int theDelta)
{
	if (mScrollbar != NULL)
	{
		int aScrollAmount = 5;
		if (theDelta > 0)
		{
			mScrollbar->SetValue(mScrollbar->mValue - aScrollAmount);
//			mScrollbar->mUpdateMode = ScrollbarWidget::UPDATE_MODE_PGUP;
//			mScrollbar->mUpdateAcc = 0;
		}
		else if (theDelta < 0)
		{
			mScrollbar->SetValue(mScrollbar->mValue + aScrollAmount);
//			mScrollbar->mUpdateMode = ScrollbarWidget::UPDATE_MODE_PGDN;
//			mScrollbar->mUpdateAcc = 0;
		}
	}
}
