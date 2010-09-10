// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "Widget.h"
#include "ButtonListener.h"
#include "SharedFont.h"

namespace Sexy
{

class DialogListener;
class ButtonWidget;
class DialogButton;

extern SexyString DIALOG_YES_STRING;
extern SexyString DIALOG_NO_STRING;
extern SexyString DIALOG_OK_STRING;
extern SexyString DIALOG_CANCEL_STRING;

typedef std::vector<std::string> StringVector;

class Dialog : public Widget, public ButtonListener
{
public:
	enum
	{
		BUTTONS_NONE,
		BUTTONS_YES_NO,
		BUTTONS_OK_CANCEL,
		BUTTONS_FOOTER
	};

	enum
	{
		ID_YES		= 1000,
		ID_NO		= 1001,
		ID_OK		= 1000,
		ID_CANCEL	= 1001,
		ID_FOOTER	= 1000
	};
	
	enum
	{
		COLOR_HEADER = 0,
		COLOR_LINES,
		COLOR_FOOTER,
		COLOR_BUTTON_TEXT,
		COLOR_BUTTON_TEXT_HILITE,
		COLOR_BKG,
		COLOR_OUTLINE,		
		NUM_COLORS
	};

	DialogListener*			mDialogListener;
	SexyImage*				mComponentImage;	
	DialogButton*			mYesButton;
	DialogButton*			mNoButton;
	int						mNumButtons;
	
	SexyString				mDialogHeader;
	SexyString				mDialogFooter;
	SexyString				mDialogLines;

	int						mButtonMode;
	SharedFontRef			mHeaderFont;
	SharedFontRef			mLinesFont;	
	int						mTextAlign;
	int						mLineSpacingOffset;
	int						mButtonHeight;
	Insets					mBackgroundInsets;
	Insets					mContentInsets;
	int						mSpaceAfterHeader;
	bool					mDragging;
	int						mDragMouseX;
	int						mDragMouseY;
	int						mId;
	bool					mIsModal;
	int						mResult;	

	int						mButtonHorzSpacing;
	int						mButtonSidePadding;
	

public:
	void					EnsureFonts();

public:
	Dialog(SexyImage* theComponentImage, SexyImage* theButtonComponentImage, 
		int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode); //UNICODE

	virtual ~Dialog();

	virtual void			SetButtonFont(SexyFont* theFont);
	virtual void			SetHeaderFont(SharedFontRef theFont);
	virtual void			SetLinesFont(SharedFontRef theFont);
	virtual void			SetHeaderFont(SexyFont* theFont){mHeaderFont = theFont;}
	virtual void			SetLinesFont(SexyFont* theFont){mLinesFont = theFont;}

	virtual void			SetColor(int theIdx, const SexyColor& theColor);
	virtual int				GetPreferredHeight(int theWidth);

	virtual void			Draw(Graphics* g);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void			OrderInManagerChanged();
	virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

	virtual void			MouseDown(int x, int y, int theClickCount) { Widget::MouseDown(x, y, theClickCount); }
	virtual void			MouseDown(int x, int y, int theBtnNum, int theClickCount);
	virtual void			MouseDrag(int x, int y);
	virtual void			MouseUp(int x, int y) { Widget::MouseUp(x, y); }
	virtual void			MouseUp(int x, int y, int theClickCount) { Widget::MouseUp(x, y, theClickCount); }
	virtual void			MouseUp(int x, int y, int theBtnNum, int theClickCount);
	virtual void			Update();
	virtual void			UpdateF(float theFrac){MarkDirty();}
	virtual	bool			IsModal();
	virtual int				WaitForResult(bool autoKill = true);

	virtual void			ButtonPress(int theId);
	virtual void			ButtonDepress(int theId);
	virtual void			ButtonDownTick(int theId);
};

}

#endif //__DIALOG_H__