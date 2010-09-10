#include "GameApp.h"

#include "Board.h"
#include "DemoDialog.h"
#include "TitleScreen.h"

#include "Checkbox.h"
#include "Dialog.h"
#include "WidgetManager.h"

// We will be accessing the resource manager in this demo, so include it's header
#include "ResourceManager.h"

// Required for playing music
#include "MusicManager.h"

// Contains all the resources from the resourcesdemo5.xml file in our
// properties directory. See that file for more information.
#include "Res.h"

// The SexyAppFramework resides in the "Sexy" namespace. As a convenience,
// you'll see in all the .cpp files "using namespace Sexy" to avoid
// having to prefix everything with Sexy::
using namespace Sexy;

GameApp::GameApp()
{
	// This is the text that appears in the title bar of the application window
	SetTitle(_S("SexyKanji: Demo5"));

	// Set the application width/height in terms of pixels here.
	SetDimensions(640, 480);

	// See Board::UpdateF for a very lengthy explanation of this and smooth motion
	EnableVSync();

	mBoard = NULL;
	mTitleScreen = NULL;

	//mIsWindowed = false;
}

GameApp::~GameApp()
{
	// We're freeing and deleting in ShutDownHook().
}

void GameApp::InitHook()
{
	// We need to tell the resource manager to read in all the groups
	// and information from that main group we made, called ResourceManifest,
	// in the file "properties/resources.xml". The path/filename are
	// by default set up to load that file, so you must name it exactly as such.
	// This doesn't load any resources: it just parses the data and sets
	// things up for loading.

	// This loads "DEFAULT_RESOURCE_PATH" which is defined as 
	// "properties/resources.xml" in SexyConfig.h:
	// GetResourceManager()->LoadResourceManifest(); 

	// This is how you would specify a different 'resource.xml' other than
	// "DEFAULT_RESOURCE_PATH" as defined in SexyConfig.h:
	GetResourceManager()->ParseResourcesFile(_S("properties/resourcesdemo5.xml"));

	// Next, we want to load our absolutely necessary files that have to
	// be loaded before anything else can run. You'll notice in the resources.xml
	// file that we created a group called Init that contains these resources.
	// You may call it whatever you like. Let's load those resources now.
	// We do that by calling the LoadResources method of our ResourceManager
	// and specifying in quotes the name of the resource group to 
	// load. This string is case sensitive.
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
	// The ResourceGen.exe program, when it generates C++ code for our
	// resources, also creates a function for EVERY resource group of the
	// form: Extract<GROUP>Resources, where <GROUP> is the exact name
	// of the resource group you made. In our case, we made an "Init"
	// group, so we have an ExtractInitResources method. You pass to it
	// the pointer to the resource manager. Because an error can occur
	// during this step, you should make sure to check for it.
	if (!ExtractInitResources(GetResourceManager()))
	{
		mLoadingFailed = true;
		GetResourceManager()->ShowResourceError(true);
		Shutdown();
		return;
	}

	// Now that the "Init" Resources are extracted, the Pointers are valid
	// Let's test them out by setting custom cursor images.  Cursor Images
	// have the Hotspot in the direct center of the image.
	SetCursorImage(SEXY_CURSOR_POINTER, IMAGE_CUSTOM_POINTER);
	SetCursorImage(SEXY_CURSOR_HAND, IMAGE_CUSTOM_HAND);
	SetCursorImage(SEXY_CURSOR_DRAGGING, IMAGE_CUSTOM_DRAGGING);
	SetCursorImage(SEXY_CURSOR_TEXT, IMAGE_CUSTOM_TEXT);
	SetCursor(SEXY_CURSOR_POINTER); // Notify the app to use the new image

	// We also need to load our title screen graphics in, since you can't 
	// display the title screen without any graphics. For an explanation of why
	// we placed this in a separate group from Init, see properties/resourcesdemo5.xml.
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
	GetWidgetManager()->AddWidget(mTitleScreen);

	// Let's also load in some music to play. We use the Music Manager
	// for all our music needs. We can load in WAV or OGG files.
	// We load our file manually, we do not use the resource manager for this.
	// The first parameter is the ID to associate the song with. Just as sounds
	// have IDs, so do music tracks.
	GetMusicManager()->LoadStream(0, _S("music/juniorgroove.ogg"));

	// Now we need to start playing a track. You use the Music Manager's 
	// PlayStream to play the track. The first parameter is the channel 
	// or song id that was used when the track was first loaded. 
	// The second parameter indicates whether or not you want to loop. 
	// The third parameter is how quickly to fade in, out of 1.0.
	GetMusicManager()->PlayStream(0, true, 0.2f);

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
}

void GameApp::TitleScreenIsFinished()
{
	mTitleScreen = NULL;
	mBoard = new Board(this);

	// Now that the title screen is done, we don't need its resources
	// wasting memory. Let's delete all of its resources. We do that
	// by calling DeleteResources and specifying the exact name of the
	// resource group we want to free up:
	GetResourceManager()->DeleteResources(_S("TitleScreen"));

	mBoard->Resize(0, 0, mWidth, mHeight);
	GetWidgetManager()->AddWidget(mBoard);

	// This is a new step: We're going to tell the WidgetManager
	// that keyboard input and mouse wheel notifications should go to
	// the board object. This way, we'll be able to respond to keypresses:
	GetWidgetManager()->SetFocus(mBoard);
}

void GameApp::SetWindowed(bool wantWindowed)
{
	// Let the app handle the actual details of this call...
	SexyAppBase::SetWindowed(wantWindowed);

	// We can see if the options dialog is up with a call to
	// GetDialog. You pass GetDialog the unique ID of the dialog box,
	// and if it exists it is returned to you, otherwise NULL is returned.
	DemoDialog* d = (DemoDialog*) GetWidgetManager()->GetDialog(DemoDialog::DIALOG_ID);

	// Set the checkbox state to our windowed state
	if ((d != NULL) && (d->mFSCheckbox != NULL))
		d->mFSCheckbox->SetChecked(!wantWindowed);

}

void GameApp::ButtonPress(int theId)
{
	if (theId == DemoDialog::MESSAGE_BOX_ID + 2000)
		GetWidgetManager()->KillDialog(theId - 2000);
}

void GameApp::SetFocusToBoard()
{
	if (mBoard != NULL)
		GetWidgetManager()->SetFocus(mBoard);
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
	// GetResourceManager()->DeleteResources(_S("Init"));
	// GetResourceManager()->DeleteResources(_S("TitleScreen"));
	// GetResourceManager()->DeleteResources(_S("Game"));
	
	// You could also delete all groups by passing an empty string:
	GetResourceManager()->DeleteResources(_S(""));
}
