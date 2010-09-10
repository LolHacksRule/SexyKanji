// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "EditWidget.h"
#include "WidgetManager.h"
#include "SexyAppBase.h"
#include "EditListener.h"
#include "SexyFont.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

static int gEditWidgetColors[][3] = 
{{255, 255, 255},
{0, 0, 0},
{0, 0, 0},
{0, 0, 0},
{255, 255, 255}};

/*!***********************************
// @return    	
// @param     	theId
// @param     	theEditListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
EditWidget::EditWidget(int theId, EditListener* theEditListener)
{		
	mId = theId;
	mEditListener = theEditListener;	
	mFont = NULL;

	mHadDoubleClick = false;
	mHilitePos = -1;
	mLastModifyIdx = -1;
	mLeftPos = 0;
	mUndoCursor = 0;
	mUndoHilitePos = 0;
	mLastModifyIdx = 0;
	mBlinkAcc = 0;
	mCursorPos = 0;
	mShowingCursor = false;
	mDrawSelOverride = false;
	mMaxChars = -1;
	mMaxPixels = -1;
	mPasswordChar = 0;
	mBlinkDelay = 40;

	SetColors(gEditWidgetColors, NUM_COLORS);
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
EditWidget::~EditWidget()
{
	mFont = NULL;
	ClearWidthCheckFonts();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::ClearWidthCheckFonts()
{
	mWidthCheckList.clear();
}

/*!***********************************
// @return    	void
// @param     	theFont
// @param     	theMaxPixels
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::AddWidthCheckFont(SexyFont *theFont, int theMaxPixels)
{
	mWidthCheckList.push_back(WidthCheck());
	WidthCheck &aCheck = mWidthCheckList.back();
	aCheck.mWidth = theMaxPixels;
	aCheck.mFont = theFont;
}

/*!***********************************
// @return    	void
// @param     	theText
// @param     	leftPosToZero
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::SetText(const SexyString& theText, bool leftPosToZero)
{
	mString = theText;
	mCursorPos = mString.length();
	mHilitePos = 0;
	if (leftPosToZero)
		mLeftPos = 0;
	else
		FocusCursor(true);
	
	MarkDirty();
}

/*!***********************************
// @return    	SexyString&
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyString& EditWidget::GetDisplayString()
{
	if (mPasswordChar==0)
		return mString;

	if (mPasswordDisplayString.size()!=mString.size())
	{
		mPasswordDisplayString = SexyString(mString.size(), mPasswordChar);
		//mPasswordDisplayString.resize(mString.size());
		//for (int i=0; i<(int)mPasswordDisplayString.length(); i++)
		//	mPasswordDisplayString[i] = mPasswordChar; 
	}

	return mPasswordDisplayString;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool EditWidget::WantsFocus()
{
	return true;
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
void EditWidget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);

	FocusCursor(false);		
}

/*!***********************************
// @return    	void
// @param     	theFont
// @param     	theWidthCheckFont
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::SetFont(SexyFont* theFont, SexyFont* theWidthCheckFont)
{
	mFont = theFont;

	ClearWidthCheckFonts();
	if (theWidthCheckFont != NULL)
		AddWidthCheckFont(theWidthCheckFont);
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::Draw(Graphics* g) // Already translated
{
	if (mFont == NULL) return;
		//mFont = new SysFont(mWidgetManager->mApp, "Arial Unicode MS", 10, false);

	g->SetFont(mFont);
	SexyString &aString = GetDisplayString();

	// Draw the background rect.
	g->SetColor(mColors[COLOR_BKG]);
	g->FillRect(0, 0, mWidth, mHeight);

	//  Get the current cursor position.
	int aCursorX = mFont->StringWidth(aString.substr(0, mCursorPos)) - mFont->StringWidth(aString.substr(0, mLeftPos));
	aCursorX = K_MIN(K_MAX(0, aCursorX), mWidth-8);

	// Get the current highlight position.
	int aHiliteX = aCursorX;
	if ((mHilitePos != -1) && (mCursorPos != mHilitePos))
	{
		aHiliteX = mFont->StringWidth(aString.substr(0, mHilitePos)) - mFont->StringWidth(aString.substr(0, mLeftPos));
		aHiliteX = K_MIN(K_MAX(0, aHiliteX), mWidth-8);
	}

	//  When drawing the cursor add 2px for cursor width.
	int aWidth = std::abs(aHiliteX - aCursorX);
	int x = 4 + K_MIN(aCursorX, aHiliteX);
	if (mShowingCursor)
	{
		aWidth += 2;
		if (aHiliteX > aCursorX)
		{
			x -= 2;
		}
	}

	// Draw the cursor/highlighted rect.
	if (aWidth > 0)
	{
		g->SetColor(mColors[COLOR_HILITE]);
		g->FillRect(x, 0, aWidth, mHeight);
	}

	g->SetColor(mColors[COLOR_TEXT]);
	g->DrawStringClipped(aString.substr(mLeftPos), 4, (mHeight / 2) - (mFont->GetHeight() / 2));

	if (mHilitePos > -1)
	{
		SexyString aClippedString = aString.substr(K_MIN(K_MAX(mHilitePos, mLeftPos), mCursorPos), std::abs(mCursorPos - K_MAX(mHilitePos, mLeftPos)));
		g->SetColor(mColors[COLOR_HILITE_TEXT]);
		g->DrawStringClipped(aClippedString, K_MIN(aHiliteX, aCursorX)+4, (mHeight / 2) - (mFont->GetHeight() / 2));
	}

	g->SetColor(mColors[COLOR_OUTLINE]);
	g->DrawRect(0, 0, mWidth - 1, mHeight - 1);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::UpdateCaretPos()
{
	SexyAppBase *anApp = mWidgetManager->mApp;

	SexyPoint aPoint = GetAbsPos();

	if (aPoint.mX<10) aPoint.mX = 10;
	else if (aPoint.mX>anApp->GetWidth()-10) aPoint.mX = anApp->GetWidth()-10;
	if (aPoint.mY<10) aPoint.mY = 10;
	else if (aPoint.mY>anApp->GetHeight()-10) aPoint.mY = anApp->GetHeight()-10;

#ifdef K_WIN32
	SetCaretPos(aPoint.mX,aPoint.mY);
#endif
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::GotFocus()
{
	Widget::GotFocus();
	//if (mWidgetManager && mWidgetManager->mApp->mTabletPC)
	//{
	//	SexyAppBase *anApp = mWidgetManager->mApp;

	//	CreateCaret(anApp->mHWnd,NULL,0,0);
	//	UpdateCaretPos();
	//	ShowCaret(anApp->mHWnd);
	//}
	
	mShowingCursor = true;
	mBlinkAcc = 0;
	MarkDirty();	
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::LostFocus()
{
	Widget::LostFocus();

	//if (mWidgetManager && mWidgetManager->mApp->mTabletPC)
	//{
	//	HideCaret(mWidgetManager->mApp->mHWnd);
	//	DestroyCaret();
	//}

	mShowingCursor = false;	
	MarkDirty();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::Update()
{
	Widget::Update();

	if (mHasFocus)
	{
		//if (mWidgetManager->mApp->mTabletPC)
		//{
		//	UpdateCaretPos();
		//}

		if (++mBlinkAcc > mBlinkDelay)
		{
			MarkDirty();
			mBlinkAcc = 0;
			mShowingCursor = !mShowingCursor;			
		}		
	}	
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::EnforceMaxPixels()
{
	if (mMaxPixels<=0 && mWidthCheckList.empty()) // no width checking in effect
		return;

	if (mWidthCheckList.empty())
	{
		while (mFont->StringWidth(mString) > mMaxPixels)
			mString = mString.substr(0, mString.length()-1);

		return;
	}
		
	for (WidthCheckList::iterator anItr = mWidthCheckList.begin(); anItr != mWidthCheckList.end(); ++anItr)
	{
		int aWidth = anItr->mWidth;
		if (aWidth<=0)
		{
			aWidth = mMaxPixels;
			if (aWidth<=0)
				continue;
		}

		while (anItr->mFont->StringWidth(mString) > aWidth)
			mString = mString.substr(0,mString.length()-1);
	} 
}

/*!***********************************
// @return    	bool
// @param     	theChar
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool EditWidget::IsPartOfWord(SexyChar theChar)
{
	return (((theChar >= _S('A')) && (theChar <= _S('Z'))) ||
			((theChar >= _S('a')) && (theChar <= _S('z'))) ||
			((theChar >= _S('0')) && (theChar <= _S('9'))) ||
			(((unsigned int)theChar >= (unsigned int)(0x00C0)) && ((unsigned int)theChar <= (unsigned int)(0x00FF))) ||
			(theChar == _S('_')));
}

/*!***********************************
// @return    	void
// @param     	theKey
// @param     	theChar
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::ProcessKey(EKeyboardLayout theKey, SexyChar theChar)
{
	bool shiftDown = (mWidgetManager->mKeyDown[K_VK_L_SHIFT] || mWidgetManager->mKeyDown[K_VK_R_SHIFT]);
	bool controlDown = (mWidgetManager->mKeyDown[K_VK_L_CONTROL] || mWidgetManager->mKeyDown[K_VK_R_CONTROL]);

	if ((theKey == K_VK_L_SHIFT) || (theKey == K_VK_R_SHIFT) || 
		(theKey == K_VK_L_CONTROL) || (theKey == K_VK_R_CONTROL))
		return;

	bool bigChange = false;
	bool removeHilite = !shiftDown;
	
	if (shiftDown && (mHilitePos == -1))
		mHilitePos = mCursorPos;
	
	SexyString anOldString = mString;
	int anOldCursorPos = mCursorPos;
	int anOldHilitePos = mHilitePos;
	if ((theChar == 3) || (theChar == 24))
	{
		// Copy	selection
		if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
		{
			if (mCursorPos < mHilitePos)
				mWidgetManager->mApp->CopyToClipboard(GetDisplayString().substr(mCursorPos, mHilitePos));
			else
				mWidgetManager->mApp->CopyToClipboard(GetDisplayString().substr(mHilitePos, mCursorPos));

			if (theChar == 3)
			{
				removeHilite = false;
			}
			else
			{
				mString = mString.substr(0, K_MIN(mCursorPos, mHilitePos)) + mString.substr(K_MAX(mCursorPos, mHilitePos));
				mCursorPos = K_MIN(mCursorPos, mHilitePos);
				mHilitePos = -1;
				bigChange = true;
			}
		}
	}
	else if (theChar == 22)
	{
		// Paste selection
		SexyString aBaseString = mWidgetManager->mApp->GetClipboard();

		if (aBaseString.length() > 0)
		{
			SexyString aString;

			for (ulong i = 0; i < aBaseString.length(); i++)
			{
				if ((aBaseString[i] == '\r') || (aBaseString[i] == '\n'))
					break;

				if (mFont->CharWidth(aBaseString[i]) != 0 && mEditListener->AllowChar(mId, aBaseString[i]))
					aString += aBaseString[i];
			}

			if (mHilitePos == -1)
			{
				// Insert string where cursor is
				mString = mString.substr(0, mCursorPos) + aString + mString.substr(mCursorPos);
			}
			else
			{
				// Replace selection with new string
				mString = mString.substr(0, K_MIN(mCursorPos, mHilitePos)) + aString + mString.substr(K_MAX(mCursorPos, mHilitePos));
				mCursorPos = K_MIN(mCursorPos, mHilitePos);
				mHilitePos = -1;
			}

			mCursorPos += aString.length();

			bigChange = true;
		}
	}
	else if (theChar == 26)
	{
		// Undo
		
		mLastModifyIdx = -1;
		
		SexyString aSwapString = mString;
		int aSwapCursorPos = mCursorPos;
		int aSwapHilitePos = mHilitePos;			
		
		mString = mUndoString;
		mCursorPos = mUndoCursor;
		mHilitePos = mUndoHilitePos;
					
		mUndoString = aSwapString;
		mUndoCursor = aSwapCursorPos;
		mUndoHilitePos = aSwapHilitePos;			
		
		removeHilite = false;						
	}
	else if (theKey == K_VK_LEFT)
	{
		if (controlDown)
		{
			// Get to a word
			while ((mCursorPos > 0) && (!IsPartOfWord(mString[mCursorPos-1])))
				   mCursorPos--;
			
			// Go beyond the word
			while ((mCursorPos > 0) && (IsPartOfWord(mString[mCursorPos-1])))
				   mCursorPos--;
		}
		else if (shiftDown || (mHilitePos == -1))
			mCursorPos--;
		else
			mCursorPos = K_MIN(mCursorPos, mHilitePos);
	}
	else if (theKey == K_VK_RIGHT)
	{
		if (controlDown)
		{
			// Get to whitespace
			while ((mCursorPos < (int) mString.length()-1) && (IsPartOfWord(mString[mCursorPos+1])))
				   mCursorPos++;
			
			// Go beyond the whitespace
			while ((mCursorPos < (int) mString.length()-1) && (!IsPartOfWord(mString[mCursorPos+1])))
				   mCursorPos++;
		}
		if (shiftDown || (mHilitePos == -1))
			mCursorPos++;
		else
			mCursorPos = K_MAX(mCursorPos, mHilitePos);
	}
	else if (theKey == K_VK_BACK)
	{
		if (mString.length() > 0)
		{
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Delete selection
				mString = mString.substr(0, K_MIN(mCursorPos, mHilitePos)) + mString.substr(K_MAX(mCursorPos, mHilitePos));
				mCursorPos = K_MIN(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Delete char behind cursor
				if (mCursorPos > 0)
					mString = mString.substr(0, mCursorPos-1) + mString.substr(mCursorPos);
				else
					mString = mString.substr(mCursorPos);
				mCursorPos--;
				mHilitePos = -1;
				
				if (mCursorPos != mLastModifyIdx)
					bigChange = true;
				mLastModifyIdx = mCursorPos-1;
			}
		}
	}
	else if (theKey == K_VK_DEL)
	{
		if (mString.length() > 0)
		{
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Delete selection
				mString = mString.substr(0, K_MIN(mCursorPos, mHilitePos)) + mString.substr(K_MAX(mCursorPos, mHilitePos));
				mCursorPos = K_MIN(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Delete char in front of cursor
				if (mCursorPos < (int) mString.length())
					mString = mString.substr(0, mCursorPos) + mString.substr(mCursorPos+1);
				
				if (mCursorPos != mLastModifyIdx)
					bigChange = true;
				mLastModifyIdx = mCursorPos;
			}
		}	
	}
	else if (theKey == K_VK_HOME)
	{
		mCursorPos = 0;	
	}
	else if (theKey == K_VK_END)
	{
		mCursorPos = mString.length();	
	}
	else if (theKey == K_VK_RETURN)
	{
		mEditListener->EditWidgetText(mId, mString);		
	}
	else
	{
		SexyString aString = SexyString(1, theChar);
		unsigned int uTheChar = (unsigned int)theChar;
		unsigned int range = 127;
		//if (gSexyAppBase->mbAllowExtendedChars)
		{
			range = 255;
		}

		if ((uTheChar >= 32) && (uTheChar <= range) && (mFont->StringWidth(aString) > 0) && mEditListener->AllowChar(mId, theChar))
		{				
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Replace selection with new character
				mString = mString.substr(0, K_MIN(mCursorPos, mHilitePos)) + SexyString(1, theChar) + mString.substr(K_MAX(mCursorPos, mHilitePos));
				mCursorPos = K_MIN(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Insert character where cursor is
				mString = mString.substr(0, mCursorPos) + SexyString(1, theChar) + mString.substr(mCursorPos);
				
				if (mCursorPos != mLastModifyIdx+1)
					bigChange = true;						
				mLastModifyIdx = mCursorPos;
				mHilitePos = -1;
			}
											
			mCursorPos++;				
			FocusCursor(false);
		}
		else
			removeHilite = false;
	}
	
	if ((mMaxChars != -1) && ((int) mString.length() > mMaxChars))
		mString = mString.substr(0, mMaxChars);

	EnforceMaxPixels();

	if (mCursorPos < 0)
		mCursorPos = 0;
	else if (mCursorPos > (int) mString.length())
		mCursorPos = mString.length();
	
	if (anOldCursorPos != mCursorPos)
	{
		mBlinkAcc = 0;
		mShowingCursor = true;
	}
	
	FocusCursor(true);
	
	if (removeHilite || mHilitePos==mCursorPos)
		mHilitePos = -1;
	
	if (!mEditListener->AllowText(mId, mString))
	{
		mString = anOldString;
		mCursorPos = anOldCursorPos;
		mHilitePos = anOldHilitePos;
	}
	else if (bigChange)
	{
		mUndoString = anOldString;
		mUndoCursor = anOldCursorPos;
		mUndoHilitePos = anOldHilitePos;
	}
	
	MarkDirty();
}

/*!***********************************
// @return    	void
// @param     	theKey
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::KeyDown(EKeyboardLayout theKey)
{
	if (((theKey < 'A') || (theKey >= 'Z')) && mEditListener->AllowKey(mId, theKey))
		ProcessKey(theKey, 0);

	Widget::KeyDown(theKey);
}

/*!***********************************
// @return    	void
// @param     	theChar
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::KeyChar(SexyChar theChar)
{
	if (mEditListener->AllowChar(mId, theChar))
		ProcessKey(K_VK_ERROR, theChar);

	Widget::KeyChar(theChar);
}

/*!***********************************
// @return    	int
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int EditWidget::GetCharAt(int x, int y)
{
	int aPos = 0;

	SexyString &aString = GetDisplayString();
					
	for (int i = mLeftPos; i < (int) aString.length(); i++)
	{
		SexyString aLoSubStr = aString.substr(mLeftPos, i-mLeftPos);
		SexyString aHiSubStr = aString.substr(mLeftPos, i-mLeftPos+1);
			
		int aLoLen = mFont->StringWidth(aLoSubStr);
		int aHiLen = mFont->StringWidth(aHiSubStr);
		if (x >= (aLoLen+aHiLen)/2 + 5)				
			aPos = i+1;	
	}					
	
	return aPos;
}

/*!***********************************
// @return    	void
// @param     	bigJump
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::FocusCursor(bool bigJump)
{
	while (mCursorPos < mLeftPos)
	{
		if (bigJump)
			mLeftPos = K_MAX(0, mLeftPos-10);
		else
			mLeftPos = K_MAX(0, mLeftPos-1);
		MarkDirty();
	}				
					
	if (mFont != NULL)
	{
		SexyString &aString = GetDisplayString();
		while ((mWidth-8 > 0) && (mFont->StringWidth(aString.substr(0, mCursorPos)) - mFont->StringWidth(aString.substr(0, mLeftPos)) >= mWidth-8))
		{
			if (bigJump)
				mLeftPos = K_MIN(mLeftPos + 10, (int) mString.length()-1);
			else
				mLeftPos = K_MIN(mLeftPos + 1, (int) mString.length()-1);

			MarkDirty();
		}

		if (mFont->StringWidth(aString) < (mWidth-8))
		{
			mLeftPos = 0;
		}

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
void EditWidget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseDown(x, y, theBtnNum, theClickCount);

	mHilitePos = -1;
	mCursorPos = GetCharAt(x, y);
	
	if (theClickCount > 1)
	{
		mHadDoubleClick = true;
		HiliteWord();
	}
	
	MarkDirty();
	
	FocusCursor(false);
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
void EditWidget::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseUp(x,y,theBtnNum,theClickCount);
	if (mHilitePos==mCursorPos)
		mHilitePos = -1;
	
	if (mHadDoubleClick)
	{		
		mHilitePos = -1;
		mCursorPos = GetCharAt(x, y);		

		mHadDoubleClick = false;
		HiliteWord();
	}

	MarkDirty();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::HiliteWord()
{
	SexyString &aString = GetDisplayString();

	if (mCursorPos < (int) aString.length())
	{
		// Find first space before word
		mHilitePos = mCursorPos;
		while ((mHilitePos > 0) && (IsPartOfWord(aString[mHilitePos-1])))
			mHilitePos--;

		// Find first space after word
		while ((mCursorPos < (int) aString.length()-1) && (IsPartOfWord(aString[mCursorPos+1])))
			mCursorPos++;
		if (mCursorPos < (int) aString.length())
			mCursorPos++;
	}
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::MouseDrag(int x, int y)
{
	Widget::MouseDrag(x, y);

	if (mHilitePos == -1)
		mHilitePos = mCursorPos;
	
	mCursorPos = GetCharAt(x, y);
	MarkDirty();
	
	FocusCursor(false);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::MouseEnter()
{
	Widget::MouseEnter();

	mWidgetManager->mApp->SetCursor(SEXY_CURSOR_TEXT);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::MouseLeave()
{	
	Widget::MouseLeave();

	mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void EditWidget::MarkDirty()
{
	if (mColors[COLOR_BKG].mAlpha != 255)
		Widget::MarkDirtyFull();
	else
		Widget::MarkDirty();
}
