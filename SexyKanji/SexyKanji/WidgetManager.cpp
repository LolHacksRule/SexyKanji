// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "WidgetManager.h"

#include "Widget.h"
#include "Graphics.h"
#include "SexyImage.h"
#include "SexyAppBase.h"
#include "PerfTimer.h"
#include "Dialog.h"
#include "Debug.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theApp
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
WidgetManager::WidgetManager(SexyAppBase* theApp)
{
	mApp = theApp;

	mMinDeferredOverlayPriority = 0x7FFFFFFF;
	mWidgetManager = this;	
	mMouseIn = false;
	mDefaultTab = NULL;
	mPopupCommandWidget = NULL;
	mFocusWidget = NULL;
	mLastDownWidget = NULL;
	mOverWidget = NULL;
	mBaseModalWidget = NULL;
	mDefaultBelowModalFlagsMod.mRemoveFlags = WIDGETFLAGS_ALLOW_MOUSE | WIDGETFLAGS_ALLOW_FOCUS;	
	mWidth = 0;
	mHeight = 0;
	mHasFocus = true;
	mUpdateCnt = 0;
	mLastDownButtonId = 0;
	mDownButtons = 0;
	mActualDownButtons = 0;
	mWidgetFlags = WIDGETFLAGS_UPDATE | WIDGETFLAGS_DRAW | WIDGETFLAGS_CLIP |
		WIDGETFLAGS_ALLOW_MOUSE | WIDGETFLAGS_ALLOW_FOCUS;

	for (int i = 0; i < 0xFF; i++)
		mKeyDown[i] = false;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
WidgetManager::~WidgetManager()
{	
	FreeResources();	
}


/*!***********************************
// @return    	Dialog*
// @param     	theDialogId
// @param     	isModal
// @param     	theDialogHeader
// @param     	theDialogLines
// @param     	theDialogFooter
// @param     	theButtonMode
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Dialog* WidgetManager::NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{	
	Dialog* aDialog = new Dialog(NULL, NULL, theDialogId, isModal, theDialogHeader,	theDialogLines, theDialogFooter, theButtonMode);
	//aDialog->mDialogListener = this;
	return aDialog;
}

/*!***********************************
// @return    	Dialog*
// @param     	theDialogId
// @param     	isModal
// @param     	theDialogHeader
// @param     	theDialogLines
// @param     	theDialogFooter
// @param     	theButtonMode
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Dialog* WidgetManager::DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	Dialog* aDialog = NewDialog(theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);		

	AddDialog(theDialogId, aDialog);

	return aDialog;
}


/*!***********************************
// @return    	Dialog*
// @param     	theDialogId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Dialog*	WidgetManager::GetDialog(int theDialogId)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())	
		return anItr->second;

	return NULL;
}

/*!***********************************
// @return    	bool
// @param     	theDialogId
// @param     	removeWidget
// @param     	deleteWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KillDialog(int theDialogId, bool removeWidget, bool deleteWidget)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())
	{
		Dialog* aDialog = anItr->second;

		DialogList::iterator aListItr = std::find(mDialogList.begin(),mDialogList.end(),aDialog);
		if (aListItr != mDialogList.end())
			mDialogList.erase(aListItr);
		
		mDialogMap.erase(anItr);

		if (removeWidget || deleteWidget)
		mWidgetManager->RemoveWidget(aDialog);

		if (aDialog->IsModal())
		{			
			ModalClose();
			mWidgetManager->RemoveBaseModal(aDialog);
		}				

		if (deleteWidget)
			gSexyAppBase->SafeDeleteWidget(aDialog);
		
		return true;
	}

	return false;
}

/*!***********************************
// @return    	bool
// @param     	theDialogId
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KillDialog(int theDialogId)
{
	return KillDialog(theDialogId,true,true);
}

/*!***********************************
// @return    	bool
// @param     	theDialog
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KillDialog(Dialog* theDialog)
{
	return KillDialog(theDialog->mId);
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int WidgetManager::GetDialogCount()
{
	return mDialogMap.size();
}

/*!***********************************
// @return    	void
// @param     	theDialogId
// @param     	theDialog
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::AddDialog(int theDialogId, Dialog* theDialog)
{
    // check if dialog was added
    DialogMap::iterator anItr = mDialogMap.find(theDialogId);
    if (anItr != mDialogMap.end())
    {
        DBG_ASSERT(false);
        return;
    }

	if (theDialog->mWidth == 0)
	{
		// Set the dialog position ourselves
		int aWidth = mWidth/2;
		theDialog->Resize((mWidth - aWidth)/2, mHeight / 5, aWidth, theDialog->GetPreferredHeight(aWidth));
	}
	mDialogMap.insert(DialogMap::value_type(theDialogId, theDialog));
	mDialogList.push_back(theDialog);

	mWidgetManager->AddWidget(theDialog);
	if (theDialog->IsModal())
	{
		mWidgetManager->AddBaseModal(theDialog);
		ModalOpen();
	}
}

/*!***********************************
// @return    	void
// @param     	theDialog
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::AddDialog(Dialog* theDialog)
{
	AddDialog(theDialog->mId, theDialog);
}

//void WidgetManager::DialogButtonPress(int theDialogId, int theButtonId)
//{	
//	if (theButtonId == Dialog::ID_YES)
//		ButtonPress(2000 + theDialogId);
//	else if (theButtonId == Dialog::ID_NO)
//		ButtonPress(3000 + theDialogId);	
//}
//
//void WidgetManager::DialogButtonDepress(int theDialogId, int theButtonId)
//{
//	if (theButtonId == Dialog::ID_YES)
//		ButtonDepress(2000 + theDialogId);
//	else if (theButtonId == Dialog::ID_NO)
//		ButtonDepress(3000 + theDialogId);
//}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::ModalOpen()
{
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::ModalClose()
{
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::FreeResources()
{	
	
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::DisableWidget(Widget* theWidget)
{
	if (mOverWidget == theWidget)
	{
		Widget* aOverWidget = mOverWidget;
		mOverWidget = NULL;
		MouseLeave(aOverWidget);
	}
	
	if (mLastDownWidget	== theWidget)
	{
		Widget* aLastDownWidget = mLastDownWidget;
		mLastDownWidget = NULL;
		DoMouseUps(aLastDownWidget, mDownButtons);
		mDownButtons = 0;		
	}
	
	if (mFocusWidget == theWidget)
	{
		Widget* aFocusWidget = mFocusWidget;
		mFocusWidget = NULL;
		aFocusWidget->LostFocus();
	}
	
	if (mBaseModalWidget == theWidget)
		mBaseModalWidget = NULL;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int WidgetManager::GetWidgetFlags()
{
	return mHasFocus ? mWidgetFlags : GetModFlags(mWidgetFlags, mLostFocusFlagsMod);
}

/*!***********************************
// @return    	Widget*
// @param     	x
// @param     	y
// @param     	theWidgetX
// @param     	theWidgetY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Widget* WidgetManager::GetAnyWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY)
{
	bool found;
	return GetWidgetAtHelper(x, y, GetWidgetFlags(), &found, theWidgetX, theWidgetY);
}

/*!***********************************
// @return    	Widget*
// @param     	x
// @param     	y
// @param     	theWidgetX
// @param     	theWidgetY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Widget* WidgetManager::GetWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY)
{	
	Widget* aWidget = GetAnyWidgetAt(x, y, theWidgetX, theWidgetY);
	if ((aWidget != NULL) && (aWidget->mDisabled))
		aWidget = NULL;
	return aWidget;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::IsLeftButtonDown()
{
	return (mActualDownButtons&1)?true:false;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::IsMiddleButtonDown()
{
	return (mActualDownButtons&4)?true:false;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::IsRightButtonDown()
{
	return (mActualDownButtons&2)?true:false;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::DoMouseUps()
{
	if (mLastDownWidget!=NULL && mDownButtons!=0)
	{
		DoMouseUps(mLastDownWidget, mDownButtons);
		mDownButtons = 0;
		mLastDownWidget = NULL;
	}
}

/*!***********************************
// @return    	void
// @param     	theWidget
// @param     	thePriority
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::DeferOverlay(Widget* theWidget, int thePriority)
{
	mDeferredOverlayWidgets.push_back(std::pair<Widget*, int>(theWidget, thePriority));
	if (thePriority < mMinDeferredOverlayPriority)
		mMinDeferredOverlayPriority = thePriority;
}

/*!***********************************
// @return    	void
// @param     	theMaxPriority
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::FlushDeferredOverlayWidgets(int theMaxPriority)
{
	for (;;)
	{
		int aNextMinPriority = 0x7FFFFFFF;

		for (int i = 0; i < (int) mDeferredOverlayWidgets.size(); i++)
		{
			Widget* aWidget = mDeferredOverlayWidgets[i].first;			
			if (aWidget != NULL)
			{
				int aPriority = mDeferredOverlayWidgets[i].second;

				if (aPriority == mMinDeferredOverlayPriority)
				{
					// Overlays don't get clipped
					mCurG->PushState();
					mCurG->Translate(-mMouseDestRect.mX, -mMouseDestRect.mY);
					mCurG->Translate(aWidget->mX, aWidget->mY);
					mCurG->SetLinearBlend(true);

					aWidget->DrawOverlay(mCurG, aPriority);
					mDeferredOverlayWidgets[i].first = NULL;
					mCurG->PopState();
				}
				else
				{
					if (aPriority < aNextMinPriority)
						aNextMinPriority = aPriority;
				}
			}
		}

		mMinDeferredOverlayPriority = aNextMinPriority;
		if (aNextMinPriority == 0x7FFFFFFF)
		{
			// No more widgets lined up for overlays, clear our vector
			mDeferredOverlayWidgets.resize(0);
			break;
		}

		// Lowest overlay priority is higher or equal to our current widget, 
		//  so continue deferring
		if (aNextMinPriority >= theMaxPriority)
			break;
	}
}

/*!***********************************
// @return    	void
// @param     	theWidget
// @param     	theDownCode
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::DoMouseUps(Widget* theWidget, ulong theDownCode)
{
	int aClickCountTable[3] = { 1,-1, 3 };
	for (int i = 0; i < 3; i++)
	{
		if ((theDownCode & (1 << i)) != 0)
		{
			theWidget->mIsDown = false;
			theWidget->MouseUp(mLastMouseX - theWidget->mX, mLastMouseY - theWidget->mY, aClickCountTable[i]);
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::RemapMouse(int& theX, int& theY)
{
	theX = ( theX - mMouseSourceRect.mX ) * mMouseDestRect.mWidth / mMouseSourceRect.mWidth + mMouseDestRect.mX;
	theY = ( theY - mMouseSourceRect.mY ) * mMouseDestRect.mHeight / mMouseSourceRect.mHeight + mMouseDestRect.mY;
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::MouseEnter(Widget* theWidget)
{
	theWidget->mIsOver = true;
	
	theWidget->MouseEnter();
	if (theWidget->mDoFinger)
		theWidget->ShowFinger(true);	
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::MouseLeave(Widget* theWidget)
{
	theWidget->mIsOver = false;
	
	theWidget->MouseLeave();
	if (theWidget->mDoFinger)
		theWidget->ShowFinger(false);
}

/*!***********************************
// @return    	void
// @param     	theWidget
// @param     	theBelowFlagsMod
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::SetBaseModal(Widget* theWidget, const FlagsMod& theBelowFlagsMod)
{
	mBaseModalWidget = theWidget;
	mBelowModalFlagsMod = theBelowFlagsMod;
	
	if ((mOverWidget != NULL) && (mBelowModalFlagsMod.mRemoveFlags & WIDGETFLAGS_ALLOW_MOUSE) && 
		(IsBelow(mOverWidget, mBaseModalWidget)))
	{
		Widget* aWidget = mOverWidget;
		mOverWidget = NULL;
		MouseLeave(aWidget);		
	}
	
	if ((mLastDownWidget != NULL) && (mBelowModalFlagsMod.mRemoveFlags & WIDGETFLAGS_ALLOW_MOUSE) && 
		(IsBelow(mLastDownWidget, mBaseModalWidget)))
	{
		Widget* aWidget = mLastDownWidget;
		int aDownButtons = mDownButtons;
		mDownButtons = 0;		
		mLastDownWidget = NULL;
		DoMouseUps(aWidget, aDownButtons);		
	}
	
	if ((mFocusWidget != NULL) && (mBelowModalFlagsMod.mRemoveFlags & WIDGETFLAGS_ALLOW_FOCUS) && 
		(IsBelow(mFocusWidget, mBaseModalWidget)))
	{
		Widget* aWidget = mFocusWidget;
		mFocusWidget = NULL;
		aWidget->LostFocus();		
	}
}

/*!***********************************
// @return    	void
// @param     	theWidget
// @param     	theBelowFlagsMod
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::AddBaseModal(Widget* theWidget, const FlagsMod& theBelowFlagsMod)
{
	PreModalInfo aPreModalInfo;
	aPreModalInfo.mBaseModalWidget = theWidget;
	aPreModalInfo.mPrevBaseModalWidget = mBaseModalWidget;
	aPreModalInfo.mPrevFocusWidget = mFocusWidget;
	aPreModalInfo.mPrevBelowModalFlagsMod = mBelowModalFlagsMod;
	mPreModalInfoList.push_back(aPreModalInfo);

	SetBaseModal(theWidget, theBelowFlagsMod);
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::AddBaseModal(Widget* theWidget)
{
	AddBaseModal(theWidget, mDefaultBelowModalFlagsMod);
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::RemoveBaseModal(Widget* theWidget)
{
	DBG_ASSERT(mPreModalInfoList.size() > 0);
	
	bool first = true;
	
	while (mPreModalInfoList.size() > 0)
	{
		PreModalInfo* aPreModalInfo = &mPreModalInfoList.back();
		
		if ((first) && (aPreModalInfo->mBaseModalWidget != theWidget))
		{
			// We don't remove it yet, because we want to restore
			//  its keyboard focused widget and crap later
			return;
		}
		
		// If we removed a widget's self from pre-modal info before
		//  then that means the dialog got removed out-of-order but we
		//  deferred setting the state back until now
		bool done = (aPreModalInfo->mPrevBaseModalWidget != NULL) ||
			(mPreModalInfoList.size() == 1);

		SetBaseModal(aPreModalInfo->mPrevBaseModalWidget, 
			aPreModalInfo->mPrevBelowModalFlagsMod);

		if (mFocusWidget == NULL)
		{
			mFocusWidget = aPreModalInfo->mPrevFocusWidget;
			if (mFocusWidget != NULL)
				mFocusWidget->GotFocus();
		}

		mPreModalInfoList.pop_back();		

		if (done)
			break;

		first = false;
	}
}

/*!***********************************
// @return    	void
// @param     	theMouseDestRect
// @param     	theMouseSourceRect
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::Resize(const SexyRect& theMouseDestRect, const SexyRect& theMouseSourceRect)
{	
	mWidth = theMouseDestRect.mWidth + 2 * theMouseDestRect.mX;
	mHeight = theMouseDestRect.mHeight + 2 * theMouseDestRect.mY;
	mMouseDestRect = theMouseDestRect;
	mMouseSourceRect = theMouseSourceRect;
}

/*!***********************************
// @return    	void
// @param     	aWidget
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::SetFocus(Widget* aWidget)
{	
	if (aWidget==mFocusWidget)
		return;

	if (mFocusWidget != NULL)
		mFocusWidget->LostFocus();

	if ((aWidget != NULL) && (aWidget->mWidgetManager == this))
	{
		mFocusWidget = aWidget;
		
		if ((mHasFocus) && (mFocusWidget != NULL))
			mFocusWidget->GotFocus();
	}
	else
		mFocusWidget = NULL;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::GotFocus()
{
	if (!mHasFocus)
	{
		mHasFocus = true;
	
		if (mFocusWidget != NULL)
			mFocusWidget->GotFocus();
	}
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::LostFocus()
{
	if (mHasFocus)
	{
		mActualDownButtons = 0;
		for (int aKeyNum = 0; aKeyNum < 0xFF; aKeyNum++)
		{
			if (mKeyDown[aKeyNum])
				KeyUp((EKeyboardLayout) aKeyNum);
		}

		mHasFocus = false;
	 
		if (mFocusWidget != NULL)
			mFocusWidget->LostFocus();
	}
}

/*!***********************************
// @return    	void
// @param     	theModalFlags
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::InitModalFlags(ModalFlags* theModalFlags)
{
	theModalFlags->mIsOver = mBaseModalWidget == NULL;
	theModalFlags->mOverFlags = GetWidgetFlags();
	theModalFlags->mUnderFlags = GetModFlags(theModalFlags->mOverFlags, mBelowModalFlagsMod);
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::DrawWidgetsTo(Graphics* g)
{
	mCurG = g;

	ModalFlags aModalFlags;
	InitModalFlags(&aModalFlags);

	WidgetList::iterator anItr = mWidgets.begin();
	while (anItr != mWidgets.end())
	{
		Widget* aWidget = *anItr;
		
		if (aWidget->mVisible)
		{
			g->PushState();
			g->SetLinearBlend(true);
			g->Translate(aWidget->mX, aWidget->mY);
			aWidget->DrawAll(&aModalFlags, g);
			g->PopState();
		}

		++anItr;
	}

	mCurG = NULL;
}

/*!***********************************
// @return    	bool
// @param     	theGraphics
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::DrawScreen(Sexy::Graphics* theGraphics)
{
	SEXY_AUTO_PERF("WidgetManager::DrawScreen");

	ModalFlags aModalFlags;
	InitModalFlags(&aModalFlags);

	bool drewStuff = false;	
	
	int aDirtyCount = 0;
	bool hasTransients = false;
	bool hasDirtyTransients = false;

	// Survey
	WidgetList::iterator anItr = mWidgets.begin();
	while (anItr != mWidgets.end())
	{
		Widget* aWidget = *anItr;
		if (aWidget->mDirty)
			aDirtyCount++;
		++anItr;
	}
	
	mMinDeferredOverlayPriority = 0x7FFFFFFF;
	mDeferredOverlayWidgets.resize(0);

	Graphics *aScrG = theGraphics;
	GraphicsAutoState anAutoPush(theGraphics);
	mCurG = aScrG;

	if (aDirtyCount > 0)
	{
		Graphics* g = aScrG;
		GraphicsAutoState anAutoState(g);

		g->Translate(-mMouseDestRect.mX, -mMouseDestRect.mY);
		bool is3D = true; //mApp->Is3DAccelerated();

		WidgetList::iterator anItr = mWidgets.begin();
		while (anItr != mWidgets.end())
		{
			Widget* aWidget = *anItr;
			
			if (aWidget == mWidgetManager->mBaseModalWidget)
				aModalFlags.mIsOver = true;

			if ((aWidget->mDirty) && (aWidget->mVisible))
			{
				GraphicsAutoState autoClip(g);
				g->SetLinearBlend(is3D);
				g->Translate(aWidget->mX, aWidget->mY);				
				aWidget->DrawAll(&aModalFlags, g);
				 
				aDirtyCount++;
				drewStuff = true;
				aWidget->mDirty = false;
			}

			++anItr;
		}
	}
	
	FlushDeferredOverlayWidgets(0x7FFFFFFF);

	mCurG = NULL;

	return drewStuff;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::UpdateFrame()
{
	SEXY_AUTO_PERF("WidgetManager::UpdateFrame");

	ModalFlags aModalFlags;
	InitModalFlags(&aModalFlags);

	// Keep us from having mLastWMUpdateCount interfere with our own updating
	mUpdateCnt++;
	mLastWMUpdateCount = mUpdateCnt;	
	UpdateAll(&aModalFlags);

	return mDirty;
}

/*!***********************************
// @return    	bool
// @param     	theFrac
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::UpdateFrameF(float theFrac)
{
	SEXY_AUTO_PERF("WidgetManager::UpdateFrame");

	ModalFlags aModalFlags;
	InitModalFlags(&aModalFlags);

	UpdateFAll(&aModalFlags, theFrac);
	return mDirty;
}

/*!***********************************
// @return    	void
// @param     	theList
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::SetPopupCommandWidget(Widget* theList)
{
	mPopupCommandWidget = theList;
	AddWidget(mPopupCommandWidget);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::RemovePopupCommandWidget()
{
	if (mPopupCommandWidget != NULL)
	{
		Widget *aWidget = mPopupCommandWidget;
		mPopupCommandWidget = NULL;	
		RemoveWidget(aWidget);
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
void WidgetManager::MousePosition(int x, int y)
{

	int aLastMouseX = mLastMouseX;
	int aLastMouseY = mLastMouseY;

	mLastMouseX = x;
	mLastMouseY = y;
	
	int aWidgetX;
	int aWidgetY;
	Widget* aWidget = GetWidgetAt(x, y, &aWidgetX, &aWidgetY);
	
	if (aWidget != mOverWidget)
	{
		Widget* aLastOverWidget = mOverWidget;
		mOverWidget = NULL;

		if (aLastOverWidget != NULL)
			MouseLeave(aLastOverWidget);
		
		mOverWidget = aWidget;
		if (aWidget != NULL)
		{
			MouseEnter(aWidget);
			aWidget->MouseMove(aWidgetX, aWidgetY);
		}
	}
	else if ((aLastMouseX != x) || (aLastMouseY != y))
	{
		if (aWidget != NULL)
			aWidget->MouseMove(aWidgetX, aWidgetY);		
	}
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::RehupMouse()
{
	if (mLastDownWidget != NULL)
	{
		if (mOverWidget != NULL)
		{						
			Widget* aWidgetOver = GetWidgetAt(mLastMouseX, mLastMouseY, NULL, NULL);

			if (aWidgetOver != mLastDownWidget)
			{								
				Widget* anOverWidget = mOverWidget;
				mOverWidget = NULL;	
				MouseLeave(anOverWidget);
			}
		}
	}
	else if (mMouseIn) 
		MousePosition(mLastMouseX, mLastMouseY);
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::MouseUp(int x, int y, int theClickCount)
{	
	mLastInputUpdateCnt = mUpdateCnt;
	
	int aMask;
	
	if (theClickCount < 0)
		aMask = 0x02;
	else if (theClickCount == 3)
		aMask = 0x04;
	else
		aMask = 0x01;

	// Make sure that we thought this button was down anyway - possibly not, if we 
	//  disabled the widget already or something
	mActualDownButtons &= ~aMask;
	if ((mLastDownWidget != NULL) && ((mDownButtons & aMask) != 0))
	{
		Widget* aLastDownWidget = mLastDownWidget;

		mDownButtons &= ~aMask;		
		if (mDownButtons == 0)
			mLastDownWidget = NULL;

		aLastDownWidget->mIsDown = false;
		aLastDownWidget->MouseUp(x - aLastDownWidget->mX, y - aLastDownWidget->mY, theClickCount);
	}	
	else
		mDownButtons &= ~aMask;		

	MousePosition(x, y);
	
	return true;
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::MouseDown(int x, int y, int theClickCount) 
{	
	mLastInputUpdateCnt = mUpdateCnt;

	if (theClickCount < 0)
		mActualDownButtons |= 0x02; // right
	else if (theClickCount == 3)
		mActualDownButtons |= 0x04; // middle
	else
		mActualDownButtons |= 0x01; // left

	MousePosition(x, y);

	if ((mPopupCommandWidget != NULL) && (!mPopupCommandWidget->Contains(x, y)))
		RemovePopupCommandWidget();

	int aWidgetX;
	int aWidgetY;
	Widget* aWidget = GetWidgetAt(x, y, &aWidgetX, &aWidgetY);	

	// Begin mouse down options
/*	
	// Option 1
	//This code sets a new widget as the mouse drag focus widget and lets the old 
	//mousedownwidget think the buttons popped up.
	if ((mLastDownWidget != NULL) && (mLastDownWidget != aWidget))
	{
		DoMouseUps(mLastDownWidget, mDownButtons);
		mDownButtons = 0;
	}
*/
	// Option 2
	// This code passes all button downs to the mLastDownWidget 
	if (mLastDownWidget != NULL)
		aWidget = mLastDownWidget;

	// End mouse down options

	if (theClickCount < 0)
	{
		mLastDownButtonId = -1;
		mDownButtons |= 0x02;
	}
	else if (theClickCount == 3)
	{
		mLastDownButtonId = 2;
		mDownButtons |= 0x04;
	}
	else
	{
		mLastDownButtonId = 1;
		mDownButtons |= 0x01;
	}
	
	mLastDownWidget = aWidget;
	if (aWidget != NULL)
	{
		if (aWidget->WantsFocus())
			SetFocus(aWidget);
		
		aWidget->mIsDown = true;
		aWidget->MouseDown(aWidgetX, aWidgetY, theClickCount);
	}
	
	return true;
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::MouseMove(int x, int y) 
{	
	mLastInputUpdateCnt = mUpdateCnt;

	if (mDownButtons)
		return MouseDrag(x,y);

	mMouseIn = true;
	MousePosition(x, y);	
			
	return true;
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::MouseDrag(int x, int y) 
{	
	mLastInputUpdateCnt = mUpdateCnt;

	mMouseIn = true;
	mLastMouseX = x;
	mLastMouseY = y;

	if ((mOverWidget != NULL) && (mOverWidget != mLastDownWidget))
	{
		Widget* anOverWidget = mOverWidget;
		mOverWidget = NULL;	
		MouseLeave(anOverWidget);		
	}

	if (mLastDownWidget != NULL)
	{
		SexyPoint anAbsPos = mLastDownWidget->GetAbsPos();

		int aWidgetX = x - anAbsPos.mX;
		int aWidgetY = y - anAbsPos.mY;		
		mLastDownWidget->MouseDrag(aWidgetX, aWidgetY);		
		
		Widget* aWidgetOver = GetWidgetAt(x, y, NULL, NULL);

		if ((aWidgetOver == mLastDownWidget) && (aWidgetOver != NULL))
		{
			if (mOverWidget == NULL)
			{
				mOverWidget = mLastDownWidget;
				MouseEnter(mOverWidget);
			}
		}
		else
		{
			if (mOverWidget != NULL)
			{
				Widget* anOverWidget = mOverWidget;
				mOverWidget = NULL;	
				MouseLeave(anOverWidget);				
			}
		}
	}
	
	return true;	
}

/*!***********************************
// @return    	bool
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::MouseExit(int x, int y)
{	
	mLastInputUpdateCnt = mUpdateCnt;

	mMouseIn = false;

	if (mOverWidget != NULL)
	{
		MouseLeave(mOverWidget);
		mOverWidget = NULL;
	}
	
	return true;
}

/*!***********************************
// @return    	void
// @param     	theDelta
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void WidgetManager::MouseWheel(int theDelta)
{
	mLastInputUpdateCnt = mUpdateCnt;

	if (mFocusWidget != NULL)
	{
		mFocusWidget->MouseWheel(theDelta);
	}
	// So a mouse over a list widget will scroll the widget.
	else if (mOverWidget != NULL)
	{
		mOverWidget->MouseWheel(theDelta);
	}
}

/*!***********************************
// @return    	bool
// @param     	theChar
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KeyChar(SexyChar theChar)
{
	mLastInputUpdateCnt = mUpdateCnt;

	if (theChar == K_VK_TAB)
	{
		//TODO: Check thing

		if (mKeyDown[K_VK_R_CONTROL] || mKeyDown[K_VK_L_CONTROL])
		{
			if (mDefaultTab != NULL)
				mDefaultTab->KeyChar(theChar);
			
			return true;
		}
	}

	if (mFocusWidget != NULL)
		mFocusWidget->KeyChar(theChar);
	
	return true;
}

/*!***********************************
// @return    	bool
// @param     	key
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KeyDown(EKeyboardLayout key)
{
	mLastInputUpdateCnt = mUpdateCnt;

	if ((key >= 0) && (key < 0xFF))
		mKeyDown[key] = true;

	if (mFocusWidget != NULL)
		mFocusWidget->KeyDown(key);
	
	return true;
}

/*!***********************************
// @return    	bool
// @param     	key
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool WidgetManager::KeyUp(EKeyboardLayout key)
{
	mLastInputUpdateCnt = mUpdateCnt;

	if ((key >= 0) && (key < 0xFF))
		mKeyDown[key] = false;

	if ((key == K_VK_TAB) && (mKeyDown[K_VK_R_CONTROL] || mKeyDown[K_VK_L_CONTROL]))
		return true;	

	if (mFocusWidget != NULL)
		mFocusWidget->KeyUp(key);
	
	return true;
}
