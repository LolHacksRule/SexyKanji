/********************************************************************
	project:	Sexy Kanji
	author(s):		James Poag, Jeremy Sullivan, Heiko Fischer, et al.
	tester(s):		Andrei (Mr. Puzzler)

	purpose:	Port of the SexyAppFramework created by PopCap Games
				to Kanji 2D Engine written by Emmanuel Marty, et al.

				"This product includes portions of the PopCap Games 
				Framework (http://developer.popcap.com/)."
*********************************************************************/

/********************************************************************
					  Sexy Kanji Ported License
*********************************************************************
Sexy Kanji Framework License

Version 2.0

Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

1. Redistributions of source code or derived libraries, whether source 
code or binary, must include a copy of this License in its entirety.

2. The end-user documentation included with a work which contains any 
portion of the PopCap Games Framework must include the following 
acknowledgment:

"This product includes portions of the PopCap Games Framework 
(http://developer.popcap.com/)."

Alternatively, this acknowledgment may appear in the software itself, 
wherever such third-party acknowledgments normally appear.

3. The names "PopCap" and "PopCap Games" must not be used to endorse 
or promote products derived from this software without prior written 
permission. For written permission, please contact bizdev@popcap.com.

4. Products derived from this software may not be called "PopCap", 
nor may "PopCap" appear in their name, without prior written 
permission of PopCap Games.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY, NON-INFRINGMENT AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL POPCAP GAMES OR ANYONE BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*Other Licenses*
The SexyKanji Framework provides interfaces to some third-party libraries, 
but licenses to those libraries are not automatically granted to you through 
use of the Framework. Listed below is information for libraries which REQUIRE 
EXPLICIT LICENSING if you release a product which uses them.

*MP3 Compressed Audio*
The Framework supports both MP3 and OGG decompression. The OGG format can be 
used for free, but if you must use MP3 in your product, you will have to contact 
Thompson Multimedia and arrange to pay for an MP3 license. Game developers can 
get a 'game license' to use MP3 for $2500. 
See mp3 licensing.

*J2K-Codec Library* (See Kanji Documentation)
The framework supports loading j2k images through the j2k-codec library. 
This library is NOT free and you must purchase your own copy if you want to 
use it in registered (non-demo) mode. You may obtain a copy from Alex Saveliev 
at http://j2k-codec.com. We do not distribute our key, so the library will run 
in demo mode with the following restrictions, as taken from the readme.txt file.

**KANJI**
This framework does NOT grant a license to the Kanji Engine.  It is, however, 
required that in order to develop and distribute games using this framework, you
will have to obtain a license for Kanji.  Please see http://www.kanjiengine.com/
or Contact Emmanuel Marty:  m a r t y (dot) e m m a n u e l (at) g m a i l (dot) c o m

*********************************************************************/
#ifndef __SEXYAPPBASE_H__
#define __SEXYAPPBASE_H__

#include "KPTK.h"
#include <queue>
#include <vector>
#include "Common.h"

class KSysThread;

namespace Sexy
{
	//////////////////////////////////////////////////////////////////////////
	// Generic FPS class: Simple call the 'OnEvent' and print the result!
	//////////////////////////////////////////////////////////////////(JPoag)/
	/*!***********************************
	// \class    	SexyFPSTimer
	//
	// \brief		Generic Frames Per Second (measured in Hertz) of Events
	// \details 	<b>Description:</b> Create the Timer and call 'OnEvent' when the
	//				Event you want to measure is triggered. This will time the event
	//				using the Game Clock (gSexyAppBase->GetClock()). GetFPS() will
	//				return a formated string for the timer.
	//************************************/
	class SexyFPSTimer
	{
		SexyString mTimerName;
		unsigned long mTickCount;
		unsigned long mLastCheck;
		long mFPS;
	public:
		SexyFPSTimer(SexyString theName = _S("")):mTimerName(theName), mTickCount(0), mLastCheck(0), mFPS(0){}
		~SexyFPSTimer(){}

		void OnEvent();

		SexyString GetFPS()
		{
			return StrFormat(_S("%s: %s Hz"), mTimerName.c_str(), mFPS ? StrFormat(_S("%3d"), mFPS).c_str() : _S("-"));
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// This enumeration is used for the mini state machine that runs in the
	// MainLoop 'Start().'  Each state allows the program to terminate when
	// changing states.
	//////////////////////////////////////////////////////////////////(JPOAG)/
	enum
	{
		UPDATESTATE_MESSAGES,
		UPDATESTATE_PROCESS_1,
		UPDATESTATE_PROCESS_2,
		UPDATESTATE_PROCESS_DONE
	};

	//////////////////////////////////////////////////////////////////////////
	// This enumeration is used for sending mouse events to the 
	// Widget Manager.
	//////////////////////////////////////////////////////////////(JSullivan)/
	enum
	{
		MOUSE_BUTTON_LEFT = 1,
		MOUSE_BUTTON_RIGHT = -1,
		MOUSE_BUTTON_CENTER = 3,
		MOUSE_DBLCLICK_LEFT = 2,
		MOUSE_DBLCLICK_RIGHT = -2,
	};

	enum
	{
		SEXY_CURSOR_POINTER,
		SEXY_CURSOR_HAND,
		SEXY_CURSOR_DRAGGING,
		SEXY_CURSOR_TEXT,
		SEXY_CURSOR_CIRCLE_SLASH,
		SEXY_CURSOR_SIZEALL,
		SEXY_CURSOR_SIZENESW,
		SEXY_CURSOR_SIZENS,
		SEXY_CURSOR_SIZENWSE,
		SEXY_CURSOR_SIZEWE,	
		SEXY_CURSOR_WAIT,
		SEXY_CURSOR_NONE,
		SEXY_CURSOR_CUSTOM,
		NUM_SEXY_CURSORS
	};

	class Widget;
	typedef std::vector<Widget*> WidgetSafeDeleteList;

	// Forward declarations
	class Graphics;
	class ResourceManager;
	class SoundManager;
	class MusicManager;
	class ImageManager;
	class FontManager;
	class WidgetManager;
	class SexyProperties;
	class SexyImage;

	/*!***********************************
	// \class    	SexyAppBase
	//
	// \brief		The Main Application.
	// \details 	<b>Description:</b> Derrive from this class to create a custom 'GameApp.cpp'
	//				version for your game.  See documentation and examples/demos
	//				to see how to startup/run the GameApp.
	//************************************/
	class SexyAppBase
	{
		//////////////////////////////////////////////////////////////////////////
		// Accessed by SexyAppBase Only!
		//////////////////////////////////////////////////////////////////(JPOAG)/
	private:
		// to keep Global Volumes in sync
		friend class MusicSoundBase;

		KWindow*							mKWindow;
		bool								mIsWindowed;

		unsigned long						mClock;
		unsigned long						mLastClockTick;
		unsigned long						mAverageClockTickDelta;
		unsigned long						mClockTickCount;

		WidgetSafeDeleteList				mSafeDeleteList;
		int									mUpdateAppDepth;

		bool								mVSyncBroken;
		unsigned long						mVSyncBrokenTestUpdates;
		bool								mVSyncEnabled;
		bool								mLastDrawWasEmpty;
		bool								mPaused;
		bool								mNeedFlip;

		//////////////////////////////////////////////////////////////////////////
		// Debug keys
		//////////////////////////////////////////////////////////////////////////
		bool								mDebugKeysEnabled;
		bool								mShowFPS;
		int									mShowProgramStats;
		Erenderer							mCurrentRendererType;

		ResourceManager*					mResourceManager;
		SoundManager*						mSoundManager;
		MusicManager*						mMusicManager;
		ImageManager*						mImageManager;
		FontManager*						mFontManager;
		WidgetManager*						mWidgetManager;

		SexyProperties*						mProperties;	// properties/default.xml
		SexyProperties*						mSettings;		// properties/settings.xml overwritten by GetAppDataFolder()/settings.xml
		KSysThread*							mLoadingThread;

		//////////////////////////////////////////////////////////////////////////
		// Music, sound, mute
		//////////////////////////////////////////////////////////////////////////
		int									mMusicVolume;
		int									mSfxVolume;
		int									mMuteCount;
		int									mAutoMuteCount;

	protected:
		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		SexyString							mCompanyName;
		SexyString							mTitle;
		SexyString							mVersion;
		int									mWidth;
		int									mHeight;
		int									mFullscreenBits; // Bits Per Pixel {-1, 16, 32}

		bool								mShutdown; 
		bool								mAutoStartLoadingThread;
		bool								mRunning;
		bool								mYieldMainThread;
		bool								mLoadingThreadStarted;
		bool								mLoadingThreadCompleted;
		int									mNumLoadingThreadTasks;
		int									mCompletedLoadingThreadTasks;
		bool								mLoadingFailed;
		bool								mLoaded;

		int									mCurrentCursor;
		std::vector<SexyImage*>				mCursorMap;
		bool								mCustomCursorsEnabled;
#ifdef K_WIN32
		HCURSOR								mDraggingCursor;
#endif

		//////////////////////////////////////////////////////////////////////////
		// Internal structures
		//////////////////////////////////////////////////////////////////////////
		Graphics*							mGraphics;
		
		// Make sure we only process events during the UPDATESTATE_MESSAGES step.
		std::queue<KEvent>					mDeferredEventQueue; 

	public:
		SexyAppBase();
		virtual ~SexyAppBase();

		/************************************************************************/
		/* Events                                                               */
		/************************************************************************/
		static bool							EventHandler(KEvent *theKEvent);
#ifdef K_WIN32
        static bool                         WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
		virtual void						PushKanjiEvent(KEvent* theKEvent);

		/************************************************************************/
		/* Main.cpp interface                                                    */
		/************************************************************************/
		virtual bool						Init();
		virtual void						Start();
		virtual void						StartAppLog();

		/************************************************************************/
		/* Callback Hooks                                                       */
		/************************************************************************/
		virtual void						InitHook();
		virtual void						ShutdownHook(){}
		virtual void						LoadingThreadCompleted(){}
		virtual void						LoadingThreadProc(){}

		/************************************************************************/
		/* Event Handlers                                                       */
		/************************************************************************/
		virtual void						GotFocus();
		virtual void						LostFocus();
		virtual void						DoUpdateFrames();
		virtual void						DoUpdateFramesF(float theFrac);
		virtual void						DrawDirtyStuff(Graphics *g);

		virtual bool						DebugKeyDown(EKeyboardLayout theKey);

		/************************************************************************/
		/* Class Interface                                                      */
		/************************************************************************/
		virtual void						SetTitle(std::wstring theTitle)			{mTitle = WStringToSexyStringFast(theTitle);}
		virtual void						SetTitle(std::string theTitle)			{mTitle = StringToSexyStringFast(theTitle);}
		virtual void						SetDimensions(int pWidth, int pHeight)	{mWidth = pWidth; mHeight = pHeight;}
		virtual void						SetBits(int pFullscreenBits)			{mFullscreenBits = pFullscreenBits;}

		virtual int							GetWidth()	{return mWidth;}
		virtual int							GetHeight()	{return mHeight;}

		virtual bool						GetDebugKeysEnabled() const { return mDebugKeysEnabled; }
		virtual void						SetDebugKeysEnabled(bool val) { mDebugKeysEnabled = val; }


		virtual KWindow*					GetKWindow() const { return mKWindow; }

		virtual bool						HasFocus();

		virtual bool						IsShuttingdown() const { return mShutdown; }
		virtual void						Shutdown();

		virtual bool						IsVSyncEnabled() const { return mVSyncEnabled && !mVSyncBroken; }
		virtual void						EnableVSync(bool val = true) { mVSyncEnabled = val; }

		virtual unsigned long				GetClock(); // In milliseconds
		virtual MusicManager*				GetMusicManager() const { return mMusicManager; }
		virtual SoundManager*				GetSoundManager() const { return mSoundManager; }
		virtual ImageManager*				GetImageManager() const { return mImageManager; }
		virtual FontManager*				GetFontManager() const { return mFontManager; }
		virtual WidgetManager*				GetWidgetManager() const { return mWidgetManager; }
		virtual ResourceManager*			GetResourceManager() const	{ return mResourceManager; }

		virtual SexyProperties*				GetSettings() const { return mSettings; }
		virtual SexyProperties*				GetProperties() const { return mProperties; }

		virtual void						SafeDeleteWidget(Widget* theWidget);
		virtual void						StartLoadingThread();
		virtual float						GetLoadingThreadProgress();

		virtual void						CopyToClipboard(const SexyString& theString);
		virtual SexyString					GetClipboard();

		virtual void						TakeScreenShot();

		virtual bool						GetWindowed() const { return mIsWindowed;}
		virtual void						SetWindowed(bool windowed);
		virtual void						ToggleWindowed();

		// Backwards Compatible 3D Testing: Kanji only supports 3D (JPOAG)
		DEPRECATED(virtual bool Is3DAccelerated(){return true;}, "** Deprecated function **");
		DEPRECATED(virtual bool	Is3DAccelerationSupported(){return true;}, "** Deprecated function **");
		DEPRECATED(virtual bool	Is3DAccelerationRecommended(){return true;}, "** Deprecated function **");
		DEPRECATED(virtual void	Set3DAcclerated(bool is3D, bool reinit = true){}, "** Deprecated function **");
		DEPRECATED(virtual void	Done3dTesting(){}, "** Deprecated function **");

		// Screen Switching
		virtual void						SwitchScreenMode(); // Works like a Toggle Switch
		virtual void						SwitchScreenMode(bool wantWindowed);
		virtual void						SwitchScreenMode(bool wantWindowed, bool is3d, bool force = false);

		// Music and Sound
		virtual void						SetMusicVolume(int theMusicVolume);
		virtual void						SetSfxVolume(int theSfxVolume);	
		virtual int							GetMusicVolume()	{ return mMusicVolume; }
		virtual int							GetSfxVolume()		{ return mSfxVolume; }
		virtual bool						IsMuted()			{ return mMuteCount > 0; }
		virtual void						Mute(bool theAutoMute = false);
		virtual void						Unmute(bool theAutoMute = false);

		/************************************************************************/
		/* Registry Access (backwards compatible)                               */
		/************************************************************************/
		DEPRECATED(virtual bool RegistryGetSubKeys(const SexyString& theKeyName, StringVector* theSubKeys), "** Deprecated function **");
		DEPRECATED(virtual bool RegistryReadString(const SexyString& theValueName, SexyString* theString), "** Deprecated function: use GetSettings()->GetString **");
		DEPRECATED(virtual bool RegistryReadInteger(const SexyString& theValueName, int* theValue), "** Deprecated function: use GetSettings()->GetInteger **");
		DEPRECATED(virtual bool RegistryReadBoolean(const SexyString& theValueName, bool* theValue), "** Deprecated function: use GetSettings()->GetBoolean **");
		DEPRECATED(virtual bool RegistryWriteString(const SexyString& theValueName, const SexyString& theString, bool bWriteImmediately = false), "** Deprecated function: use GetSettings()->SetString **");
		DEPRECATED(virtual bool RegistryWriteInteger(const SexyString& theValueName, int theValue, bool bWriteImmediately = false), "** Deprecated function: use GetSettings()->SetInteger **");
		DEPRECATED(virtual bool RegistryWriteBoolean(const SexyString& theValueName, bool theValue, bool bWriteImmediately = false), "** Deprecated function: use GetSettings()->SetBoolean **");

		virtual bool						LoadProperties();
		virtual void						WriteToRegistry();
		virtual void						ReadFromRegistry();

		/************************************************************************/
		/* System Dialog Methods                                                */
		/************************************************************************/
		virtual void						BeginPopup();
		virtual void						EndPopup();
		virtual void						MsgBox(const SexyString &theText, const SexyString &theTitle = _S("Message"));

		/************************************************************************/
		/* Cursor Handling                                                      */
		/************************************************************************/
		virtual void						SetCursor(int theCursorNum);
		virtual int							GetCursor();
		virtual void						SetCursorImage(int theCursorNum, SexyImage* theImage = NULL);
		virtual void						EnableCustomCursors(bool theEnabled);
		virtual bool						GetCustomCursorsEnabled() { return mCustomCursorsEnabled; }

		/************************************************************************/
		/* Internal Handlers                                                    */
		/************************************************************************/
	protected:
		virtual bool						ProcessDeferredMessages(bool bSingleMessage = false);
		virtual void						ProcessSafeDeleteList();
		virtual void						WaitForLoadingThread();
		static void							LoadingThreadProcStub(void *lpParam, long nParam);

		virtual void						DrawFPS();
		virtual void						DrawStats();
		virtual void						_Render( unsigned long &aLastTimeCheck, double &anUpdateFrameTimeAccum );
	};

	//////////////////////////////////////////////////////////////////////////
	// This is so the MAINWINDOWPROC will have access to the EventQueue.
	//////////////////////////////////////////////////////////////////(JPOAG)/
	extern SexyAppBase* gSexyAppBase;
}

#endif // __SEXYAPPBASE_H__
