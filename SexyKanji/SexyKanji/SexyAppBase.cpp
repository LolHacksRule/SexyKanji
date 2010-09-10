// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyAppBase.h"

#include "SexyConfig.h"
#include "Graphics.h"

#include "SoundManager.h"
#include "MusicManager.h"
#include "ImageManager.h"
#include "FontManager.h"
#include "WidgetManager.h"
#include "ResourceManager.h"

#include "Widget.h"

#include "SexyProperties.h"
#include "SexyPropertiesParser.h"
#include "SexyPropertiesWriter.h"

#include "XMLParser.h"

#include "KSysDebug.h"
#include "KText.h"
#include "KTrueText.h"

#include "KSysThread.h"

#include "Common.h"

#include "SexyImage.h"
#include "SexyFont.h"

#include "PerfTimer.h"

#include <time.h>

#ifdef K_WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif // WIN32

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// \brief		Global Pointer to Access Main Application derived from SexyAppBase
// \details 	<b>Description:</b> Use this pointer anywhere in the program when
//				you need access to the SexyAppBase derrived class.  Once SexyAppBase
//				or the SexyAppBase derived class is created, this pointer points to that
//				object.
//************************************/
SexyAppBase* Sexy::gSexyAppBase = NULL;


void SEHCatcher (bool bIsAssertion,
				 unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
				 const char *lpszAssertionFileName, long nAssertionLine,
				 long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[]);

/*!***********************************
// \brief		The initial charater string that the TTF Fonts are initialized to Render.
// \details 	<b>Description:</b> We need to find someway to dynamically load this string 
//				from a file to override this default definition.  The TTF Fonts are initialized
//				to render the characters in this string.  It's important that the characters match
//				the Game Render Texts.
//************************************/
std::wstring gUnicodeDefaultInitStr = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

//HotSpot: 15 10
//Size: 32 32
unsigned char gDraggingCursorData[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0, 
	0x01, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 
	0xe0, 0x00, 0xff, 0xfe, 0x60, 0x00, 0xff, 0xfc, 0x20, 0x00, 0xff, 0xfc, 0x00, 0x00, 0xff, 
	0xfe, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 
	0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xf0, 
	0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x80, 0x00, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 
	0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 
	0x01, 0x8d, 0xb6, 0x00, 0x01, 0xcf, 0xfe, 0x00, 0x00, 0xef, 0xfe, 0x00, 0x00, 0xff, 0xfe, 
	0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 
	0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 
	0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00
};

SexyImage* gScreenGraphic = NULL;
#ifdef K_IPHONE
bool g_buffered_rendering = false;
#else
bool g_buffered_rendering = true;
#endif

SexyFont* gDebugFont = NULL;
Graphics* gDebugGraphic = NULL;
SexyFPSTimer gDrawFPSTimer(_S("DrawFPS"));
SexyFPSTimer gDirtyFPSTimer(_S("DirtyFPS"));
SexyFPSTimer gUpdateFPSTimer(_S("Update  FPS"));
SexyFPSTimer gUpdateFFPSTimer(_S("UpdateF FPS"));


/*!***********************************
// @return    	
//
// \brief		The Main Application Default Ctor
// \details 	<b>Description:</b> Default CTor to init class variables.
//************************************/
SexyAppBase::SexyAppBase() 
{
	gSexyAppBase = this;
	mLoadingThread = NULL;
	mKWindow = NULL;
	mIsWindowed = DEFAULT_WINDOWED;
	mCompanyName = DEFAULT_COMPANY;
	mTitle = DEFAULT_TITLE;
	mVersion = DEFAULT_VERSION;
	mWidth = DEFAULT_WIDTH;
	mHeight = DEFAULT_HEIGHT;
	mFullscreenBits = DEFAULT_BITS;
	mGraphics = NULL;
	mClock = 0;
	mLastClockTick = 0;
	mAverageClockTickDelta = 0;
	mClockTickCount = 0;
	mUpdateAppDepth = 0;
	mShutdown = false;
	mAutoStartLoadingThread = true;
	mRunning = false;
	mYieldMainThread = false;
	mLoadingThreadStarted = false;
	mLoadingThreadCompleted = false;
	mNumLoadingThreadTasks = 0;
	mCompletedLoadingThreadTasks = 0;
	mLoadingFailed = false;
	mMusicVolume = DEFAULT_MUSIC_VOLUME;
	mSfxVolume = DEFAULT_SOUND_VOLUME;
	mMuteCount = 0;	
	mAutoMuteCount = 0;
	mVSyncBroken = false;
	mVSyncEnabled = true;
	mLastDrawWasEmpty = false;
	mPaused = false;
	mVSyncBrokenTestUpdates = 0;
	mLoaded = false;
	mSoundManager = NULL;
	mMusicManager = NULL;
	mImageManager = NULL;
	mFontManager = NULL;
	mWidgetManager = NULL;
	mProperties = NULL;
	mSettings = NULL;
	mResourceManager= NULL;
	mCurrentCursor = SEXY_CURSOR_POINTER;
	mCursorMap.resize(NUM_SEXY_CURSORS, NULL);
	mCustomCursorsEnabled = true;

	mDebugKeysEnabled = false;
#ifdef _DEBUG
	mDebugKeysEnabled = true;
#endif
	mShowFPS = false;
	mShowProgramStats = 0;

#ifdef K_WIN32
	mDraggingCursor = NULL;
#endif
}

/*!***********************************
// \brief		DTor
// \details 	<b>Description:</b> Destructor
//************************************/
SexyAppBase::~SexyAppBase()
{
	delete mKWindow;
	mKWindow = NULL;

	delete mGraphics;
	mGraphics = NULL;

	delete gScreenGraphic;
	gScreenGraphic = NULL;

	delete gDebugFont;
	gDebugFont = NULL;

	delete gDebugGraphic;
	gDebugGraphic = NULL;

	delete mResourceManager;
	mResourceManager = NULL;
	delete mFontManager;
	mFontManager = NULL;

	delete mImageManager;
	mImageManager = NULL;
	delete mSoundManager;
	mSoundManager = NULL;
	delete mMusicManager;
	mMusicManager = NULL;
	delete mWidgetManager;
	mWidgetManager = NULL;

	delete mLoadingThread;
	mLoadingThread = NULL;

	delete mProperties;
	mProperties = NULL;
	delete mSettings;
	mSettings = NULL;

#ifdef K_WIN32
	if(mDraggingCursor != NULL)
		DestroyCursor(mDraggingCursor);
	mDraggingCursor = NULL;
#endif
}

/*!***********************************
// @return    	bool
//
// \brief		Attempt to create the Game Window and load Engine settings.
// \details 	<b>Description:</b> This method will attempt to load the engine settings and create a game window.
/				If it fails, then this method will return false. Don't overried this method unless you are absolutely
//				sure you know what you are doing.  Override InitHook() instead.
// \warning		<b>If this Method Returns False, DO NOT START UP!.</b>  Instead of overriding this method, override 
//				InitHook() instead.
//************************************/
bool SexyAppBase::Init()
{
	// UUID Generated at: http://www.famkruithof.net/uuid/uuidgen
	if(KPTK::checkGameInstance(StrFormat("%s-%s",SexyStringToStringFast(mTitle).c_str(), "88d106b0-1fc6-11df").c_str()))
		return false; // Don't Start Up!

	/* Read data files from application bundle (for Mac) */
	KMiscTools::setFileSource(true);
#ifdef K_WIN32
	_chdir( KMiscTools::makeFilePath( "" ) );
#else
	chdir( KMiscTools::makeFilePath( "" ) );
#endif

	StartAppLog();
	ReadFromRegistry();
	LoadProperties();
	
#ifndef K_IPHONE
	std::queue<Erenderer> aRenderDeviceQueue;
	aRenderDeviceQueue.push(K_DIRECTX9);
	aRenderDeviceQueue.push(K_DIRECTX);
	aRenderDeviceQueue.push(K_OPENGL); // This is the only one to work on Mac and iDevices (JPOAG)

	/* Create game window */
	while(mKWindow == NULL && !aRenderDeviceQueue.empty())
	{
		mKWindow = KPTK::createKWindow(aRenderDeviceQueue.front());
		mCurrentRendererType = aRenderDeviceQueue.front();
		aRenderDeviceQueue.pop();

		if (mKWindow != NULL)
		{
			mKWindow->setFixedAspectRatio(true); /* Keep 4:3 ratio when going fullscreen */
			if(!mKWindow->createGameWindow(mWidth, mHeight, mFullscreenBits, mIsWindowed, SexyStringToStringFast(mTitle).c_str()))
			{
				delete mKWindow;
				mKWindow = NULL;
			}
		}
	}
#else
	mKWindow = KPTK::createKWindow (K_OPENGL);
	if (!mKWindow->createGameWindow (mWidth, mHeight, 32, true, SexyStringToStringFast(mTitle).c_str(), K_LANDSCAPE))
	{
		delete mKWindow;
		mKWindow = NULL;
	}
	mKWindow->enableMultiTouch (true);
#endif

	if(mKWindow == NULL)
	{
		K_LOG("Sexy:SexyAppBase::Init :Error: Creating Render Device!");
		Shutdown();
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// This makes sure the icon is loaded properly to display in the window title 
	// bar and the task switcher
	/////////////////////////////////////////////////////////////////(JPOAG)//
#if defined(WIN32) || defined (WIN64)
	HICON anIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(K_APPICON));
	HRESULT aResult = SendMessage((HWND)mKWindow->getWindowHandle(), WM_SETICON, (LPARAM)ICON_SMALL, (WPARAM)anIcon);
	aResult = SendMessage((HWND)mKWindow->getWindowHandle(), WM_SETICON, (LPARAM)ICON_BIG, (WPARAM)anIcon);

	//////////////////////////////////////////////////////////////////////////
	// This is so custom cursors work
	//////////////////////////////////////////////////////////////////////////
	SetClassLong((HWND)mKWindow->getWindowHandle(), GCL_HCURSOR, NULL);
	SetCursor(SEXY_CURSOR_POINTER);

	mDraggingCursor = CreateCursor(GetModuleHandle(NULL), 15, 10, 32, 32, gDraggingCursorData, gDraggingCursorData+sizeof(gDraggingCursorData)/2);
#endif

	mKWindow->setMaxFrameRate(0); // enable VSync Drawing
	mKWindow->setPTKCallBack(EventHandler);

#ifdef K_WIN32
    // set windows event handler
    mKWindow->setCallBack(WindowsEventHandler);
#endif

	//////////////////////////////////////////////////////////////////////////
	// init Drawing Target
	//////////////////////////////////////////////////////////////////////////
	if (g_buffered_rendering)
	{
		gScreenGraphic = new SexyImage();
		g_buffered_rendering = gScreenGraphic->GetKanjiImage()->makeRenderTarget(mWidth, mHeight, true);

		if(!g_buffered_rendering)
		{
			delete gScreenGraphic;
			gScreenGraphic = NULL;
		}
		else
			gScreenGraphic->GetKanjiImage()->clearRenderTarget();
	}

	/* Use software mouse cursor */
// 	KInput::hidePointer();
// 	mKWindow->displayMouse(true);

	mResourceManager	= new ResourceManager();
	mImageManager		= new ImageManager();
	mFontManager		= new FontManager();
	mMusicManager		= new MusicManager();
	mSoundManager		= new SoundManager();
	mGraphics			= new Graphics(gScreenGraphic);

	mWidgetManager		= new WidgetManager(this);

	// Set volumes (and mute)
	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);

	SexyRect aRect = SexyRect(0, 0, mKWindow->getWindowWidth(), mKWindow->getWindowHeight());
	//mKWindow->getActualWindowWidth();
	//mKWindow->getActualWindowHeight();

	mWidgetManager->Resize(aRect, aRect);

	//////////////////////////////////////////////////////////////////////////
	// Tell Fonts to use Unicode
	//////////////////////////////////////////////////////////////////////////
	KText::initializeAsUnicode(K_WCHAR);
	const char* aTable[1];
	aTable[0] = (char*)gUnicodeDefaultInitStr.c_str();
	KTrueText::initializeAsUnicode(1, (aTable), K_WCHAR);

	InitHook();

	return true;
}

/*!***********************************
// @return    	void
//
// \brief		Main Loop of the GameApp
// \details 	<b>Description:</b> Main Loop of the GameApp.  Only run this if Init() returns true.
//************************************/
void SexyAppBase::Start()
{
	if (mShutdown)
		return;

	KSysDebug::catchExceptions(SEHCatcher);

	if (mAutoStartLoadingThread)
		StartLoadingThread();

	int aSleepCount = 0;
	unsigned long aLastTimeCheck = 0;
	unsigned long aVSyncBrokenTestStartTick = 0;
	int aVSyncBrokenCount = 0;
	double anUpdateFrameTimeAccum = 0.0f;
	int aNonDrawCount = 0;
	bool bHasPendingDraw = true;
	double aPendingUpdatesAccum = 0;

	mRunning = true;

	int anUpdateAppState = UPDATESTATE_MESSAGES;

	// DO MAIN LOOP
	while (!mShutdown && mKWindow!= NULL && !mKWindow->isQuit())
	{
		if (anUpdateAppState == UPDATESTATE_PROCESS_DONE)
			anUpdateAppState = UPDATESTATE_MESSAGES;

		if (mLoadingFailed)
			Shutdown();

		// Used in mYieldMainThread Calculations
		unsigned long aStartTime = GetClock();	
		int aCumSleepTime = 0;

		bool didUpdate = false;
		mUpdateAppDepth++;

		bool isVSynched = (mVSyncEnabled) && (!mLastDrawWasEmpty) && (!mVSyncBroken);
		double aFrameFTime = 10.0f; // 100 Hz
		double anUpdatesPerUpdateF = 1.0f;
		int aSyncRefreshRate = (mKWindow->getDesktopDisplayFrequency()) ? mKWindow->getDesktopDisplayFrequency() : 60;

		if (mVSyncEnabled)
		{
			aFrameFTime = (1000.0 / aSyncRefreshRate);
			anUpdatesPerUpdateF = (float) (1000.0 / (10.0f * aSyncRefreshRate));
		}

		// This looks a little scary, but it works like this:
		// The update is done in phases.  After each phase of the update
		// we break away and make sure we haven't received a QUIT message
		switch (anUpdateAppState)
		{
		case UPDATESTATE_MESSAGES: // Phase 1: process messages from OS
			{
				// In Sexy, all Messages are sent to a deferred queue to 
				// wait for processing
				mKWindow->processEvents();

				// The deferred queue is processed one message at a time:
				// in case one message is quit, which then we immediately stop.
 				if (!ProcessDeferredMessages()) // returns false when queue is empty
					anUpdateAppState = UPDATESTATE_PROCESS_1;
				break;
			}
		case UPDATESTATE_PROCESS_1: // Phase II: DoUpdateFrames() at 100Hz
			{
				if(mLoadingFailed)
					break;

				if (!mPaused && !mShutdown)
				{
					//////////////////////////////////////////////////////////////////////////
					// DoUpdateFrames the Frame Time Accumulator
					//////////////////////////////////////////////////////////////////(JPOAG)/
					if (!isVSynched) // When we are VSynching, only calculate this FTimeAcc right after drawing
					{
						unsigned long aCurTime = GetClock();

						if (aLastTimeCheck != 0)
						{				
							int aDeltaTime = aCurTime - aLastTimeCheck;		
							anUpdateFrameTimeAccum = (anUpdateFrameTimeAccum + aDeltaTime < 200.0) ? anUpdateFrameTimeAccum + aDeltaTime : 200.0f;	// Min
							anUpdateFrameTimeAccum = (anUpdateFrameTimeAccum > 0.0f) ? anUpdateFrameTimeAccum : 0.0f; // Max

						}

						aLastTimeCheck = aCurTime;
					}

					//////////////////////////////////////////////////////////////////////////
					// Check to see if it's time to do an update
					//////////////////////////////////////////////////////////////////(JPOAG)/
					if ((++aNonDrawCount < 10) || (!mLoaded))
					{
						bool doUpdate = false;

						if (isVSynched)// Synch'ed to vertical refresh, so update as soon as possible after draw
							doUpdate = (!bHasPendingDraw) || (anUpdateFrameTimeAccum >= (int) (aFrameFTime * 0.75));
						else if (anUpdateFrameTimeAccum >= aFrameFTime)
							doUpdate = true;

						if (doUpdate)
						{
							// Do VSyncBroken test.  This test fails if we're in fullscreen and
							// "don't vsync" has been forced in Advanced settings up Display Properties
							{
								mVSyncBrokenTestUpdates++;
								if (mVSyncBrokenTestUpdates >= (unsigned long) ((1000+10-1)/10))
								{
									// It has to be running 33% fast to be "broken" (25% = 1/0.800)
									if (aStartTime - aVSyncBrokenTestStartTick <= 800)
									{
										// The test has to fail 3 times in a row before we decide that
										//  vsync is broken overall
										aVSyncBrokenCount++;
										if (aVSyncBrokenCount >= 3)
											mVSyncBroken = true;
									}
									else
										aVSyncBrokenCount = 0;

									aVSyncBrokenTestStartTick = aStartTime;
									mVSyncBrokenTestUpdates = 0;
								}
							}

							DoUpdateFrames();

							anUpdateAppState = UPDATESTATE_PROCESS_2;					

							bHasPendingDraw = true;
							didUpdate = true;
						}
					}


					//////////////////////////////////////////////////////////////////////////
					// Draw
					//////////////////////////////////////////////////////////////////////////
					if (!didUpdate && !mShutdown)
					{			
						anUpdateAppState = UPDATESTATE_PROCESS_DONE;

						aNonDrawCount = 0;

						if (bHasPendingDraw && !mShutdown)
						{
							unsigned long aDrawStart = GetClock();
							DrawDirtyStuff(mGraphics);
							unsigned long aPreScreenBltTime = GetClock();

							_Render(aLastTimeCheck, anUpdateFrameTimeAccum);


							bHasPendingDraw = false;
							mLastDrawWasEmpty = false;
						}
						else
						{
							// Let us take into account the time it took to draw dirty stuff			
							int aTimeToNextFrame = (int) (aFrameFTime - anUpdateFrameTimeAccum);
							if (aTimeToNextFrame > 0)
							{
								// Wait till next processing cycle
								++aSleepCount;
								KMiscTools::pause(aTimeToNextFrame);

								aCumSleepTime += aTimeToNextFrame;					
							}
						}
					}					
				}
				break;
			}
		case UPDATESTATE_PROCESS_2: // Phase C: UpdateF() at Monitor Refresh Rate
			{
				anUpdateAppState = UPDATESTATE_PROCESS_DONE;

				aPendingUpdatesAccum += anUpdatesPerUpdateF;
				aPendingUpdatesAccum -= 1.0;

				// Process any extra updates
				// This is to keep DoUpdateFrames() running at 100Hz while the rest of the
				// loop is running at the Monitor Refresh Rate. (JPOAG)
				while (aPendingUpdatesAccum >= 1.0 && !mShutdown)
				{	
					++aNonDrawCount;
					DoUpdateFrames();
					
					aPendingUpdatesAccum -= 1.0;
				}					

				if(mShutdown)break;

				DoUpdateFramesF((float)anUpdatesPerUpdateF);

				// Don't let mUpdateFTimeAcc dip below 0
				//  Subtract an extra 0.2ms, because sometimes refresh rates have some
				//  fractional component that gets truncated, and it's better to take off
				//  too much to keep our timing tending toward occuring right after 
				//  redraws
				if (isVSynched)
					anUpdateFrameTimeAccum = (anUpdateFrameTimeAccum - aFrameFTime - 0.2f > 0.0) ? anUpdateFrameTimeAccum - aFrameFTime - 0.2f : 0.0;
				else
					anUpdateFrameTimeAccum -= aFrameFTime;

				break;
			}
		case UPDATESTATE_PROCESS_DONE:
		default:
				anUpdateAppState = UPDATESTATE_MESSAGES;
		}

		mUpdateAppDepth--;
		ProcessSafeDeleteList();
		if(GetFontManager())mFontManager->CleanSharedFonts();
		if(GetImageManager())mImageManager->CleanSharedImages();

		if (mYieldMainThread && !mShutdown)
		{
			// This is to make sure that the title screen doesn't take up any more than 
			// 1/3 of the processor time

			unsigned long anEndTime = GetClock();
			int anElapsedTime = (anEndTime - aStartTime) - aCumSleepTime;
			int aLoadingYieldSleepTime = K_MIN(250, (anElapsedTime * 2) - aCumSleepTime);

			if (aLoadingYieldSleepTime >= 0)
				KMiscTools::pause(aLoadingYieldSleepTime);
		}
	}

 	ProcessSafeDeleteList();

	mRunning = false;

 	WaitForLoadingThread();

 	char aString[256];
 	sprintf(aString, "Seconds       = %g\r\n", GetClock() / 1000.0);
 	K_LOG(aString);

//	WriteToRegistry();

	ShutdownHook();
	ProcessSafeDeleteList();

	//////////////////////////////////////////////////////////////////////////
	// IMPORTANT: delete ResourceManager first, it uses Sound/MusicManager
	// to release samples/streams
	/////////////////////////////////////////////////////////////////(HeikoF)/
	delete mResourceManager;
	mResourceManager = NULL;

	// delete Font manager before image manager, fonts use images!
	if(GetFontManager())mFontManager->CleanSharedFonts(true);
	delete mFontManager;
	mFontManager = NULL;

	if(GetImageManager())mImageManager->CleanSharedImages(true);
	delete mImageManager;
	mImageManager = NULL;

	if (GetSoundManager())mSoundManager->ReleaseSamples();
	delete mSoundManager;
	mSoundManager = NULL;

	if (GetMusicManager())mMusicManager->ReleaseStreams();
	delete mMusicManager;
	mMusicManager = NULL;

	WriteToRegistry();

	delete mProperties;
	mProperties = NULL;
	delete mSettings;
	mSettings = NULL;

	if(mKWindow) mKWindow->terminate();
}

/*!***********************************
// @return    	void
//
// \brief		Starts the Logging of the Engine to the default log file.
// \details 	<b>Description:</b> Creates a log in "%APPDATA_COMMON%\Game\Log.txt"
//************************************/
void SexyAppBase::StartAppLog()
{
	std::string title = SexyStringToStringFast(mTitle);
	std::string company = SexyStringToStringFast(mCompanyName);
	std::string version = SexyStringToStringFast(mVersion);

	static char szLogPath[K_MAXPATH];

	K_snprintf (szLogPath, K_MAXPATH - 1, "%slog.txt", SexyStringToStringFast(GetAppDataFolder()).c_str());
	szLogPath[K_MAXPATH - 1] = 0;

	KPTK::enableLog  (szLogPath, title.c_str(), version.c_str(), false);
}

/*!***********************************
// @return    	void
//
// \brief		Override this to specify things that need to be done before Start()
// \details 	<b>Description:</b> This is a good place to Load Resources() and default
//				settings as well as load the minimum set of assets needed to display the loading screen.
//************************************/
void SexyAppBase::InitHook()
{
}

/*!***********************************
// @return    	void
//
// \brief		Runs at ~100Hz
// \details 	<b>Description:</b> Runs at ~100Hz.  You do not have to call this, this is called automatically
//				from Start()
//************************************/
void SexyAppBase::DoUpdateFrames()
{
	if ((mLoadingThreadCompleted) && (!mLoaded))
	{
		mLoaded = true;
		mYieldMainThread = false;
		LoadingThreadCompleted();

		delete mLoadingThread;
		mLoadingThread = NULL;
	}
	
	if(HasFocus())KSound::resume(); // Potential bug
	mSoundManager->Update();
	mMusicManager->Update();
	mWidgetManager->UpdateFrame();

	gUpdateFPSTimer.OnEvent();
}

/*!***********************************
// @return    	void
// @param     	theFrac
//
// \brief		IF(VSync On) THEN Update @ Monitor Refresh Rate.  ELSE refresh @ 100 Hz
// \details 	<b>Description:</b> IF(VSync On) THEN Update @ Monitor Refresh Rate.  ELSE refresh @ 100 Hz END.
//************************************/
void Sexy::SexyAppBase::DoUpdateFramesF( float theFrac )
{
	//if (mVSyncEnabled) // && (!mMinimized))
	mWidgetManager->UpdateFrameF(theFrac); // (JSullivan)
	gUpdateFFPSTimer.OnEvent();
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		Draws the Widget hierarchy to the back buffer.
// \details 	<b>Description:</b> You do not need to call this.  This is automatically called
//				by Start().
//************************************/
void SexyAppBase::DrawDirtyStuff(Graphics *g)
{
	mKWindow->setWorldView(0, 0, 0.0f, 1.0f, false);

// 	if(g_buffered_rendering)
// 		mWidgetManager->MarkDirtyFull();

	g->PreDraw();

	if(!mShutdown && mWidgetManager->DrawScreen(g)) gDirtyFPSTimer.OnEvent();
}

/*!***********************************
// @return    	bool
// @param     	theKEvent
//
// \brief		The Kanji Event Handler.  Passes events to Sexy.
// \details 	<b>Description:</b> This is required to receive system events in a platform
//				agnostic manner. For Sexy, we stuff the events into a queue and process them
//				when we are certain to avoid crashes.
//************************************/
bool SexyAppBase::EventHandler(KEvent *theKEvent)
{
	if(gSexyAppBase != NULL && theKEvent && theKEvent->type != K_EVENT_NULL)
		gSexyAppBase->PushKanjiEvent(theKEvent);

	//////////////////////////////////////////////////////////////////////////
	// Always return true:  If a Quit Event is posted, Sexy will catch it 
	// via Kanji.  Other events are handled natively by Kanji, like the
	// ScreenSaver event or the Monitor Powering off.
	///////////////////////////////////////////////////////////////////(JPoag)/
	return true;
}


#ifdef K_WIN32
/*!***********************************
// @return    	bool
// @param     	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
//
// \brief		The Windows Event Handler.
//************************************/
bool SexyAppBase::WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // use accelerator table created in KWindow::createGameWindow
        case WM_COMMAND:
            if(HIWORD(wParam) == 1 && LOWORD(wParam) == WM_USER)
                gSexyAppBase->ToggleWindowed();
            break;

        // for case when we don't have accelerator table
        case WM_SYSKEYDOWN:
            if(LOWORD(wParam) == VK_RETURN)
                gSexyAppBase->ToggleWindowed();
            break;
    }

    // continue to manage all other messages
    return true;
}
#endif

/*!***********************************
// @return    	void
//
// \brief		Event Triggered when Game Window Receives Focus.
// \details 	<b>Description:</b> Event Triggered when Game Window Receives Focus. By default,
//				Turns the sound back on.
//************************************/
void SexyAppBase::GotFocus()
{
	/* Window got focus; resume audio */
	// delegated to the sound manager (HEIKOF)
	mSoundManager->ResumeAll();

	// RehupFocus(); : See SexyAppFramework 1.33 (JPOAG)
}

/*!***********************************
// @return    	void
//
// \brief		Even Triggered when the Game Window Loses Focus.
// \details 	<b>Description:</b> Even Triggered when the Game Window Loses Focus. By default,
//				turns sound off.
//************************************/
void SexyAppBase::LostFocus()
{
	/* Window lost focus; suspend audio */
	// delegated to the sound manager (HEIKOF)
	mSoundManager->SuspendAll();
}

/*!***********************************
// @return    	void
//
// \brief		Debug Method to draw FPS timers to screen.
// \details 	<b>Description:</b> This method is called directly before the backbuffer is
//				presented to render FPS debug info.  This only works when Debug mode is on and
//				if you press 'F3'
//************************************/
void SexyAppBase::DrawFPS()
{
	//////////////////////////////////////////////////////////////////////////
	// Draw Debug Info
	//////////////////////////////////////////////////////////////////////////
	if (mShowFPS && gDebugFont != NULL && gDebugGraphic != NULL)
	{
		SexyString anFPS = gDrawFPSTimer.GetFPS() + gDirtyFPSTimer.GetFPS() 
			+ _S("\n") + gUpdateFPSTimer.GetFPS() + _S("\n") + gUpdateFFPSTimer.GetFPS();

		int aHeight = gDebugFont->GetLineSpacing() * 4;
		int aWidth = gDebugFont->StringWidth(gUpdateFFPSTimer.GetFPS() + _S("   "));

		gDebugGraphic->SetColor(SexyColor::Black);
		gDebugGraphic->FillRect(SexyRect(mWidth - aWidth, mHeight - aHeight, aWidth, aHeight));
		gDebugGraphic->SetColor(SexyColor::White);
		gDebugGraphic->SetFont(gDebugFont);
		gDebugGraphic->WriteWordWrapped(SexyRect(mWidth - aWidth, mHeight - aHeight, aWidth, aHeight), anFPS);
	}
}
/*!***********************************
// @return    	void
// @param     	aLastTimeCheck
// @param     	anUpdateFrameTimeAccum
//
// \brief		Internal Presentation Routine.
// \details 	<b>Description:</b> Called internally, do not modify or override.
//************************************/
void SexyAppBase::_Render( unsigned long &aLastTimeCheck, double &anUpdateFrameTimeAccum )
{
	if(g_buffered_rendering)
	{
		mGraphics->Finalize(); // stop RTT drawing
		gScreenGraphic->GetKanjiImage()->endRenderToTarget(); // in case we weren't the last graphics Render Target or if the screen lost focus
		gScreenGraphic->GetKanjiImage()->setTextureQuality(true);
		gScreenGraphic->GetKanjiImage()->allowTextureWrap(K_WRAP_MIRROR);
		gScreenGraphic->GetKanjiImage()->blitRect(0,0,(float)mWidth,(float)mHeight,0,0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Blit Custom Mouse Image
	//////////////////////////////////////////////////////////////////////////
	if(mKWindow->canDisplayMouse() && mCurrentCursor != SEXY_CURSOR_NONE && mCursorMap[mCurrentCursor] != NULL && mCustomCursorsEnabled)
	{
		if (mCursorMap[mCurrentCursor]->GetKanjiImage())
		{
			float aCursorWidth = mCursorMap[mCurrentCursor]->GetKanjiImage()->getWidth();
			float aCursorHeight = mCursorMap[mCurrentCursor]->GetKanjiImage()->getHeight();

			// For Now, the cursor image hotspot is in the center of the image.
			mCursorMap[mCurrentCursor]->GetKanjiImage()->blitAlphaRect (0, 0, aCursorWidth, aCursorHeight, 
				short(KInput::getMouseX () - 1 - aCursorWidth/2.0f), short(KInput::getMouseY () - 1 - aCursorHeight/2.0f), 
				false, false);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Debug Drawing performed Directly to the BackBuffer
	//////////////////////////////////////////////////////////////////////////
	DrawFPS();
	DrawStats();
	//////////////////////////////////////////////////////////////////////////

	/* Flip buffers, present the frame we've just drawn */
	mKWindow->flipBackBuffer(true); gDrawFPSTimer.OnEvent();

	unsigned long aPostScreenBltTime = GetClock();

	if(g_buffered_rendering)
/*		gScreenGraphic->GetKanjiImage()->beginRenderToTarget();*/
		mGraphics->PreDraw();

	//////////////////////////////////////////////////////////////////////////
	// DoUpdateFrames the Frame Time Accumulator
	//////////////////////////////////////////////////////////////////(JPOAG)/
	// if (isVSynched) then this is the only update
	{
		unsigned long aCurTime = GetClock();

		if (aLastTimeCheck != 0)
		{				
			int aDeltaTime = aCurTime - aLastTimeCheck;		
			anUpdateFrameTimeAccum = (anUpdateFrameTimeAccum + aDeltaTime < 200.0) ? anUpdateFrameTimeAccum + aDeltaTime : 200.0f;	// Min
			anUpdateFrameTimeAccum = (anUpdateFrameTimeAccum > 0.0f) ? anUpdateFrameTimeAccum : 0.0f; // Max

		}

		aLastTimeCheck = aCurTime;
	}
}
/*!***********************************
// @return    	bool
// @param     	bSingleMessage Optionally process 1 message at a time.
//
// \brief		Processed the Message Queue backlog.
// \details 	<b>Description:</b> Processed the Message Queue backlog. Sexy queues system messages and 
//				processes them during a specific part of the update cycle.  This is to avoid asynchronous
//				issues that could potentially crash the game. (like a widget getting deleted while it is drawing)
//************************************/
bool Sexy::SexyAppBase::ProcessDeferredMessages( bool bSingleMessage /*= false*/ )
{
	if(mDeferredEventQueue.empty())
		return false;

	//////////////////////////////////////////////////////////////////////////
	// bSingleMessage is broken 
	///////////////////////////////////////////////////////////////////(JPoag)/
	while (!mDeferredEventQueue.empty()/* && !bSingleMessage*/)
	{
		KEvent anEvent = mDeferredEventQueue.front();
		mDeferredEventQueue.pop();

		// This is provided for ease of use.
		int theMouseX = anEvent.mouseX;
		int theMouseY = anEvent.mouseY;
		wchar_t aCharacter = (wchar_t)anEvent.unicodeChar;

		switch (anEvent.type)
		{
		case K_EVENT_ACTIVATED :
			if (!mIsWindowed)
			{
				mWidgetManager->MarkAllDirty();
			}
			GotFocus();
			break;

		case K_EVENT_DEACTIVATED :
			LostFocus();
			break;
		case K_EVENT_MOUSEMOVE:
			mWidgetManager->MouseMove(theMouseX, theMouseY);
			break;

		case K_EVENT_MOUSEDOWN:
			switch (anEvent.buttonIndex)
			{
			case K_LBUTTON :
				mWidgetManager->MouseDown(theMouseX, theMouseY, MOUSE_BUTTON_LEFT);
				break;
			case K_RBUTTON :
				mWidgetManager->MouseDown(theMouseX, theMouseY, MOUSE_BUTTON_RIGHT);
				break;
			case K_MBUTTON :
				mWidgetManager->MouseDown(theMouseX, theMouseY, MOUSE_BUTTON_CENTER);
				break;
			}
			break;

		case K_EVENT_DBLCLICK:
			switch (anEvent.buttonIndex)
			{
			case K_LBUTTON :
				mWidgetManager->MouseDown(theMouseX, theMouseY, MOUSE_DBLCLICK_LEFT);
				break;
			case K_RBUTTON :
				mWidgetManager->MouseDown(theMouseX, theMouseY, MOUSE_DBLCLICK_RIGHT);
				break;
			}
			break;

		case K_EVENT_MOUSEUP:
			switch (anEvent.buttonIndex)
			{
			case K_LBUTTON :
				mWidgetManager->MouseUp(theMouseX, theMouseY, MOUSE_BUTTON_LEFT);
				break;
			case K_RBUTTON :
				mWidgetManager->MouseUp(theMouseX, theMouseY, MOUSE_BUTTON_RIGHT);
				break;
			case K_MBUTTON :
				mWidgetManager->MouseUp(theMouseX, theMouseY, MOUSE_BUTTON_CENTER);
				break;
			}
			break;

		case K_EVENT_KEYDOWN:
			if(!DebugKeyDown(anEvent.ptkKey))
			{
				if(anEvent.ptkKey == K_VK_RETURN && (KInput::isPressed(K_VK_R_CONTROL) || KInput::isPressed(K_VK_L_CONTROL)))
					ToggleWindowed();
				if(anEvent.ptkKey == K_VK_D && (KInput::isPressed(K_VK_R_CONTROL) || KInput::isPressed(K_VK_L_CONTROL)))
					mDebugKeysEnabled = !mDebugKeysEnabled;
				else
					mWidgetManager->KeyDown(anEvent.ptkKey);
			}
			break;
		case K_EVENT_KEYUP:
			mWidgetManager->KeyUp(anEvent.ptkKey);
			break;

		case K_EVENT_WHEEL:
			mWidgetManager->MouseWheel(anEvent.wheel/120);
			break;

		case K_EVENT_CONTEXTMENU:
			break;
		case K_EVENT_UNICHAR:
			mWidgetManager->KeyChar((SexyChar)aCharacter);
			break;
		case K_EVENT_TOUCH: // iPhone/iPad
			break;
		case K_EVENT_ACCEL: // iPhone/iPad
			break;
		case K_EVENT_LOW_MEMORY: // iPhone/iPad
			break;
		case K_EVENT_ORIENTATION: // iPhone/iPad
			break;
		default :
			break;
		}
	}

	return true;
}

/*!***********************************
// @return    	void
// @param     	theKEvent
//
// \brief		Allows deferred processing of events
// \details 	<b>Description:</b> Allows deferred processing of events.  Pushes the event into a queue.
//************************************/
void Sexy::SexyAppBase::PushKanjiEvent( KEvent* theKEvent )
{
	if (theKEvent != NULL)
	{
		switch (theKEvent->type)
		{
			case K_EVENT_MOUSEMOVE:
			case K_EVENT_MOUSEUP:
			case K_EVENT_MOUSEDOWN:
		//	case K_EVENT_CHAR:  // Ignored because we process K_EVENT_UNICHAR and we use Unicode Strings (JPOAG)
			case K_EVENT_KEYDOWN:
			case K_EVENT_KEYUP:
			case K_EVENT_DBLCLICK:
			case K_EVENT_WHEEL:
			case K_EVENT_CONTEXTMENU:
			case K_EVENT_ACTIVATED:
			case K_EVENT_DEACTIVATED:
			case K_EVENT_UNICHAR:
			case K_EVENT_TOUCH: // iPhone/iPad
			case K_EVENT_ACCEL: // iPhone/iPad
			case K_EVENT_LOW_MEMORY: // iPhone/iPad
			case K_EVENT_ORIENTATION: // iPhone/iPad
				mDeferredEventQueue.push(*theKEvent);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Kanji can ProcessEvents() at any time (to keep the window responsive 
	// the the OS).  If the MainLoop 'Start()' state machine isn't in the
	// UPDATESTATE_MESSAGES state, then very bad things can occur.
	///////////////////////////////////////////////////////////////////(JPoag)/
}

/*!***********************************
// @return    	unsigned long
//
// \brief		Returns the number of milliseconds since the program began.
// \details 	<b>Description:</b> Returns the number of milliseconds since the program began.
//************************************/
unsigned long Sexy::SexyAppBase::GetClock()
{
	static const int kMaxClockTickDelta  = 250;

	// First Run.
	if (mLastClockTick == 0)
	{
		mLastClockTick = KMiscTools::getMilliseconds();
		return mClock;
	}

	unsigned long aCurrentTick = KMiscTools::getMilliseconds();

	int aDelta = (int)(aCurrentTick - mLastClockTick);
	if (aDelta < 0) // Error: Did we switch processor cores?
	{
		// attempt to recover
		aDelta = (mAverageClockTickDelta < kMaxClockTickDelta) ? mAverageClockTickDelta : kMaxClockTickDelta;
	}
	else if (aDelta > 0)
	{
		if (aDelta > kMaxClockTickDelta) // Lagging behind
		{
			aDelta = kMaxClockTickDelta;
		}
		else
		{
			// compute averages to help recover from switching cores.
			mAverageClockTickDelta = mAverageClockTickDelta*mClockTickCount + aDelta;
			mClockTickCount++;
			mAverageClockTickDelta /= mClockTickCount;

			// smooth out to the last 4000 updates or so
			if (mClockTickCount > 5000) mClockTickCount = 1000;
		}
	}

	mClock += (unsigned long)aDelta;
	mLastClockTick = aCurrentTick;

	return mClock;
}

/*!***********************************
// @return    	void
//
// \brief		Safely deletes widgets when it's guaranteed to be safe.
// \details 	<b>Description:</b> To avoid the possibility that a Widget gets deleted while
//				it is on the program stack, widgets are pushed into a safe delete list and
//				deleted when the program stack has unwound enough to guarantee there are no 
//				widgets in it.
//************************************/
void Sexy::SexyAppBase::ProcessSafeDeleteList()
{
	if(mUpdateAppDepth == 0)
	{
		for (unsigned int i = 0; i < mSafeDeleteList.size(); i++)
			delete mSafeDeleteList[i];

		mSafeDeleteList.clear();
	}
}

/*!***********************************
// @return    	void
// @param     	theWidget
//
// \brief		Call this when deleting a widget.
// \details 	<b>Description:</b> Make sure we don't delete a Widget while it's
//				on the program stack.
//************************************/
void Sexy::SexyAppBase::SafeDeleteWidget( Widget* theWidget )
{
	if(std::find(mSafeDeleteList.begin(), mSafeDeleteList.end(), theWidget) == mSafeDeleteList.end())
		mSafeDeleteList.push_back(theWidget);
}
/*!***********************************
// @return    	void
//
// \brief		Call This to post a shutdown message.
// \details 	<b>Description:</b> Shuts down the Game app when it is safe.
//************************************/
void Sexy::SexyAppBase::Shutdown()
{
	if (!mShutdown)
	{
		mShutdown = true;

// 			WriteToRegistry();

	}
}

/*!***********************************
// @return    	void
//
// \brief		Wait for the main loading thread to receive the shutdown signal.
// \details 	<b>Description:</b> If the user/player exits the game while the 
//				main loading thread is running, then we have to wait for the loading 
//				thread to see the shutdown message and exit so we can shutdown without
//				causing problems (like deleting the ResourceManger that it is inevitably using).
//************************************/
void Sexy::SexyAppBase::WaitForLoadingThread()
{
	while(mLoadingThreadStarted && !mLoadingThreadCompleted)
		KMiscTools::pause(20);

	if(mLoadingThread)
		mLoadingThread->waitForCompletion();
}

/*!***********************************
// @return    	void
//
// \brief		Called by Start() to kick off the loading thread.
// \details 	<b>Description:</b> Do no call this directly.  This is automatically called by Start().
//				Games that override this method will use the second thread to do loading tasks that would 
//				normally slow or stall the game.
//************************************/
void Sexy::SexyAppBase::StartLoadingThread()
{
	if (!mLoadingThreadStarted)
	{
		mYieldMainThread = true; 
		mLoadingThreadStarted = true;
		delete mLoadingThread;
		mLoadingThread = new KSysThread(LoadingThreadProcStub,this, 0);
		mLoadingThread->setThreadPriority(KSYSTHREAD_HIGH);
	}
}

/*!***********************************
// @return    	void
// @param     	lpParam
// @param     	nParam
//
// \brief		Necessary to Kick off the loading thread.
// \details 	<b>Description:</b> The Kanji Worker Thread Stub to call the Loading Thread Procedure.
//************************************/
void Sexy::SexyAppBase::LoadingThreadProcStub( void *lpParam, long nParam )
{
	SexyAppBase* aSexyApp = (SexyAppBase*) lpParam;
	aSexyApp->LoadingThreadProc();		
	aSexyApp->mLoadingThreadCompleted = true;
}

/*!***********************************
// @return    	float
//
// \brief		Returns a float between [0..1.0] for the loading progress.
// \details 	<b>Description:</b> Attempts to calculate the remaining tasks and return a float.
//************************************/
float Sexy::SexyAppBase::GetLoadingThreadProgress()
{
	if (mLoaded)
		return 1.0f;
	if (!mLoadingThreadStarted)
		return 0.0f;
	if (mNumLoadingThreadTasks == 0)
		return 0.0f;
	return K_MIN(mCompletedLoadingThreadTasks / static_cast<float>(mNumLoadingThreadTasks), 1.0f);
}

/*!***********************************
// @return    	bool
//
// \brief		Does the Game have Focus?
// \details 	<b>Description:</b> Forwards the Call to Kanji to determine if this app has focus.
//************************************/
bool Sexy::SexyAppBase::HasFocus()
{
	if (mKWindow)
		return mKWindow->hasFocus();
	
	return true;
}

/*!***********************************
// @return    	void
// @param     	windowed
//
// \brief		Change Windowed/Fullscreen Mode.
// \details 	<b>Description:</b> Change Windowed/Fullscreen Mode.
//************************************/
void Sexy::SexyAppBase::SetWindowed(bool windowed)
{
	if (mIsWindowed == windowed) return;
	
	if(mKWindow->toggleFullScreen(!windowed))
		mIsWindowed = windowed;
}

/*!***********************************
// @return    	void
//
// \brief		Toggle the current Fullscreen/Windowed Mode.
// \details 	<b>Description:</b> Toggle the current Fullscreen/Windowed Mode.
//************************************/
void Sexy::SexyAppBase::ToggleWindowed()
{
	if(mKWindow->toggleFullScreen(mIsWindowed))
		mIsWindowed = !mIsWindowed;
}

/*!***********************************
// @return    	bool
// @param     	theKeyName
// @param     	theSubKeys
//
// \brief		Returns the list of strings @ theKeyName
// \details 	<b>Description:</b> Returns the list of strings @ theKeyName.  On success, clears values 
//				already in theSubKeys.
//************************************/
bool Sexy::SexyAppBase::RegistryGetSubKeys( const SexyString& theKeyName, StringVector* theSubKeys )
{
	if (mSettings && theSubKeys && mSettings->HasStringVector(theKeyName))
	{
		StringVector aVector;
		aVector = mSettings->GetStringVector(theKeyName);

		if (aVector.empty()) return false;
		
		theSubKeys->resize(aVector.size());
		std::copy(aVector.begin(),aVector.end(), theSubKeys->begin());

		return true;
	}

	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theString
//
// \brief		Looks in the Engine Settings for the Specified String.
// \details 	<b>Description:</b> Retained for Backward Compatibility.
//************************************/
bool Sexy::SexyAppBase::RegistryReadString( const SexyString& theValueName, SexyString* theString )
{
	if (mSettings && theString && mSettings->HasString(theValueName))
	{
		*theString = mSettings->GetString(theValueName);
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theValue
//
// \brief		Looks in the Engine Settings for the Specified Int.
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyAppBase::RegistryReadInteger( const SexyString& theValueName, int* theValue )
{
	if (mSettings && theValue && mSettings->HasString(theValueName))
	{
		*theValue = mSettings->GetInteger(theValueName);
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theValue
//
// \brief		Looks in the Engine Settings for the Specified Bool.
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyAppBase::RegistryReadBoolean( const SexyString& theValueName, bool* theValue )
{
	if (mSettings && theValue && mSettings->HasBoolean(theValueName))
	{
		*theValue = mSettings->GetBoolean(theValueName);
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theString
// @param     	bWriteImmediately
//
// \brief		Writes the Specified String to the Engine Settings.
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyAppBase::RegistryWriteString( const SexyString& theValueName, const SexyString& theString, bool bWriteImmediately /*= false*/ )
{
	if (mSettings)
	{
		mSettings->SetString(theValueName, theString);

		if(bWriteImmediately)
		{
			SexyPropertiesWriter aPropWriter(mSettings);
			aPropWriter.WritePropertiesFile(GetAppDataFolder() + _S("settings.xml"));
		}
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theValue
// @param     	bWriteImmediately
//
// \brief		Writes the Specified Integer to the Engine Settings.
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyAppBase::RegistryWriteInteger( const SexyString& theValueName, int theValue, bool bWriteImmediately /*= false*/ )
{
	if (mSettings)
	{
		mSettings->SetInteger(theValueName, theValue);

		if(bWriteImmediately)
		{
			SexyPropertiesWriter aPropWriter(mSettings);
			aPropWriter.WritePropertiesFile(GetAppDataFolder() + _S("settings.xml"));
		}
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
// @param     	theValueName
// @param     	theValue
// @param     	bWriteImmediately
//
// \brief		Writes the Specified Boolean to the Engine Settings.
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyAppBase::RegistryWriteBoolean( const SexyString& theValueName, bool theValue, bool bWriteImmediately /*= false*/ )
{
	if (mSettings)
	{
		mSettings->SetBoolean(theValueName, theValue);

		if(bWriteImmediately)
		{
			SexyPropertiesWriter aPropWriter(mSettings);
			aPropWriter.WritePropertiesFile(GetAppDataFolder() + _S("settings.xml"));
		}
		return true;
	}
	return false;
}

/*!***********************************
// @return    	bool
//
// \brief		Loads "properties/default.xml"
// \details 	<b>Description:</b> Loads "properties/default.xml" to mProperties so that GetProperties() 
//				contains game variables/strings.  Now automatically loaded during Init()
//************************************/
bool Sexy::SexyAppBase::LoadProperties()
{
	if(mProperties == NULL)
		mProperties = new SexyProperties();

	SexyPropertiesParser aPropParser(mProperties);
	
	return aPropParser.ParsePropertiesFile(DEFAULT_PROPERTIES_PATH);
}

/*!***********************************
// @return    	void
//
// \brief		Saves the Engine settings to a local XML file on the HDD
// \details 	<b>Description:</b> Saves the Engine settings to a local XML file on the HDD
//************************************/
void Sexy::SexyAppBase::WriteToRegistry()
{
	if (mSettings == NULL) return;

	mSettings->SetBoolean(_S("IsWindowed"), mIsWindowed);
	mSettings->SetInteger(_S("MusicVolume"), mMusicVolume);
	mSettings->SetInteger(_S("SfxVolume"), mSfxVolume);
	mSettings->SetBoolean(_S("Muted"), (mMuteCount - mAutoMuteCount > 0) ? true : false);
	mSettings->SetBoolean(_S("WaitForVSync"), IsVSyncEnabled());

	// Save a local copy of the engine settings (JPOAG)
	SexyPropertiesWriter aPropWriter(mSettings);
	aPropWriter.WritePropertiesFile(GetAppDataFolder() + _S("settings.xml"));
}

/*!***********************************
// @return    	void
//
// \brief		Reads the Engine settings
// \details 	<b>Description:</b> Reads the Engine settings from the resource pak and then 
//				overwrites those settings using any local xml copies found.
//				Read the file "properties/settings.xml" as defined in config.h
//************************************/
void Sexy::SexyAppBase::ReadFromRegistry()
{
	if(mSettings == NULL)
		mSettings = new SexyProperties();

	SexyPropertiesParser aPropParser(mSettings);
	aPropParser.ParsePropertiesFile(DEFAULT_SETTINGS_PATH); // parse the settings that ship with the game

	// Doesn't matter if this fails.  In fact, it should fail on first run!
	aPropParser.ParsePropertiesFile(GetAppDataFolder() + _S("settings.xml"));

	//////////////////////////////////////////////////////////////////////////
	// Now Load System Settings
	//////////////////////////////////////////////////////////////////////////

	if (mSettings->HasBoolean(_S("IsWindowed")))
		mIsWindowed = mSettings->GetBoolean(_S("IsWindowed"), mIsWindowed);

	if (mSettings->HasInteger(_S("MusicVolume")))
		mMusicVolume = mSettings->GetInteger(_S("MusicVolume"), DEFAULT_MUSIC_VOLUME);

	if (mSettings->HasInteger(_S("SfxVolume")))
		mSfxVolume = mSettings->GetInteger(_S("SfxVolume"), DEFAULT_SOUND_VOLUME);

	if (mSettings->HasBoolean(_S("Muted")))
	{
		bool aMute = mSettings->GetBoolean(_S("Muted"), false);
		mMuteCount = aMute ? 1 : 0;
	}

	if (mSettings->HasBoolean(_S("WaitForVSync")))
		mVSyncEnabled = mSettings->GetBoolean(_S("WaitForVSync"), true);
}

/*!***********************************
// @return    	void
//
// \brief		Call before displaying a system dialog.
// \details 	<b>Description:</b> Call before displaying a system dialog.
//************************************/
void Sexy::SexyAppBase::BeginPopup()
{
	//if(mGraphics)
	//	mGraphics->Finalize();
#ifndef K_IPHONE
	if(mKWindow)
		mKWindow->forceWindowed();
#endif
}

/*!***********************************
// @return    	void
//
// \brief		Called After a System Dialog Exits.
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyAppBase::EndPopup()
{
	// When game regains focus, fullscreen will automatically be restored!

}

/*!***********************************
// @return    	void
// @param     	theText
// @param     	theTitle
//
// \brief		Creates and Displays aSystem Message Box. 
// \details 	<b>Description:</b> This is not for game dialogs, this is for critical errors.
//************************************/
void Sexy::SexyAppBase::MsgBox( const SexyString &theText, const SexyString &theTitle /*= _S("Message")*/)
{
	if(mKWindow)
	{
		BeginPopup();
		KMiscTools::messageBox(SexyStringToStringFast(theText).c_str(), SexyStringToStringFast(theTitle).c_str());
		EndPopup();
	}
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		Send the Text to the System Clipboard.
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyAppBase::CopyToClipboard(const SexyString& theString)
{
#ifndef K_IPHONE
	Sexy::CopyToClipboard(theString, mKWindow->getWindowHandle());
#endif
}

/*!***********************************
// @return    	SexyString
//
// \brief		Returns the System Clipboard Text.
// \details 	<b>Description:</b> 
//************************************/
SexyString Sexy::SexyAppBase::GetClipboard()
{
#ifndef K_IPHONE
	return Sexy::GetClipboard(mKWindow->getWindowHandle());
#else
	return _S("");
#endif
}

/*!***********************************
// @return    	void
// @param     	theCursorNum
//
// \brief		Change to the specified cursor.
// \details 	<b>Description:</b> Valid cursors are:
//				{SEXY_CURSOR_POINTER,SEXY_CURSOR_HAND, SEXY_CURSOR_DRAGGING, SEXY_CURSOR_TEXT, SEXY_CURSOR_CIRCLE_SLASH, SEXY_CURSOR_SIZEALL,
//				 SEXY_CURSOR_SIZENESW, SEXY_CURSOR_SIZENS, SEXY_CURSOR_SIZENWSE, SEXY_CURSOR_SIZEWE, SEXY_CURSOR_WAIT, SEXY_CURSOR_NONE, 
//				 SEXY_CURSOR_CUSTOM}
//************************************/
void Sexy::SexyAppBase::SetCursor( int theCursorNum )
{
	if (theCursorNum >= 0 && theCursorNum < NUM_SEXY_CURSORS)
	{
		mCurrentCursor = theCursorNum;
	}

	if (mCursorMap[mCurrentCursor] == NULL || mCustomCursorsEnabled == false)
	{
		KInput::showPointer();
#ifdef K_WIN32
		switch(mCurrentCursor)
		{
		case SEXY_CURSOR_POINTER:
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			break;
		case SEXY_CURSOR_HAND:
			::SetCursor(::LoadCursor(NULL, IDC_HAND));
			break;
		case SEXY_CURSOR_DRAGGING:
			::SetCursor(mDraggingCursor);
			break;
		case SEXY_CURSOR_TEXT:
			::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			break;
		case SEXY_CURSOR_CIRCLE_SLASH:
			::SetCursor(::LoadCursor(NULL, IDC_NO));
			break;
		case SEXY_CURSOR_SIZEALL:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			break;
		case SEXY_CURSOR_SIZENESW:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			break;
		case SEXY_CURSOR_SIZENS:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			break;
		case SEXY_CURSOR_SIZENWSE:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			break;
		case SEXY_CURSOR_SIZEWE:	
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			break;
		case SEXY_CURSOR_WAIT:
			::SetCursor(::LoadCursor(NULL, IDC_WAIT));
			break;
		case SEXY_CURSOR_NONE:
			::SetCursor(NULL);
			break;
		case SEXY_CURSOR_CUSTOM:
			::SetCursor(NULL);	
			break;
		}
#endif
	}
	else
		KInput::hidePointer();
}

/*!***********************************
// @return    	int
//
// \brief		Return the current cursor ID.
// \details 	<b>Description:</b> Return the current cursor ID.  See SetCursor()
//************************************/
int Sexy::SexyAppBase::GetCursor()
{
	return mCurrentCursor;
}

/*!***********************************
// @return    	void
// @param     	theCursorNum
// @param     	theImage
//
// \brief		Replace the default system cursor image with your own, custom cursor image.
// \details 	<b>Description:</b>Replace the default system cursor image with your own, custom cursor image. 
//************************************/
void Sexy::SexyAppBase::SetCursorImage( int theCursorNum, SexyImage* theImage )
{
	mCursorMap[theCursorNum] = theImage;
}

/*!***********************************
// @return    	void
// @param     	theEnabled
//
// \brief		Enable/disable custom cursors
// \details 	<b>Description:</b> Enable/disable custom cursors
//************************************/
void Sexy::SexyAppBase::EnableCustomCursors(bool theEnabled)
{
	mCustomCursorsEnabled = theEnabled;
	SetCursor(mCurrentCursor);
}

/*!***********************************
// @return    	bool
// @param     	theKey
//
// \brief		
// \details 	<b>Description:</b> First chance at catching a key stroke before being passed to 
//				the widget manager.  Return True if you handled the Keystroke,
//				False if you did not.
//************************************/
bool Sexy::SexyAppBase::DebugKeyDown( EKeyboardLayout theKey )
{
	if(mDebugKeysEnabled)
	{
		if(gDebugFont == NULL)
		{
			gDebugFont = new SexyFont(GetSystemFontsFolder() + _S("arial.ttf"));
			gDebugFont->SetFontPointHeight(8);
		}

		if(gDebugGraphic == NULL)
		{
			gDebugGraphic = new Graphics();
			gDebugGraphic->SetFont(gDebugFont);
		}

		switch (theKey)
		{
		case K_VK_F1:
			{
				mShowProgramStats = ++mShowProgramStats%3;
				return true;
			}
		case K_VK_F2:
			if (SexyPerf::IsPerfOn() == false)
			{
				SexyPerf::BeginPerf();
				return true;
			}
			else
			{
				SexyPerf::EndPerf();
				KMiscTools::messageBox("Perf Results", SexyPerf::GetResults().c_str());
				return true;
			}

		case K_VK_F3:
			{
				mShowFPS = !mShowFPS;
				return true;
			}
		case K_VK_F11:
			TakeScreenShot();
			break;
		}
	}

	return false;
}
/*!***********************************
// @return    	void
//
// \brief		Debug Method to draw Stats.
// \details 	<b>Description:</b> See DrawFPS().  Press F1 to toggle while in debug mode.
//************************************/
void Sexy::SexyAppBase::DrawStats()
{
	//////////////////////////////////////////////////////////////////////////
	// Draw Debug Info
	//////////////////////////////////////////////////////////////////////////
	if (mShowProgramStats && gDebugFont != NULL && gDebugGraphic != NULL)
	{
		//////////////////////////////////////////////////////////////////////////
		// Step 1: Format Strings
		//////////////////////////////////////////////////////////////////////////
		std::vector<SexyString> aStringList;

		//////////////////////////////////////////////////////////////////////////
		// Game Version and Local Time
		//////////////////////////////////////////////////////////////////////////
		time_t aTime = time(NULL);
		SexyString aTimeStr = StringToSexyStringFast(ctime(&aTime));
		aTimeStr = aTimeStr.substr(0, aTimeStr.size() -1);
		aStringList.push_back(StrFormat(_S("^FFFFFF^Game Version: %s, Current Time: %s"), mVersion.c_str(), aTimeStr.c_str()));

		//////////////////////////////////////////////////////////////////////////
		// Windowed Mode and Render Type
		//////////////////////////////////////////////////////////////////////////
		SexyString aRenderType = 
			(mCurrentRendererType == K_DIRECTX9) ? _S("DX9") : 
			((mCurrentRendererType == K_DIRECTX) ? _S("DX8") : 
			((mCurrentRendererType == K_OPENGL) ? _S("OpenGL") : _S("")));

		aStringList.push_back(StrFormat(_S("^FFFF00^%s %s"), mIsWindowed ? _S("Windowed") : _S("Fullscreen"), aRenderType.c_str()));

		//////////////////////////////////////////////////////////////////////////
		// Game and Desktop Display Modes
		//////////////////////////////////////////////////////////////////////////
		aStringList.push_back(StrFormat(_S("^FFFF00^BackBuffer: %dx%dx%d  DisplayMode: %dx%d @ %dHz"), mKWindow->getWindowWidth(), 
			mKWindow->getWindowHeight(), mKWindow->getWindowDepth(), mKWindow->getDesktopWidth(), mKWindow->getDesktopHeight(), 
			mKWindow->getDesktopDisplayFrequency()));

		//////////////////////////////////////////////////////////////////////////
		// The adapter Name
		//////////////////////////////////////////////////////////////////////////
		char anAdapterStr[256] = {0};
		mKWindow->getAdapterName(anAdapterStr, 256);
		aStringList.push_back(StrFormat(_S("^FF8000^%s"), StringToSexyString(anAdapterStr).c_str()));

		//////////////////////////////////////////////////////////////////////////
		// OS version
		//////////////////////////////////////////////////////////////////////////
		aStringList.push_back(StrFormat(_S("^FF00FF^%s"), StringToSexyString(KMiscTools::getOSVersion()).c_str()));

		//////////////////////////////////////////////////////////////////////////
		// Video Memory
		//////////////////////////////////////////////////////////////////////////
		aStringList.push_back(StrFormat(_S("^00FF80^Video Memory: [%d Kb] Used / [%d Kb] Availiable"), mKWindow->getVidMem() - mKWindow->getAvailVidMem(), mKWindow->getVidMem()));

		int aWidth = 0;
		SexyString aRenderString;
		for (unsigned int i = 0; i < aStringList.size(); i++)
		{
			int aStrWidth = gDebugFont->StringWidth(aStringList[i]);
			aWidth = (aWidth < aStrWidth) ? aStrWidth : aWidth;
			aRenderString += aStringList[i] + _S("\n");
		}

		int aHeight = gDebugFont->GetLineSpacing() * (aStringList.size());
		int y = (mShowProgramStats == 1) ? mHeight - aHeight : 0;

		gDebugGraphic->SetColor(SexyColor(0,0,0,128));
		gDebugGraphic->FillRect(SexyRect(0, y, aWidth, aHeight));
		gDebugGraphic->SetColor(SexyColor::White);
		gDebugGraphic->SetFont(gDebugFont);

		for(unsigned int i = 0; i < aStringList.size(); ++i)
		{
			gDebugGraphic->DrawStringColor(aStringList[i],0, y);
			y += gDebugFont->GetLineSpacing();
		}
	}
}

/*!***********************************
// @return    	void
//
// \brief		Toggle the Fullscreen/Windowed State
// \details 	<b>Description:</b> Toggle the Fullscreen/Windowed State
//************************************/
void Sexy::SexyAppBase::SwitchScreenMode()
{
	SwitchScreenMode(!mIsWindowed);
}

/*!***********************************
// @return    	void
// @param     	wantWindowed
//
// \brief		Change the Windowed/Fullscreen Mode.
// \details 	<b>Description:</b> Change the Windowed/Fullscreen Mode.
//************************************/
void Sexy::SexyAppBase::SwitchScreenMode( bool wantWindowed )
{
	if(wantWindowed && mIsWindowed) return;

	if(mKWindow && mKWindow->toggleFullScreen(!wantWindowed))
		mIsWindowed = wantWindowed;
}

/*!***********************************
// @return    	void
// @param     	wantWindowed
// @param     	is3d
// @param     	force
//
// \brief Change the FullScreen/Windowed Mode.
// \details <b>Description:</b> Last 2 parameters are ignored and the call is forwarded to SwitchScreenMode(bool)
// \depreciated Retained for backward compatibility.\n
//************************************/
void Sexy::SexyAppBase::SwitchScreenMode( bool wantWindowed, bool is3d, bool force /*= false*/ )
{
	SwitchScreenMode(wantWindowed);
}

/*!***********************************
// @return    	void
// @param     	theAutoMute
//
// \brief		Mute music and sound
// \details 	<b>Description:</b> Mute music and sound
// \depreciated Retained for backward compatibility.\n
//************************************/
void Sexy::SexyAppBase::Mute(bool theAutoMute)
{	
	mMuteCount++;

	if (theAutoMute == true)
		mAutoMuteCount++;

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}

/*!***********************************
// @return    	void
// @param     	theAutoMute
//
// \brief		Unmute music and sound
// \details 	<b>Description:</b> Unmute music and sound
// \depreciated Retained for backward compatibility.\n
//************************************/
void Sexy::SexyAppBase::Unmute(bool theAutoMute)
{	
	if (mMuteCount > 0)
	{
		mMuteCount--;

		if (theAutoMute == true)
			mAutoMuteCount--;
	}

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}

/*!***********************************
// @return    	void
// @param     	theMusicVolume
//
// \brief		Set global music volume
// \details 	<b>Description:</b> Set global music volume, this is synced with volume in MusicSoundBase.
// \depreciated Retained for backward compatibility.\n
//************************************/
void Sexy::SexyAppBase::SetMusicVolume(int theMusicVolume)
{
	mMusicVolume = theMusicVolume;

	if (mMusicManager != NULL)
		mMusicManager->SetGlobalMusicVolume((mMuteCount > 0) ? 0 : mMusicVolume);
}

/*!***********************************
// @return    	void
// @param     	theSfxVolume
//
// \brief		Set global sound volume
// \details 	<b>Description:</b> Set global sound volume, this is synced with volume in MusicSoundBase.
// \depreciated Retained for backward compatibility.\n
//************************************/
void Sexy::SexyAppBase::SetSfxVolume(int theSfxVolume)
{
	mSfxVolume = theSfxVolume;

	if (mSoundManager != NULL)
		mSoundManager->SetGlobalSoundVolume((mMuteCount > 0) ? 0 : mSfxVolume);
}

bool EnumScreenShots (const char *lpszEntryName, bool bIsFolder, void *lpCallBackData)
{
	if(bIsFolder) return true;

	int* aMaxID = (int*)lpCallBackData;
	if (aMaxID != NULL)
	{
		std::string anImagePrefix = "image";
		int aNum = *aMaxID;
		if(sscanf(lpszEntryName,(anImagePrefix + "%d.png").c_str(), &aNum)==1)
		{
			if(aNum >= *aMaxID)
				*aMaxID = aNum + 1;
		}
	}
	return true;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyAppBase::TakeScreenShot()
{
	SexyString aScreenShotDir = GetAppDataFolder() + _S("_screenshots");
	int aMaxID = 0;
	KMiscTools::enumerateFolder(SexyStringToStringFast(aScreenShotDir).c_str(), EnumScreenShots, &aMaxID);

	MkDir(SexyStringToStringFast(GetAppDataFolder() + _S("_screenshots")));

	SexyString aFileName = StrFormat(_S("%s_screenshots\\image%d.png"), GetAppDataFolder().c_str(), aMaxID);
	if (mKWindow)
		mKWindow->saveBackBuffer(SexyStringToStringFast(aFileName).c_str(), K_IMAGE_PNG);
}
/*!***********************************
// @return    	void
// @param     	bIsAssertion
// @param     	nExceptionCode
// @param     	lpszExceptionName
// @param     	bHasParam
// @param     	lpParam
// @param     	lpszAssertionFileName
// @param     	nAssertionLine
// @param     	nStackDepth
// @param     	lpStackReturnAddr
// @param     	lpszStackModule
//
// \brief		Catches System Exceptions: Warning, the stack has already unwound by this point to attempt recovery.
// \details 	<b>Description:</b> Warning, the stack has already unwound by this point to attempt recovery.
//************************************/
void SEHCatcher (bool bIsAssertion,
				 unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
				 const char *lpszAssertionFileName, long nAssertionLine,
				 long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[])
{
	K_LOG("Sexy: EXCEPTION CAUGHT: \n\tIsAssert = %s\n\tException Code: %0X\n\t%s\n\tOccurred in %s\n\tOn Line: %i\n\t", 
		(bIsAssertion)?"true":false,
		nExceptionCode,
		(lpszExceptionName) ? lpszExceptionName : "(null)",
		(lpszAssertionFileName) ? lpszAssertionFileName : "(null)",
		nAssertionLine);

	K_LOG("Begin Stack Dump:");

	for(long i = 0; i < nStackDepth; i++)
		K_LOG("\n\t%s @ %s", lpszStackModule[i], lpStackReturnAddr[i]);

	void LoadPropertySettings();
}

/*!***********************************
// @return    	void
//
// \brief		The event trigger for an FPS timer
// \details 	<b>Description:</b> The event trigger for an FPS timer
//************************************/
void Sexy::SexyFPSTimer::OnEvent()
{
	if (mLastCheck == 0)
	{
		mLastCheck = gSexyAppBase->GetClock();
		mTickCount = 0;
		return;
	}

	if(gSexyAppBase->GetClock() - mLastCheck > 1000)
	{
		double aDiff = (float)gSexyAppBase->GetClock() - (float)mLastCheck;
		aDiff = 1000.0 / aDiff;
		mFPS = (long)((float)mTickCount * aDiff);
		mTickCount = 0;
		mLastCheck = gSexyAppBase->GetClock();
	}

	mTickCount++;
}

