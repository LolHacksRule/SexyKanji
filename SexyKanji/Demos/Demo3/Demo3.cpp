//////////////////////////////////////////////////////////////////////////
//						Demo3.cpp
//
//	This is the starting point for all new projects. This file's purpose is
//	pretty small, but important. In here we create our application, initialize
//	it, and begin processing all the game code.
//
//	This demo will teach you:
//	* Modifying image data/bits
//	* Stretching images
//	* Mirroring/flipping images
//	* Animation using image strips
//	* Widgets: buttons, listeners, basic events, mouse move/down/up/drag
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
