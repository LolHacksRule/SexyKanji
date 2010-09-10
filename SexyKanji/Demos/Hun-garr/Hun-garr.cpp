//////////////////////////////////////////////////////////////////////////
//						Hun-garr.cpp
//
//	This is the starting point for all new projects. This file's purpose is
//	pretty small, but important. In here we create our application, initialize
//	it, and begin processing all the game code.
//
//	This demo will teach you:
//	* Using Hue, Saturation, Luminance values instead of RGB for color
//	* Custom cursors
//	* Handling loss/gain of application focus
//	* Basic particle effects
//	* Various programmatic effects/logic
//	* Putting most of the knowledge from the previous demos to work in a game
//
//	IMPORTANT: Comments for topics covered in the previous demos will
//	be kept to a minimum and removed in most cases. You should read through
//	the previous demos to familiarize yourself with anything you don't
//	understand. All functions, variables, and concepts are explained
//	either in this demo, or in previous ones if they've already been covered.
//////////////////////////////////////////////////////////////////////////

#include "GameApp.h"

/* Windows uses WinMain(). Mac, iPhone and iPad use kanjiMain() */

#ifdef K_WIN32
INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT iCmdShow) {
#else
int kanjiMain (int argc, char **argv) {
#endif

	// Create and initialize our game application.
	Sexy::GameApp* anApp = new Sexy::GameApp();
	if(anApp->Init())
	{
		// Starts the entire application: sets up the resource loading thread and 
		// custom cursor thread, and enters the game loop where the application
		// will remain until it is shut down. You will most likely not need to
		// override this function.
		anApp->Start();
	}
	delete anApp;

	return 0;
}
