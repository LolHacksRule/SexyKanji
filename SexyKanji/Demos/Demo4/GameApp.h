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

	// We're going to be responsible for creating and adding the title screen widget
	class TitleScreen;

	class GameApp : public SexyAppBase
	{
	private:
		Board*			mBoard;
		TitleScreen*	mTitleScreen;
		
		// We load a second copy of the music stream and refer to it with this ID
		unsigned int	mStreamID;	

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

		//////////////////////////////////////////////////////////////////////////
		//	Function: TitleScreenIsFinished
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called by the TitleScreen widget when it is about to close
		//	down, this indicates that we should now add the board widget and
		//	start the game.
		//////////////////////////////////////////////////////////////////////////
		void TitleScreenIsFinished();

		/* In SexyKanji the command line is not supported. */
		//virtual void HandleCmdLineParam(const std::string& theParamName, const std::string& theParamValue);
	};
}

#endif	// __GAMEAPP_H__