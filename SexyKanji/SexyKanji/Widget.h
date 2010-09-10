// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Common.h"
#include "SexyColor.h"
#include "Insets.h"
#include "Graphics.h"
#include "WidgetContainer.h"
#include "KKeyTable.h"

namespace Sexy
{

class WidgetManager;

typedef std::vector<SexyColor> ColorVector;

class Widget : public WidgetContainer
{
public:			
	bool					mVisible;
	bool					mMouseVisible;		
	bool					mDisabled;
	bool					mHasFocus;	
	bool					mIsDown;
	bool					mIsOver;
	bool					mHasTransparencies;	
	ColorVector				mColors;
	Insets					mMouseInsets;
	bool					mDoFinger;
	bool					mWantsFocus;

	Widget*					mTabPrev;
	Widget*					mTabNext;	

	void					WidgetRemovedHelper();

public:
	Widget();
	virtual ~Widget();
		
	virtual void			OrderInManagerChanged();
	virtual void			SetVisible(bool isVisible);
	
	virtual void			SetColors(int theColors[][3], int theNumColors);
	virtual void			SetColors(int theColors[][4], int theNumColors);
	virtual void			SetColor(int theIdx, const SexyColor& theColor);
	virtual const SexyColor&	GetColor(int theIdx);
	virtual SexyColor			GetColor(int theIdx, const SexyColor& theDefaultColor);	
		
	virtual void			SetDisabled(bool isDisabled);
	virtual void			ShowFinger(bool on);
	
	virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void			Resize(const SexyRect& theRect);
	virtual void			Move(int theNewX, int theNewY);	
	virtual bool			WantsFocus();
	virtual void			Draw(Graphics* g); // Already translated
	virtual void			DrawOverlay(Graphics* g);
	virtual void			DrawOverlay(Graphics* g, int thePriority);
	virtual void			Update();
	virtual void			UpdateF(float theFrac);
	virtual void			GotFocus();
	virtual void			LostFocus();	
	virtual void			KeyChar(SexyChar theChar);
	virtual void			KeyDown(EKeyboardLayout theKey);
	virtual void			KeyUp(EKeyboardLayout theKey);	
	virtual void			MouseEnter();
	virtual void			MouseLeave();
	virtual void			MouseMove(int x, int y);
	virtual void			MouseDown(int x, int y, int theClickCount);
	virtual void			MouseDown(int x, int y, int theBtnNum, int theClickCount);
	virtual void			MouseUp(int x, int y);
	virtual void			MouseUp(int x, int y, int theClickCount);
	virtual void			MouseUp(int x, int y, int theBtnNum, int theClickCount);
	virtual void			MouseDrag(int x, int y);
	virtual void			MouseWheel(int theDelta);
	virtual bool			IsPointVisible(int x, int y);
	
	//////// Helper functions
	
	virtual int				GetNumDigits(int theNumber);
	virtual bool			Contains(int theX, int theY);
	virtual SexyRect			GetInsetRect();	
	void					DeferOverlay(int thePriority = 0);	

	//////// Layout functions
	int						Left()							{ return mX; } 
	int						Top()							{ return mY; }
	int						Right()							{ return mX + mWidth; }
	int						Bottom()						{ return mY + mHeight; }
	int						Width()							{ return mWidth; }
	int						Height()						{ return mHeight; }

	void					Layout(int theLayoutFlags, Widget *theRelativeWidget, int theLeftPad = 0, int theTopPad = 0, int theWidthPad = 0, int theHeightPad = 0);
};

/////// Layout flags used in Widget::Layout method
enum LayoutFlags
{
	LAY_SameWidth		=		0x0001,
	LAY_SameHeight		=		0x0002,

	LAY_SetLeft			=		0x0010,
	LAY_SetTop			=		0x0020,
	LAY_SetWidth		=		0x0040,
	LAY_SetHeight		=		0x0080,

	LAY_Above			=		0x0100,
	LAY_Below			=		0x0200,
	LAY_Right			=		0x0400,
	LAY_Left			=		0x0800,

	LAY_SameLeft		=		0x1000,
	LAY_SameRight		=		0x2000,
	LAY_SameTop			=		0x4000,
	LAY_SameBottom		=		0x8000,

	LAY_GrowToRight		=		0x10000,
	LAY_GrowToLeft		=		0x20000,
	LAY_GrowToTop		=		0x40000,
	LAY_GrowToBottom	=		0x80000,

	LAY_HCenter			=		0x100000,
	LAY_VCenter			=		0x200000,
	LAY_Max				=		0x400000,

	LAY_SameSize		=		LAY_SameWidth | LAY_SameHeight,
	LAY_SameCorner		=		LAY_SameLeft  | LAY_SameTop,
	LAY_SetPos			=		LAY_SetLeft   | LAY_SetTop,
	LAY_SetSize			=		LAY_SetWidth  | LAY_SetHeight
};


}

#endif //__WIDGET_H__
