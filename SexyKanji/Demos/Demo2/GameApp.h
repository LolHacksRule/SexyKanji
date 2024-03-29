#ifndef __GAMEAPP_H__
#define __GAMEAPP_H__

//////////////////////////////////////////////////////////////////////////
//							GameApp.h
//
//	This is what drives the whole game. In here, you derive your class
//	from SexyAppBase and implement common game tasks, such as 
//	responding to widgets (covered later), initializing and loading
//	resources, setting up the various game screens, etc.
//	All applications at minimum must have a class that derives from
//	SexyAppBase.
//
//	The GameApp class is used to do such things as create the main 
//	menu screen, create the main game class (where all drawing/updating/
//	interaction takes place), etc.
//////////////////////////////////////////////////////////////////////////

#include "SexyAppBase.h"

// We place all our classes inside the "Sexy" namespace to avoid name collisions
// with other libraries that might be added.
namespace Sexy
{
	// The GameApp class will be responsible for creating a class by the name
	// of "Board", which we will use to do all the game's drawing, input processing,
	// etc. Board is the second most important class and is where almost all of your
	// game logic code will originate from. It is a widget, which allows for
	// easy and automatic invocation of its update, drawing, and input processing
	// functions. See the "Board" class for more details. 
	class Board;

	// Alright, fonts and images! You'll learn more about these in GameApp.cpp
	// so check there for more info. 
	//class ImageFont;
	//class Image;
	/* In SexyKanji ImageFOnt has been replaced with SexyFont
	   Image has been replaced with SexyImage (DDImage and MemoryImage no longer exist) */
	class SexyFont;
	class SexyImage;
	class SexyMovie;

	class GameApp : public SexyAppBase
	{
	private:
		Board*		mBoard;	

	public:
		// These are the fonts and images we'll be using for this demo. See
		// GameApp.cpp for full information on fonts and images. We're going to
		// make them public for easy accessing, since the point of this tutorial
		// is to quickly get you up to speed on using the framework.
		SexyFont*	mTextFont;
		SexyFont*	mNumberFont;
		SexyImage*	mKanjiLogo;
		SexyImage*	mOpaqueBeamImg;
		SexyImage*	mMoonImg;
		SexyImage*	mTurbotImg;
		SexyMovie*	mFishMovie;

	public:
		GameApp();
		virtual ~GameApp();

		//////////////////////////////////////////////////////////////////////////
		//	Function: InitHook
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called after Sexy Initializes the application. The framework 
		//	will then automatically call the LoadingThreadProc() method after this 
		//	method returns.
		//////////////////////////////////////////////////////////////////////////		
		void InitHook();

		//////////////////////////////////////////////////////////////////////////
		//	Function: LoadingThreadProc
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Loads all resources in a separate thread. If there is a 
		//	loader/intro screen (not in this demo), would also update the
		//	loader progress indicator. When the function returns, the
		//	LoadingThreadCompleted() method is automatically called.
		//////////////////////////////////////////////////////////////////////////		
		virtual void LoadingThreadProc();

		//////////////////////////////////////////////////////////////////////////
		//	Function: LoadingThreadCompleted
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called when LoadingThreadProc is complete and all resources
		//	have been loaded. It is in this function that you would then set up
		//	your main menu or similar screen. For this particular demo however,
		//	we will go straight to the main game class, "Board".
		//////////////////////////////////////////////////////////////////////////		
		virtual void LoadingThreadCompleted();

		//////////////////////////////////////////////////////////////////////////
		//	Function: ShutdownHook
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called once when the game is shutting down. This is where you 
		//	should clean up after yourself: remove Widgets, free resources, and delete
		//	objects, etc.
		//////////////////////////////////////////////////////////////////////////	
		/* In SexyKanji you should clean up in ShutdownHook instead of
		   waiting until the destructor is called. */
		virtual void ShutdownHook();
	};
}

#endif	// __GAMEAPP_H__
