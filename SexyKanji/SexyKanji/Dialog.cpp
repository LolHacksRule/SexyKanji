// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Common.h"
#include "Dialog.h"
#include "DialogButton.h"
#include "DialogListener.h"
#include "SexyAppBase.h"
#include "WidgetManager.h"
#include "SexyFont.h"
#include "FontManager.h"
#include "SexyImage.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;


SexyString Sexy::DIALOG_YES_STRING				= _S("YES");
SexyString Sexy::DIALOG_NO_STRING				= _S("NO");
SexyString Sexy::DIALOG_OK_STRING				= _S("OK");
SexyString Sexy::DIALOG_CANCEL_STRING			= _S("CANCEL");

static int gDialogColors[][3] = 
{{255, 255, 255},
{255, 255, 0},
{255, 255, 255},
{255, 255, 255},
{255, 255, 255},

{80, 80, 80},
{255, 255, 255}};

/*!***********************************
// @return    	
// @param     	theComponentImage
// @param     	theButtonComponentImage
// @param     	theId
// @param     	isModal
// @param     	theDialogHeader
// @param     	theDialogLines
// @param     	theDialogFooter
// @param     	theButtonMode
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Dialog::Dialog(SexyImage* theComponentImage, SexyImage* theButtonComponentImage, int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)	
{
	mId = theId;
	mResult = 0x7FFFFFFF;
	mComponentImage = theComponentImage;
	mIsModal = isModal;
	mContentInsets = Insets(24, 24, 24, 24);
	mTextAlign = 0;
	mLineSpacingOffset = 0;
	mSpaceAfterHeader = 10;
	mButtonSidePadding = 0;
	mButtonHorzSpacing = 8;

	mDialogListener = NULL; //gSexyAppBase;

	mDialogHeader = theDialogHeader;
	mDialogFooter = theDialogFooter;
	mButtonMode = theButtonMode;

	if ((mButtonMode == BUTTONS_YES_NO) || (mButtonMode == BUTTONS_OK_CANCEL))
	{
		mYesButton = new DialogButton(theButtonComponentImage, ID_YES, this);
		mNoButton = new DialogButton(theButtonComponentImage, ID_NO, this);						

		if (mButtonMode == BUTTONS_YES_NO)
		{
			mYesButton->mLabel = DIALOG_YES_STRING;
			mNoButton->mLabel = DIALOG_NO_STRING;
		}
		else
		{
			mYesButton->mLabel = DIALOG_OK_STRING;
			mNoButton->mLabel = DIALOG_CANCEL_STRING;
		}
	}
	else if (mButtonMode == BUTTONS_FOOTER)
	{
		mYesButton = new DialogButton(theButtonComponentImage, ID_FOOTER, this);		
		mYesButton->mLabel = mDialogFooter;
		mNoButton = NULL;		
	}
	else
	{
		mYesButton = NULL;
		mNoButton = NULL;
		mNumButtons = 0;
	}

	mDialogLines = theDialogLines;

	mButtonHeight = (theButtonComponentImage == NULL) ? 24 : theButtonComponentImage->GetHeight();

	mHasTransparencies = true;	
	mHasAlpha = true;

	mDragging = false;	
	mPriority = 1;

	if (theButtonComponentImage == NULL)
	{
		gDialogColors[COLOR_BUTTON_TEXT][0] = 0;
		gDialogColors[COLOR_BUTTON_TEXT][1] = 0;
		gDialogColors[COLOR_BUTTON_TEXT][2] = 0;
		gDialogColors[COLOR_BUTTON_TEXT_HILITE][0] = 0;
		gDialogColors[COLOR_BUTTON_TEXT_HILITE][1] = 0;
		gDialogColors[COLOR_BUTTON_TEXT_HILITE][2] = 0;
	}

	SetColors(gDialogColors, NUM_COLORS);
}	


/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Dialog::~Dialog()
{
	delete mYesButton;
	delete mNoButton;
}

/*!***********************************
// @return    	void
// @param     	theIdx
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::SetColor(int theIdx, const SexyColor& theColor)
{
	Widget::SetColor(theIdx, theColor);
	
	if (theIdx == COLOR_BUTTON_TEXT)
	{
		if (mYesButton != NULL)
			mYesButton->SetColor(DialogButton::COLOR_LABEL, theColor);
		if (mNoButton != NULL)
			mNoButton->SetColor(DialogButton::COLOR_LABEL, theColor);
	}
	else if (theIdx == COLOR_BUTTON_TEXT_HILITE)
	{
		if (mYesButton != NULL)
			mYesButton->SetColor(DialogButton::COLOR_LABEL_HILITE, theColor);
		if (mNoButton != NULL)
			mNoButton->SetColor(DialogButton::COLOR_LABEL_HILITE, theColor);
	}	
}

/*!***********************************
// @return    	void
// @param     	theFont
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::SetButtonFont(SexyFont* theFont)
{
	if (mYesButton != NULL)
		mYesButton->SetFont(theFont);

	if (mNoButton != NULL)
		mNoButton->SetFont(theFont);
}

/*!***********************************
// @return    	void
// @param     	theFont
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::SetHeaderFont(SharedFontRef theFont)
{
	mHeaderFont = theFont;
}

/*!***********************************
// @return    	void
// @param     	theFont
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::SetLinesFont(SharedFontRef theFont)
{
	mLinesFont = theFont;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::EnsureFonts()
{
	if (((SexyFont*)mHeaderFont) == NULL)
	{
		mHeaderFont = gSexyAppBase->GetFontManager()->GetSharedFont(GetSystemFontsFolder() + _S("arial.ttf"), _S("14"));
		mHeaderFont->SetFontPointHeight(14);
	}

	if (((SexyFont*)mLinesFont) == NULL)
	{
		mLinesFont = gSexyAppBase->GetFontManager()->GetSharedFont(GetSystemFontsFolder() + _S("arial.ttf"), _S("12"));
		mLinesFont->SetFontPointHeight(12);
	}
}

/*!***********************************
// @return    	int
// @param     	theWidth
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int	Dialog::GetPreferredHeight(int theWidth)
{
	EnsureFonts();

	int aHeight = mContentInsets.mTop + mContentInsets.mBottom + mBackgroundInsets.mTop + mBackgroundInsets.mBottom;

	bool needSpace = false;
	if (mDialogHeader.length() > 0)
	{
		aHeight += mHeaderFont->GetHeight() - mHeaderFont->GetAscentPadding();
		needSpace = true;
	}
	
	if (mDialogLines.length() > 0)
	{
		if (needSpace)
			aHeight += mSpaceAfterHeader;
		Graphics g;
		g.SetFont(mLinesFont);	
		aHeight += g.GetWordWrappedHeight(mDialogLines, 
			theWidth-mContentInsets.mLeft-mContentInsets.mRight-mBackgroundInsets.mLeft-mBackgroundInsets.mRight-4, 
			mLinesFont->GetLineSpacing() + mLineSpacingOffset);
		needSpace = true;
	}

	if ((mDialogFooter.length() != 0) && (mButtonMode != BUTTONS_FOOTER))
	{
		if (needSpace)
			aHeight += 8;
		aHeight += mHeaderFont->GetLineSpacing();
		needSpace = true;
	}

	if (mYesButton != NULL)
	{
		if (needSpace)
			aHeight += 8;
		aHeight += mButtonHeight + 8;		
	}	

	return aHeight;
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::Draw(Graphics* g)
{
	EnsureFonts();

	SexyRect aBoxRect(mBackgroundInsets.mLeft,mBackgroundInsets.mTop,mWidth-mBackgroundInsets.mLeft-mBackgroundInsets.mRight,mHeight-mBackgroundInsets.mTop-mBackgroundInsets.mBottom);
	if (mComponentImage != NULL)
	{
		g->DrawImageBox(aBoxRect,mComponentImage);
	}
	else
	{
		g->SetColor(GetColor(COLOR_OUTLINE, SexyColor(gDialogColors[COLOR_OUTLINE])));
		g->DrawRect(12, 12, mWidth - 12*2 - 1, mHeight - 12*2 - 1);
		g->SetColor(GetColor(COLOR_BKG, SexyColor(gDialogColors[COLOR_BKG])));
		g->FillRect(12+1, 12+1, mWidth - 12*2 - 2, mHeight - 12*2 - 2);
		
		g->SetColor(SexyColor(0, 0, 0, 128));
		g->FillRect(mWidth - 12, 12*2, 12, mHeight - 12*3);
		g->FillRect(12*2, mHeight-12, mWidth - 12*2, 12);
	}

	int aCurY = mContentInsets.mTop + mBackgroundInsets.mTop;

	if (mDialogHeader.length() > 0)
	{
		//aCurY += mHeaderFont->GetAscent() - mHeaderFont->GetAscentPadding();
		
		g->SetFont(mHeaderFont);
		g->SetColor(mColors[COLOR_HEADER]);
		g->WriteString(mDialogHeader, 0, aCurY, mWidth);		

		aCurY += mHeaderFont->GetHeight();

		aCurY += mSpaceAfterHeader;
	}

	g->SetFont(mLinesFont);
	g->SetColor(mColors[COLOR_LINES]);

	SexyRect aRect(mBackgroundInsets.mLeft+mContentInsets.mLeft+2, aCurY, mWidth-mContentInsets.mLeft-mContentInsets.mRight-mBackgroundInsets.mLeft-mBackgroundInsets.mRight-4, 0);
	aCurY += g->WriteWordWrapped(aRect, mDialogLines, mLinesFont->GetLineSpacing() + mLineSpacingOffset, mTextAlign);	

	if ((mDialogFooter.length() != 0) && (mButtonMode != BUTTONS_FOOTER))
	{		
		aCurY += 8;
		aCurY += mHeaderFont->GetLineSpacing();
		
		g->SetFont(mHeaderFont);
		g->SetColor(mColors[COLOR_FOOTER]);
		g->WriteString(mDialogFooter, mX, aCurY, mWidth);		
	}	
}

/*!***********************************
// @return    	void
// @param     	theWidgetManager
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);

	if (mYesButton != NULL)
		theWidgetManager->AddWidget(mYesButton);
	if (mNoButton != NULL)
		theWidgetManager->AddWidget(mNoButton);
}

/*!***********************************
// @return    	void
// @param     	theWidgetManager
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);

	if (mYesButton != NULL)
		theWidgetManager->RemoveWidget(mYesButton);
	if (mNoButton != NULL)
		theWidgetManager->RemoveWidget(mNoButton);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::OrderInManagerChanged()
{
	Widget::OrderInManagerChanged();
	if (mYesButton != NULL)
		mWidgetManager->PutInfront(mYesButton,this);
	if (mNoButton != NULL)
		mWidgetManager->PutInfront(mNoButton,this);
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
void Dialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);	

	if ((mYesButton != NULL) && (mNoButton != NULL))
	{
		int aBtnWidth = (mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight - mButtonSidePadding*2 - mButtonHorzSpacing) / 2;
		int aBtnHeight = mButtonHeight;

		mYesButton->Resize(mX + mBackgroundInsets.mLeft + mContentInsets.mLeft + mButtonSidePadding, mY + mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - aBtnHeight, aBtnWidth, aBtnHeight);
		mNoButton->Resize(mYesButton->mX + aBtnWidth + mButtonHorzSpacing, mYesButton->mY, aBtnWidth, aBtnHeight);
	}
	else if (mYesButton != NULL)
	{
		int aBtnHeight = mButtonHeight;

		mYesButton->Resize(mX + mContentInsets.mLeft + mBackgroundInsets.mLeft, mY + mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - aBtnHeight, 
			mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight, aBtnHeight);	
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
void Dialog::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	if (theClickCount == 1)
	{
		mWidgetManager->mApp->SetCursor(SEXY_CURSOR_DRAGGING);
		mDragging = true;
		mDragMouseX = x;
		mDragMouseY = y;
	}
	Widget::MouseDown(x,y,theBtnNum, theClickCount);
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::MouseDrag(int x, int y)
{
	if (mDragging)
	{
		int aNewX = mX + x - mDragMouseX;
		int aNewY = mY + y - mDragMouseY;	

		if (aNewX < -8)
			aNewX = -8;
		else if (aNewX + mWidth > mWidgetManager->mWidth + 8)
			aNewX = mWidgetManager->mWidth - mWidth + 8;

		if (aNewY < -8)
			aNewY = -8;
		else if (aNewY + mHeight > mWidgetManager->mHeight + 8)
			aNewY = mWidgetManager->mHeight- mHeight + 8;

		mDragMouseX = mX + x - aNewX;
		mDragMouseY = mY + y - aNewY;

		if (mDragMouseX < 8)
			mDragMouseX = 8;
		else if (mDragMouseX > mWidth-9)
			mDragMouseX = mWidth-9;

		if (mDragMouseY < 8)
			mDragMouseY = 8;
		else if (mDragMouseY > mHeight-9)
			mDragMouseY = mHeight-9;

		Move(aNewX, aNewY);
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
void Dialog::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
	if (mDragging)
	{
		mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);		
		mDragging = false;
	}
	Widget::MouseUp(x,y, theBtnNum, theClickCount);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::Update()
{
	Widget::Update();

	//Move(mX, mY+1);
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Dialog::IsModal()
{
	return mIsModal;
}

/*!***********************************
// @return    	int
// @param     	autoKill
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Dialog::WaitForResult(bool autoKill)
{	
	//gSexyAppBase->DoMainLoop(mId);	

	//while ((gSexyAppBase->UpdateAppStep(NULL)) && (mWidgetManager != NULL) && (mResult == 0x7FFFFFFF));

	//if (autoKill)
	//	gSexyAppBase->KillDialog(mId);

	return mResult;
}

/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::ButtonPress(int theId)
{
	if (mDialogListener != NULL && ((theId == ID_YES) || (theId == ID_NO)))
		mDialogListener->DialogButtonPress(mId, theId);
}

/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::ButtonDepress(int theId)
{
	if (mDialogListener != NULL && ((theId == ID_YES) || (theId == ID_NO)))
	{
		mResult = theId;		
		mDialogListener->DialogButtonDepress(mId, theId);
	}
	else
	{
		gSexyAppBase->GetWidgetManager()->KillDialog(mId);
	}
}

/*!***********************************
// @return    	void
// @param     	theId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Dialog::ButtonDownTick(int theId)
{	
}
