#include "GameApp.h"
#include "TitleScreen.h"
#include "Board.h"
#include "WidgetManager.h"

// We will be accessing the resource manager in this demo, so include it's header
#include "ResourceManager.h"

// Required for playing music
//#include "SexyAppFramework/BassMusicInterface.h"
#include "MusicManager.h" // SexyKanji Note: We now use a MusicManager.

// Contains all the resources from the resources.xml file in our
// properties directory. See that file for more information.
#include "Res.h"


// The SexyAppFramework resides in the "Sexy" namespace. As a convenience,
// you'll see in all the .cpp files "using namespace Sexy" to avoid
// having to prefix everything with Sexy::
using namespace Sexy;

GameApp::GameApp()
{
	// mProdName is used for internal purposes to indicate the game that we're working on
	/* In SexyKanji mProdName has been removed as unnecessary */
	//mProdName = "Demo 4";

	// For internal uses, indicates the current product version
	//mProductVersion = "1.0";
	/* In SexyKanji mProductVersion has been renamed to mVersion */
	mVersion = _S("1.0"); // SexyKanji uses wide strings

	// This is the text that appears in the title bar of the application window
	//mTitle = StringToSexyStringFast("SexyAppFramework: " + mProdName + " - " + mProductVersion);
	/* In SexyKanji we'll use SetTitle() */
	SetTitle(_S("SexyKanji: Demo 4"));

	// Indicates the registry location where all registry keys will be read from
	// and written to. This is stored under the HKEY_CURRENT_USER tree on 
	// Windows systems.
	/* In SexyKanji we no longer use mRegKey. 
	Instead settings are stored in an xml file in the game's data folder. */
	//mRegKey = "PopCap\\SexyAppFramework\\Demo2";

	// Set the application width/height in terms of pixels here.
	// use a different resolution from Demo 1 just for fun.
	//mWidth = 800;
	//mHeight = 600;
	/* In SexyKanji we'll use SetDimensions() */
	SetDimensions(800, 600);

	/* In SexyKanji we are always 3D accelerated so this is no longer needed */
	//mAutoEnable3D = true;

	mBoard = NULL;
	mTitleScreen = NULL;

	// initialize with invalid stream ID
	mStreamID = MAX_MUSIC_STREAMS + 1;

	// See Board::UpdateF for a very lengthy explanation of this and smooth motion
	//mVSyncUpdates = true;
	/* In SexyKanji we call EnableVSync() */
	EnableVSync();
}

GameApp::~GameApp()
{
	/* In SexyKanji you should clean up in ShutdownHook() instead of
	   waiting until the destructor is called. */
}

void GameApp::InitHook()
{
	// We need to tell the resource manager to read in all the groups
	// and information from that main group we made, called ResourceManifest,
	// in the file "properties/resources.xml". The path/filename are
	// by default set up to load that file, so you must name it exactly as such.
	// This doesn't load any resources: it just parses the data and sets
	// things up for loading.
	//LoadResourceManifest();
	/* In SexyKanji you must use the ResourceManager. You can still call 
	   LoadResourceManifest(), we use ParseResourcesFile() here because 
	   we have multiple resource files for the demos. */
	// load properties/resource.xml with LoadResourceManifest():
	// GetResourceManager()->LoadResourceManifest();
	// be sure to check the return value in case parsing fails
	if (GetResourceManager()->ParseResourcesFile(_S("properties/resourcesdemo4.xml")) == false)
	{
		GetResourceManager()->ShowResourceError(true);
		mLoadingFailed = true;
		return;
	}

	// It's now the time to take a look at the resource 
	// file "properties/resourcesdemo4.xml", if not already done.

	// Next, we want to load our absolutely necessary files that have to
	// be loaded before anything else can run. You'll notice in the resourcesdemo4.xml
	// file that we created a group called Init that contains these resources.
	// You may call it whatever you like. Let's load those resources now.
	// We do that by calling the LoadResources method of our ResourceManager
	// and specifying in quotes the name of the resource group to load. 
	// This string is case sensitive.

	/* In SexyKanji we must use GetResourceManager() instead of mResourceManager */
	if (!GetResourceManager()->LoadResources(_S("Init")))
	{
		mLoadingFailed = true;
		// This will display an informative error message indicating exactly
		// what went wrong in the resource loading process.
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	// Now we've loaded the resources, but we need to extract them.
	// Extraction is the phase that converts sound files to raw WAV
	// files, and sets up and initializes fonts and images.
	// The ResourceGenenerator program, when it generates C++ code for our
	// resources, also creates a function for EVERY resource group of the
	// form: Extract<GROUP>Resources, where <GROUP> is the exact name
	// of the resource group you made. In our case, we made an "Init"
	// group, so we have an ExtractInitResources method. You pass to it
	// the pointer to the resource manager. Because an error can occur
	// during this step, you should make sure to check for it.

	/* In SexyKanji we must use GetResourceManager() instead of mResourceManager */
	if (!ExtractInitResources(GetResourceManager()))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	// Now that the "Init" Resources are extracted, the Pointers are valid
	// Let's test them out by setting custom cursor images. Cursor Images
	// have the Hotspot in the direct center of the image.
	// To use custom cursors, you have to specify an image resource
	// to use. The CURSOR_ defines indicate which cursor type you
	// want to assign the image to. 
	// SEXY_CURSOR_POINTER is the normal Windows pointer.
	// SEXY_CURSOR_HAND is used when mousing over widgets (if mDoFinger is true).
	// SEXY_CURSOR_DRAGGING is used when a drag operation is happening.
	// SEXY_CURSOR_TEXT is the icon used when entering text in an edit box.

	/* In SexyKanji the Cursor defines are preceded with SEXY_ */
	SetCursorImage(SEXY_CURSOR_POINTER, IMAGE_CUSTOM_POINTER);
	SetCursorImage(SEXY_CURSOR_HAND, IMAGE_CUSTOM_HAND);
	SetCursorImage(SEXY_CURSOR_DRAGGING, IMAGE_CUSTOM_DRAGGING);
	SetCursorImage(SEXY_CURSOR_TEXT, IMAGE_CUSTOM_TEXT);
	SetCursor(SEXY_CURSOR_POINTER); // Notify the app to use the new image

	// We also need to load our title screen graphics in, since you can't 
	// display the title screen without any graphics. For an explanation of why
	// we placed this in a separate group from Init, see properties/resourcesdemo4.xml.
	// This code works exactly like the above did for the Init group.
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

	/* In SexyKanji we must use GetWidgetManager() instead of mWidgetManager */
	GetWidgetManager()->AddWidget(mTitleScreen);


	// Let's also load in some music to play. We use the mMusicInterface
	// member for all our music needs, which requires the BassMusicInterface.h
	// header to be loaded, since we use the library BASS to play our music.

	/* SexyKanji does not use BASS. In SexyKanji you need to have the 
	   MusicManager.h header loaded. */

	// We can load in WAV, OGG, or MP3 files. BASS also supports a number
	// of tracker formats, such as .it, .xm, .mod, etc. It also supports
	// a format called MO3, which is a compressed version of a tracker
	// file.
	/* In SexyKanji we can load in WAV or OGG files only. */

	// We load our file manually, we do not use the resource manager for this.
	// The first parameter is the ID to associate the song with. Just as sounds
	// have IDs, so do music tracks.
	//mMusicInterface->LoadMusic(0, "music/music.mo3");
	/* In SexyKanji we must use GetMusicManager() instead of mMusicInterface.
	   Because we added the music to the resourcedemo4.xml, we don´t need
	   to load the music manually (the Resource Manager has already loaded
	   the music with the ID MUSIC_GROOVE for us). 
	   To load the music manually, use e.g.:
		GetMusicManager()->LoadStream(0, _S("music/juniorgroove.ogg"));
	*/

	// Let's load another copy of the stream. Why? In order to fade from one
	// track to another, we need two instances of the track on different
	// channels. We load our file manually, we do not use the resource 
	// manager for this. Let's load it again with the option 
	// "forceload == true, and save the auto assigned stream ID.
	mStreamID = GetMusicManager()->AutoLoadStream(_S("music/juniorgroove.ogg"), true);

	// Note you can also load the same stream twice with the Resource Manager
	// by specifying the "forceload" modifier. 
	// Use this option "forceload == true" carefully, because streams can
	// consume a lot of memory. We are only loading this stream twice to 
	// demonstrate how to cross fade between streams (see TitleScreenIsFinished).

	// Now we need to start playing a track.
	// You can use PlayMusic to instantly play the track, or, like below,
	// you can use FadeIn to smoothly fade the song in. The first parameter
	// for both methods is the channel or song id that was used when the
	// track was first loaded (In our case, either 0 or 1 works). For both,
	// the second parameter is the offset to start playing at. Again, I just
	// happen to know that the intro song is at offset 0. For FadeIn, the
	// third parameter is how quickly to fade in, out of 1.0. The last parameter
	// for both indicates whether or not you want to loop. This is kind of weird,
	// but specify "false" to loop and "true" to not loop.
	//mMusicInterface->FadeIn(0, 0, 0.002, false);

	/* In SexyKanji this has changed. See below for the new call and parameters. */

	// You use the Music Manager's 
	// PlayStream to play the track. The first parameter is the channel 
	// or song id that was used when the track was first loaded (In our 
	// case, either 0 or 1 works). The second parameter indicates whether 
	// or not you want to loop. The third parameter is how quickly to fade 
	// in, out of 1.0.
	//GetMusicManager()->PlayStream(MUSIC_GROOVE); <-- No loop, No Fade
	//GetMusicManager()->PlayStream(MUSIC_GROOVE, true); <-- Loop, No Fade
	GetMusicManager()->PlayStream(MUSIC_GROOVE, true, 0.2f); //<-- Loop, and Fade

	// We'll cover changing the music and sound volumes in a later demo.

	// Next, we need to know how many resources there are to load.
	// This is necessary so we can display our progress bar on the title screen
	// and make it be the appropriate length. There's a variable in SexyAppBase
	// called mNumLoadingThreadTasks which holds the number of resources to
	// load in the LoadingThreadProc function. You get the number of resources
	// in a given group with a call to the resource manager's GetNumResources function
	// for each of your groups that you are going to load:
	mNumLoadingThreadTasks = GetResourceManager()->GetNumResources(_S("Game"));
}

void GameApp::LoadingThreadProc()
{
	// This time, things are different. We aren't manually loading
	// our fonts, sounds, and images. The resource manager is doing
	// it for us. For each of the groups that we want to load,
	// we first have to instruct the resource manager to begin the
	// loading phase and initialize its internal variables. 
	// We do that with the StartLoadResources method and pass in the 
	// exact string name of the group to begin loading:
	GetResourceManager()->StartLoadResources(_S("Game"));

	// Now we need to load each individual resource. We will loop,
	// calling LoadNextResource at the start. When it returns false,
	// there are no more resources to load for the current group.
	// LoadNextResource knows what group to load from because 
	// of the call to StartLoadResources above:
	while (GetResourceManager()->LoadNextResource())
	{
		// The SexyAppBase variable, mCompletedLoadingThreadTasks, indicates the
		// total number of resources that have so far been loaded. This is used
		// to tell our loading screen the % progress we've made. See TitleScreen::Draw
		// for an example of how this is used. We need to increment this value
		// ourselves everytime we load a resource:
		mCompletedLoadingThreadTasks++;

		// If there was an error loading our resource, the resource manager
		// will tell us to shut down by setting mShutdown to true. If that
		// happened, immediately abort and return:
		if (mShutdown)
			return;

		// Remember in demos 1-3 how we had the Board class call MarkDirty
		// every update? Well, the title screen doesn't need to be such a hog.
		// The title screen only needs to repaint when its progress bar changes
		// size. The progress bar only changes size when a resource gets loaded.
		// Because the game app is the only one that knows when this happens,
		// the game app will be the one to tell the title screen that it's a
		// dirty, dirty widget and that it needs a good and proper repainting.
		// You COULD make an update method for the title screen and mark dirty
		// every frame. But because this consumes more CPU time, it will take
		// longer to load our resources. And since you want the loading time
		// to be as quick as possible, you should only repaint when you need to.
		//mTitleScreen->MarkDirty();

		/* In SexyKanji you should use the individual Widget's Update method 
		   instead of this method. Otherwise you will see strange cursor draw problems. */
	}

	// Just like in our Init function, after loading resources we
	// need to extract them. Let's do that. Let's also ask the resource
	// manager if an error occurred in the above loop that we
	// didn't yet catch. We do that with the HadError method:
	if (GetResourceManager()->HadError() || !ExtractGameResources(GetResourceManager()))
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

	// When we're actually loading resources, we'll set the
	// mLoadingFailed variable to "true" if there were any problems
	// encountered along the way. If that is the case, just return
	// because we won't want the user to get to the main menu or any
	// other part of the game. We will want them to exit out.
	if (mLoadingFailed)
		return;

	// We aren't going to make and add the Board class here like we
	// did in the previous demos. Instead, since we are done loading
	// everything, we're going to tell the title screen that 
	// we're done and that it should unhide the continue link and let
	// the user enter the game.
	mTitleScreen->LoadingComplete();

	// Remember: since we didn't give our title screen an Update method,
	// this class is responsible for telling it when to repaint. If we
	// don't mark it dirty, you won't see the hyperlink widget
	// appear. So mark it dirty now:
	//mTitleScreen->MarkDirty();

	/* In SexyKanji you should use the individual Widget's Update method 
	   instead of this method. Otherwise you will see strange cursor draw problems. */
}

void GameApp::TitleScreenIsFinished()
{
	// This function is called by the title screen when the user clicks
	// on the hyperlink widget to continue. At this point, the title screen
	// has already removed itself and its widgets and we should set up our
	// Board class and begin the game. Let's also set our title screen
	// pointer to NULL, since it will be safely deleted automatically at a
	// later point, and we don't want to delete it twice.
	mTitleScreen = NULL;
	mBoard = new Board(this);

	// Now that the title screen is done, we don't need its resources
	// wasting memory. Let's delete all of its resources. We do that
	// by calling DeleteResources and specifying the exact name of the
	// resource group we want to free up:
	GetResourceManager()->DeleteResources(_S("TitleScreen"));

	// This is a very important step: Because the Board class is a widget
	// (see Board.h/.cpp for more details) we need to tell it what
	// dimensions it has and where to place it. 
	// By default a widget is invisible because its
	// width/height are 0, 0. Since the Board class is our main
	// drawing area and game logic class, we want to make it the
	// same size as the application. For this particular demo, that means
	// 800x600. We will use mWidth and mHeight though, as those were
	// already set to the proper resolution in GameApp::InitHook().
	mBoard->Resize(0, 0, mWidth, mHeight);

	// Also an important step is to add the newly created Board widget to
	// the widget manager so that it will automatically have its update, draw,
	// and input processing methods called.
	GetWidgetManager()->AddWidget(mBoard);

	// Let's fade out the intro song and fade in the main game music.
	// FadeOut works just like FadeIn did in InitHook() but with some
	// slightly different parameters. The first, is like with FadeIn and
	// PlayMusic, the channel or song id that you want to mess with.
	// The second indicates that the song fading out should stop when
	// done, if it is true. The final parameter indicates how fast
	// to fade out, and is from 0 to 1.
	//mMusicInterface->FadeOut(0, true, 0.004);

	/* In SexyKanji this has changed. We use the Music Manager's. 
	   FadeOutStream() and the track is always stopped when done fading. */
	GetMusicManager()->FadeOutStream(MUSIC_GROOVE, 0.4f);

	// Let's fade in the main game music. This is the same as in InitHook().
	// The only difference is we're using mStreamID instead of MUSIC_GROOVE
	// for our stream ID.
	// Why? Well, channel/song ID MUSIC_GROOVE is being used to fade out the 
	// previously playing track, we can't use it to also fade in.
	// That's why we loaded another copy of the song with the ID mStreamID.
	GetMusicManager()->PlayStream(mStreamID, true, 0.2f);

	// We'll cover changing the music and sound volumes in a later demo.
}

void GameApp::ShutdownHook()
{
	// Remove our "Board" class which was, in this particular demo,
	// responsible for all our game drawing and updating.
	// All widgets MUST be removed from the widget manager before deletion.
	// More information on the basics of widgets can be found in the Board
	// class file. If you tried to delete the Board widget before removing
	// it, you will get an assert. Because our board might not have been
	// added (if you shut down the app before closing the loading screen),
	// only remove it if it isn't null.
	if (mBoard != NULL)
		GetWidgetManager()->RemoveWidget(mBoard);

	// Take a look at TitleScreen::ButtonDepress if you haven't already.
	// It explains a function called SafeDeleteWidget. Notice that we're
	// directly deleting the widget here: that is because when our app's
	// destructor is called, it's at the very end of the shutdown sequence
	// and the safe delete widget list will NOT be processed. Thus we
	// have to delete the memory manually.
	delete mBoard;

	// If you shut down the app before closing the loading screen, then
	// it will need to be removed here. The rational for the next two
	// steps is the same as for Board:
	if (mTitleScreen != NULL)
		GetWidgetManager()->RemoveWidget(mTitleScreen);
	delete mTitleScreen;

	// We should also free up all the resources that we loaded
	// for ALL the resource groups. Deleting a group that was
	// already deleted doesn't do anything, it's ignored.
	GetResourceManager()->DeleteResources(_S("Init"));
	GetResourceManager()->DeleteResources(_S("TitleScreen"));
	GetResourceManager()->DeleteResources(_S("Game"));
}
