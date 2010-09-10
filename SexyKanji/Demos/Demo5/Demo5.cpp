//////////////////////////////////////////////////////////////////////////
//						Demo5.cpp
//
//	This is the starting point for all new projects. This file's purpose is
//	pretty small, but important. In here we create our application, initialize
//	it, and begin processing all the game code.
//
//	This demo will teach you:
//	* Using Translate instead of using XYs
//	* Keyboard input, setting focus
//	* Widgets: Dialog box, sliders, an options dialog box
//	* Advanced widget topics: mClip, using Layout_ flags, using images
//	* Making an options dialog box and controlling fullscreen, volumes,
//		3D settings.
//	* Parallax scrolling
//	* Random numbers
//	* Catching memory leaks
//	* Profiling
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
