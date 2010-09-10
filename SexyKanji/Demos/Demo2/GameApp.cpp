#include "GameApp.h"
#include "Board.h"
#include "WidgetManager.h"

//#include "SexyAppFramework/ImageFont.h"
/* This is a change in SexyKanji. All Font calls are now centralized in 
   SexyFont. Font, ImageFont, and SysFont no longer exist. */
#include "SexyFont.h"

//#include "SexyAppFramework/DDImage.h"
/* This is a change in SexyKanji. All images are now SexyImages. Image, 
   MemoryImage and DDImage no longer exist. */
#include "SexyImage.h"
/* In SexyKanji you must use the ImageManger to load and access images. */
#include "ImageManager.h"

// This will let us load sounds
#include "SoundManager.h"

// To load and play movies
#include "SexyMovie.h"

// The SexyKanjiFramework resides in the "Sexy" namespace. As a convenience,
// you'll see in all the .cpp files "using namespace Sexy" to avoid
// having to prefix everything with Sexy::
using namespace Sexy;

GameApp::GameApp()
{
	// mProdName is used for internal purposes to indicate the game that we're working on
	/* In SexyKanji mProdName has been removed as unnecessary */
	//mProdName = "Demo 2";

	// For internal uses, indicates the current product version
	//mProductVersion = "1.0";
	/* In SexyKanji mProductVersion has been renamed to mVersion */
	mVersion = _S("1.0"); // SexyKanji uses wide strings

	// This is the text that appears in the title bar of the application window
	//mTitle = StringToSexyStringFast("SexyAppFramework: " + mProdName + " - " + mProductVersion);
	/* In SexyKanji we'll use SetTitle() */
	SetTitle(_S("SexyKanji: Demo 2"));

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

	mBoard = NULL;

	mTextFont = NULL;
	mNumberFont = NULL;
	mTurbotImg = NULL;
	mMoonImg = NULL;
	mOpaqueBeamImg = NULL;
	mKanjiLogo = NULL;
	mFishMovie = NULL;
}

GameApp::~GameApp()
{
	/* In SexyKanji you should clean up in ShutdownHook() instead of
	   waiting until the destructor is called. */
}

void Sexy::GameApp::InitHook()
{
	// In later demos, you will see more done with this function.
	// For now, we have nothing else to initialize, so we are done.
	// Once complete, the LoadingThreadProc function will automatically
	// start and we will begin loading all our needed resources.
}

void GameApp::LoadingThreadProc()
{
	// This time, we have things to load. Let's load in our two fonts 
	// and our three images. Besides loading data, this thread can 
	// also update the progress indicator for the loading screen, 
	// which you will see in later demos. Once complete, the 
	// LoadingThreadCompleted function will be called.

	// Loading images is easy: you don't have to specify the image type.
	// Depending on the file extension, the appropriate image decoder
	// will be used. The following image types are supported:
	// Targa (.tga), JPEG (.jpg), PNG (.png), GIF (.gif).

	/* This is a change in SexyKanji. GIF is no longer supported. So the 
	   file types are: */
	// Targa (.tga), JPEG (.jpg), PNG (.png), BMP (.bmp).

	// You do NOT have to specify the extension when loading the file 
	// if you don't want to. In this case, all of the above extensions 
	// will be looked for. A discussion of image formats is beyond the 
	// scope of this tutorial.

	// There is some important information to know about images.
	// You will notice in the "images" directory that for each image, 
	// there is a black and white image with the same name but with
	// an underscore ("_") at the end of it. By default, when you load
	// an image, the code automatically looks for the presence of
	// that file to use for the alpha information. Some file formats
	// have the alpha channel built into them, like PNG files. But
	// others, like JPEG files, do not. The purpose of the alpha
	// file is of course to generate an image that doesn't have jagged
	// lines, or to control the opacity of various parts of the image.
	// As a side not, the alpha image file may also begin with the
	// underscore instead of ending with it, it matters not, and again,
	// is automatically loaded in by the image loading code.
	// You need to clean up the memory allocated by these functions yourself.

	//mOpaqueBeamImg = (DDImage*) GetImage("images/beam_opaque");
	/* In SexyKanji you must use the ImageManager to load your images.
	   Notice that we now use Wide strings. Again, all images are now SexyImages
	   so there is no need to cast to anything different (DDImage). */
	mOpaqueBeamImg = GetImageManager()->GetImage(_S("images/beam_opaque"));
	mKanjiLogo = GetImageManager()->GetImage(_S("images/kanji_logo"));

	// If the file was not found or couldn't be loaded (i.e. due to an
	// incompatible file format) the returned value will be NULL.
	// You should always check this, and if it occurs, display an error
	// message, then set mLoadingFailed to true, and then immediately return.
	if (mOpaqueBeamImg == NULL)
	{
		// The PopUp method displays a standard Windows message box.
		// If in full screen mode, this will appropriately handle things such
		// that the GDI surface is properly rendered and the dialog box appears
		// as expected.
		//Popup("There was an error loading the file: images/beam_opaque");

		/* In SexyKanji use KMiscTools::messageBox() instead of Popup(). */
		KMiscTools::messageBox("Example error", "There was an error loading the file: images/beam_opaque");
		mLoadingFailed = true;
		return;
	}

	// Now load the other two images
	mMoonImg = GetImageManager()->GetImage(_S("images/moon"));
	if (mMoonImg == NULL)
	{
		KMiscTools::messageBox("Example error", "There was an error loading the file: images/moon");
		mLoadingFailed = true;
		return;
	}

	mTurbotImg = GetImageManager()->GetImage(_S("images/turbot_worry"));
	if (mTurbotImg == NULL)
	{
		KMiscTools::messageBox("Example error", "There was an error loading the file: images/turbot_worry");
		mLoadingFailed = true;
		return;
	}

	/* SexyKanji does not support Palletize() */
	//((DDImage*)mOpaqueBeamImg)->Palletize();
	//((DDImage*)mMoonImg)->Palletize();
	//((DDImage*)mTurbotImg)->Palletize();


	/* In SexyKanji the font text data file has been replaced with an
	   XML data file. You must use the new SexyKanji Font Builder to 
	   create fonts for use with SexyKanji. */

	// Now let's load and create some fonts. A font consists of an 
	// image and a xml file. The image works on the same principles
	// as a normal image file (like the ones above) works on. More 
	// information on colorizing fonts and images can
	// be found in Board.cpp, but for now know that the image in the fonts
	// directory contains the alpha information for your font, and that it
	// is assumed that the "main" image is pure white.
	// The other file in the directory is an xml file, commonly with the
	// same name as the image. The file will define the name of the image to load.
	// This file defines the characters, their widths, their offsets
	// within the image, the point size, and any layers (which are not
	// used or discussed in this demo). This is the data file and is
	// how the font knows how to take a string and convert it into
	// the proper images representing each character. A font is really
	// just an image but with an extra data file that tells the program
	// how to map strings to their image representation.
	// You load a font by specifying the xml data file.

	mTextFont = new SexyFont() ;

	// We need to check to see if the font was properly initialized.
	// If it wasn't, then an error occurred and we need to abort.
	if (!mTextFont->OpenFontFile(_S("fonts/Huxtable20.xml")))
	{
		delete mTextFont;
		//Popup("There was an error loading fonts/Kiloton9.txt");
		KMiscTools::messageBox("Example error", "Couldn't load fonts/A.C.M.E.SecretAgent20.xml");
		mLoadingFailed = true;
		return;
	}

	mNumberFont = new SexyFont() ;
	if (!mNumberFont->OpenFontFile(_S("fonts/PrimerApples24.xml")))
	{
		delete mNumberFont;
		//Popup("There was an error loading fonts/supernova20.txt");
		KMiscTools::messageBox("Example error", "Couldn't load fonts/PrimerApples24.xml");
		mLoadingFailed = true;
		return;
	}

	// load a movie
	mFishMovie = new SexyMovie();
	if (!mFishMovie->OpenFile(_S("videos/video.ogv"), true))
	{
		delete mFishMovie;
		KMiscTools::messageBox("Example error", "Couldn't load movie videos/video.ogv");
		mLoadingFailed = true;
		return;
	}

	// Let's load some sounds. You assign a unique unsigned integer ID to each
	// sound. It is with this ID that you indicate which sound you want to play. 
	// Valid types of sounds to load are: 
	// WAV, OGG, AU, and if you have FMod and enable FMod: MP3. Although

	/* This is a change in SexyKanji. AU and MP3 are no longer supported.
	   Valid types of sounds to load are WAV and OGG. */

	// LoadSample returns a boolean indicating success or failure.
	if (!GetSoundManager()->LoadSample(1, _S("sounds/timer.ogg")))
	{
		//Popup("There was an error loading sounds/timer");
		KMiscTools::messageBox("Example error", "There was an error loading sounds/timer");
		mLoadingFailed = true;		
		return;
	}

	if (!GetSoundManager()->LoadSample(2, _S("sounds/mutator.ogg")))
	{
		//Popup("There was an error loading sounds/mutator");
		KMiscTools::messageBox("Example error", "There was an error loading sounds/mutator.ogg");
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

	// Now that we're done loading everything we need (which wasn't
	// anything in this particular demo), we need to get the main
	// game screen up and running: That is our "Board" class, and
	// it will handle all the drawing, updating, and input processing
	// for most of the game.
	mBoard = new Board(this);

	// This is a very important step: Because the Board class is a widget
	// (see Board.h/.cpp for more details) we need to tell it what
	// dimensions it has and where to place it. By default a widget is 
	// invisible because its width/height are 0, 0. Since the Board class 
	// is our main drawing area and game logic class, we want to make it the
	// same size as the application. For this particular demo, that means
	// 800x600. We will use mWidth and mHeight though, as those were
	// already set to the proper resolution in GameApp::Init().
	//mBoard->Resize(0, 0, mWidth, mHeight);

	/* In SexyKanji we will use GetWidth() and GetHeight() */
	mBoard->Resize(0, 0, GetWidth(), GetHeight());

	// Also an important step is to add the newly created Board widget to
	// the widget manager so that it will automatically have its update, draw,
	// and input processing methods called.
	//mWidgetManager->AddWidget(mBoard);

	/* In SexyKanji we must use GetWidgetManager() instead of mWidgetManager */
	GetWidgetManager()->AddWidget(mBoard);

	// And just to test out our sound playing abilities, let's play the
	// mutator sound to indicate that we're done loading.
	// We do that by calling GameApp's PlaySample()
	// method and specifying the integral ID of the sound to play. This
	// ID is the same as we used when loading the sound in GameApp::LoadingThreadProc()
	//PlaySample(2);

	/* In SexyKanji we must use GetSoundManager() to play sounds */
	GetSoundManager()->PlaySample(2);
}

void GameApp::ShutdownHook()
{
	// Remove our "Board" class which was, in this particular demo,
	// responsible for all our game drawing and updating.
	// All widgets MUST be removed from the widget manager before deletion.
	// More information on the basics of widgets can be found in the Board
	// class file. If you tried to delete the Board widget before removing
	// it, you will get an assert.
	GetWidgetManager()->RemoveWidget(mBoard);
	delete mBoard;

	// We need to clean up after ourselves and delete the image and
	// font information.
	delete mTextFont;
	delete mNumberFont;
	delete mTurbotImg;
	delete mMoonImg;
	delete mOpaqueBeamImg;
	delete mKanjiLogo;
	delete mFishMovie;

	// We need to release the memory allocated for our sounds too.
	// This call frees up the memory for ALL sound effects.
	GetSoundManager()->ReleaseSamples();
}
