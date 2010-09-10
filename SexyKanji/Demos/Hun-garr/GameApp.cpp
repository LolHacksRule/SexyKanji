#include "GameApp.h"

#include "Board.h"
#include "OptionsDialog.h"
#include "Res.h"
#include "TitleScreen.h"

#include "Dialog.h"
#include "MusicManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "WidgetManager.h"

using namespace Sexy;

GameApp::GameApp()
{
	// This is the text that appears in the title bar of the application window
	SetTitle(_S("Hun-garr Needs Planets!"));

	// Set the application width/height in terms of pixels here.
	SetDimensions(640, 480);

	// Enable smooth motion via UpdateF
	EnableVSync();

	mBoard = NULL;
	mTitleScreen = NULL;

	// Because it's annoying to hear the sound of the planets hitting a wall too many
	// times in a second, we'll limit how many can occur.
	mLastPlanetHitSoundTime = -30;
}

GameApp::~GameApp()
{	
}

void GameApp::InitHook()
{
	GetResourceManager()->LoadResourceManifest(); 

	if (!GetResourceManager()->LoadResources(_S("Init")))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	if (!ExtractInitResources(GetResourceManager()))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	// To use custom cursors, you have to specify an image resource
	// to use. The CURSOR_ defines indicate which cursor type you
	// want to assign the image to. 
	// CURSOR_POINTER is the normal Windows pointer.
	// CURSOR_HAND is used when mousing over widgets (if mDoFinger is true).
	// CURSOR_DRAGGING is used when a drag operation is happening.
	// CURSOR_TEXT is the icon used when entering text in an edit box.
	SetCursorImage(SEXY_CURSOR_POINTER, IMAGE_CUSTOM_POINTER);
	SetCursorImage(SEXY_CURSOR_HAND, IMAGE_CUSTOM_HAND);
	SetCursorImage(SEXY_CURSOR_DRAGGING, IMAGE_CUSTOM_DRAGGING);
	SetCursorImage(SEXY_CURSOR_TEXT, IMAGE_CUSTOM_TEXT);
	SetCursor(SEXY_CURSOR_POINTER); // This is important: let the framework know to enforce the cursor.

	if (!GetResourceManager()->LoadResources(_S("TitleScreen")))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	if (!ExtractTitleScreenResources(GetResourceManager()))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	// Now let's create and add our title screen to the widget manager
	mTitleScreen = new TitleScreen(this);
	mTitleScreen->Resize(0, 0, mWidth, mHeight);

	// Let's let the title screen initialize it's widgets and data
	// before adding it to the widget manager:
	mTitleScreen->Init();

	GetWidgetManager()->AddWidget(mTitleScreen);

	// This example just uses 1 standard OGG file instead of a tracker file.
	// Since we aren't cross fading at all, we'll only load it once.
	GetMusicManager()->LoadStream(0, _S("music/hungarr.ogg"));
	GetMusicManager()->PlayStream(0, false, 0.2f);

	mNumLoadingThreadTasks = GetResourceManager()->GetNumResources(_S("Game")) + 
							 GetResourceManager()->GetNumResources(_S("Hungarr"));
}

void GameApp::LoadingThreadProc()
{
	GetResourceManager()->StartLoadResources(_S("Game"));

	while (GetResourceManager()->LoadNextResource())
	{
		mCompletedLoadingThreadTasks++;
		if (mShutdown)
			return;
		
		mTitleScreen->MarkDirty();
	}

	if (GetResourceManager()->HadError() || !ExtractGameResources(GetResourceManager()))
	{		
		GetResourceManager()->ShowResourceError(false);
		mLoadingFailed = true;
	
		return;
	}

	GetResourceManager()->StartLoadResources(_S("Hungarr"));

	while (GetResourceManager()->LoadNextResource())
	{
		mCompletedLoadingThreadTasks++;
		if (mShutdown)
			return;

		mTitleScreen->MarkDirty();
	}

	if (GetResourceManager()->HadError() || !ExtractHungarrResources(GetResourceManager()))
	{		
		GetResourceManager()->ShowResourceError(false);
		mLoadingFailed = true;

		return;
	}
	
}

void GameApp::LoadingThreadCompleted()
{
	// Let the base app class also know that we have completed
	SexyAppBase::LoadingThreadCompleted();

	if (mLoadingFailed)
		return;

	mTitleScreen->LoadingComplete();

	mTitleScreen->MarkDirty();
}

void GameApp::TitleScreenIsFinished()
{
	GetResourceManager()->DeleteResources(_S("TitleScreen"));
	mTitleScreen = NULL;

	mBoard = new Board(this);
	mBoard->Resize(0, 0, mWidth, mHeight);
	GetWidgetManager()->AddWidget(mBoard);
	GetWidgetManager()->SetFocus(mBoard);
}

Dialog* GameApp::NewDialog(int theDialogId, bool isModal, const std::string& theDialogHeader, 
						   const std::string& theDialogLines, const std::string& theDialogFooter, int theButtonMode)
{
	// Rather than dupliate a lengthy explanation, check out the top of DemoDialog.cpp for a complete description
	// of what all the parameters and functions are, in Demo5.
	Dialog* d = new Dialog(IMAGE_DIALOG_BOX, IMAGE_DIALOG_BUTTON, theDialogId, isModal,
		StringToSexyStringFast(theDialogHeader), StringToSexyStringFast(theDialogLines), StringToSexyStringFast(theDialogFooter), theButtonMode);

	d->SetButtonFont(FONT_DEFAULT);
	d->SetLinesFont(FONT_DEFAULT);
	d->SetHeaderFont(FONT_DEFAULT);

	d->SetColor(Dialog::COLOR_HEADER, SexyColor::Black);
	d->SetColor(Dialog::COLOR_LINES, SexyColor::Black);

	d->mSpaceAfterHeader = 30;
	d->Resize(100, 100, 300, 250);

	return d;
}

void GameApp::LostFocus()
{
	SexyAppBase::LostFocus();

	// When losing focus, we should pause the game board
	if (mBoard)
		mBoard->Pause(true);
}

void GameApp::GotFocus()
{
	SexyAppBase::GotFocus();

	if (mBoard)
		mBoard->Pause(false);
}

void GameApp::ButtonPress(int theId)
{
	if (theId == OptionsDialog::MESSAGE_BOX_ID + 2000)
		GetWidgetManager()->KillDialog(theId - 2000);
}

void GameApp::PlaySample(int theSoundNum)
{
	// If playing the sound for when a planet hits a wall, only let it
	// play if it hasn't played the sound for at least 30 update frames
	// (30 hundredths of a second).
	if (theSoundNum == SOUND_PLANET_HIT)
	{
		//if (mUpdateCount - mLastPlanetHitSoundTime > 30)
		{
			//mLastPlanetHitSoundTime = mUpdateCount;
			GetSoundManager()->PlaySample(theSoundNum, 30);
		}
	}
	else
		GetSoundManager()->PlaySample(theSoundNum);
}

void GameApp::ShutdownHook()
{
	if (mBoard != NULL)
		GetWidgetManager()->RemoveWidget(mBoard);

	delete mBoard;

	if (mTitleScreen != NULL)
		GetWidgetManager()->RemoveWidget(mTitleScreen);

	delete mTitleScreen;

	// Using "" deletes ALL resources
	GetResourceManager()->DeleteResources(_S(""));
}

