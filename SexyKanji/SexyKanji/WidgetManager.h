// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __WIDGETMANAGER_H__
#define __WIDGETMANAGER_H__

#include "Common.h"
#include "WidgetContainer.h"
#include "KKeyTable.h"
//#include "DialogListener.h"
//#include "ButtonListener.h"

namespace Sexy
{
class Widget;
class SexyImage;
class SexyAppBase;
class Graphics;
class Dialog;

typedef std::list<Widget*> WidgetList;

enum
{
	WIDGETFLAGS_UPDATE			= 1,
	WIDGETFLAGS_MARK_DIRTY		= 2,
	WIDGETFLAGS_DRAW			= 4,
	WIDGETFLAGS_CLIP			= 8,	
	WIDGETFLAGS_ALLOW_MOUSE		= 16,
	WIDGETFLAGS_ALLOW_FOCUS		= 32,	
};

class PreModalInfo
{
public:
	Widget*					mBaseModalWidget;
	Widget*					mPrevBaseModalWidget;
	Widget*					mPrevFocusWidget;
	FlagsMod				mPrevBelowModalFlagsMod;
};

typedef std::list<PreModalInfo> PreModalInfoList;
typedef std::vector<std::pair<Widget*, int> > DeferredOverlayVector;
typedef std::map<int, Dialog*> DialogMap;
typedef std::list<Dialog*> DialogList;

class WidgetManager : public WidgetContainer//, public DialogListener, public ButtonListener
{
public:	
	Widget*					mDefaultTab;
	
	Graphics*				mCurG;
	SexyAppBase*			mApp;
	Widget*					mPopupCommandWidget;	
	DeferredOverlayVector	mDeferredOverlayWidgets;
	int						mMinDeferredOverlayPriority;
	
	bool					mHasFocus;
	Widget*					mFocusWidget;
	Widget*					mLastDownWidget;
	Widget*					mOverWidget;
	Widget*					mBaseModalWidget;
	FlagsMod				mLostFocusFlagsMod;
	FlagsMod				mBelowModalFlagsMod;
	FlagsMod				mDefaultBelowModalFlagsMod;
	PreModalInfoList		mPreModalInfoList;
	SexyRect				mMouseDestRect;
	SexyRect				mMouseSourceRect;
	bool					mMouseIn;
	int						mLastMouseX;
	int						mLastMouseY;
	int						mDownButtons;
	int						mActualDownButtons;
	int						mLastInputUpdateCnt;
	
	bool					mKeyDown[0xFF];
	int						mLastDownButtonId;	
	
	int						mWidgetFlags;

protected:
	int						GetWidgetFlags();
	void					MouseEnter(Widget* theWidget);
	void					MouseLeave(Widget* theWidget);

	DialogMap				mDialogMap;
	DialogList				mDialogList;

protected:
	void					SetBaseModal(Widget* theWidget, const FlagsMod& theBelowFlagsMod);

public:
	WidgetManager(SexyAppBase* theApplet);
	virtual ~WidgetManager();
	

	Dialog*					NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);		
	Dialog*					DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);
	Dialog*					GetDialog(int theDialogId);
	void					AddDialog(int theDialogId, Dialog* theDialog);
	void					AddDialog(Dialog* theDialog);
	bool					KillDialog(int theDialogId, bool removeWidget, bool deleteWidget);
	bool					KillDialog(int theDialogId);
	bool					KillDialog(Dialog* theDialog);
	int						GetDialogCount();
	//void					DialogButtonPress(int theDialogId, int theButtonId);
	//void					DialogButtonDepress(int theDialogId, int theButtonId);
	void					ModalOpen();
	void					ModalClose();	


	void					FreeResources();		
	void					AddBaseModal(Widget* theWidget, const FlagsMod& theBelowFlagsMod);
	void					AddBaseModal(Widget* theWidget);
	void					RemoveBaseModal(Widget* theWidget);
	void					Resize(const SexyRect& theMouseDestRect, const SexyRect& theMouseSourceRect);
	void					DisableWidget(Widget* theWidget);	
	Widget*					GetAnyWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY);
	Widget*					GetWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY);
	void					SetFocus(Widget* aWidget);
	void					GotFocus();	
	void					LostFocus();	
	void					InitModalFlags(ModalFlags* theModalFlags);
	void					DrawWidgetsTo(Graphics* g);
	void					DoMouseUps(Widget* theWidget, ulong theDownCode);	
	void					DoMouseUps();
	void					DeferOverlay(Widget* theWidget, int thePriority);
	void					FlushDeferredOverlayWidgets(int theMaxPriority);
	
	bool					DrawScreen(Sexy::Graphics* theGraphics);
	bool					UpdateFrame();				
	bool					UpdateFrameF(float theFrac);
	void					SetPopupCommandWidget(Widget* theList);
	void					RemovePopupCommandWidget();	
	void					MousePosition(int x, int y);	
	void					RehupMouse();
	void					RemapMouse(int& theX, int& theY);
	bool					MouseUp(int x, int y, int theClickCount);
	bool					MouseDown(int x, int y, int theClickCount);
	bool					MouseMove(int x, int y);
	bool					MouseDrag(int x, int y);
	bool					MouseExit(int x, int y);
	void					MouseWheel(int theDelta);
	bool					KeyChar(SexyChar theChar);
	bool					KeyDown(EKeyboardLayout key);
	bool					KeyUp(EKeyboardLayout key);

	bool					IsLeftButtonDown();
	bool					IsMiddleButtonDown();
	bool					IsRightButtonDown();
};

}

#endif // __WIDGETMANAGER_H__
