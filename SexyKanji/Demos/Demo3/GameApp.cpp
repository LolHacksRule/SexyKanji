#include "GameApp.h"
#include "Board.h"
#include "WidgetManager.h"

//#include "SexyAppFramework/DDImage.h"
/* This is a change in SexyKanji. All images are now SexyImages. Image, 
   MemoryImage and DDImage no longer exist. */
#include "SexyImage.h"
/* In SexyKanji you must use the ImageManger to load and access images. */
#include "ImageManager.h"

// We're going to create a font for Board's button widget's label
//#include "SexyAppFramework/ImageFont.h"
/* This is a change in SexyKanji. All Font calls are now centralized in 
   SexyFont. Font, ImageFont, and SysFont no longer exist. */
#include "SexyFont.h"

// The SexyKanjiFramework resides in the "Sexy" namespace. As a convenience,
// you'll see in all the .cpp files "using namespace Sexy" to avoid
// having to prefix everything with Sexy::
using namespace Sexy;

GameApp::GameApp()
{
	// mProdName is used for internal purposes to indicate the game that we're working on
	/* In SexyKanji mProdName has been removed as unnecessary */
	//mProdName = "Demo 3";

	// For internal uses, indicates the current product version
	//mProductVersion = "1.0";
	/* In SexyKanji mProductVersion has been renamed to mVersion */
	mVersion = _S("1.0"); // SexyKanji uses wide strings

	// This is the text that appears in the title bar of the application window
	//mTitle = StringToSexyStringFast("SexyAppFramework: " + mProdName + " - " + mProductVersion);
	/* In SexyKanji we'll use SetTitle() */
	SetTitle(_S("SexyKanji: Demo 3"));

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
	mKanjiLogo= NULL;
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

	//mTurbotImg = (DDImage*) GetImage("images/turbot_worry");
	/* In SexyKanji you must use the ImageManager to load your images.
	   Notice that we now use Wide strings. Again, all images are now SexyImages
	   so there is no need to cast to anything different (DDImage). */
	mTurbotImg = GetImageManager()->GetImage(_S("images/turbot_worry"));
	mKanjiLogo = GetImageManager()->GetImage(_S("images/kanji_logo"));


	// If the file was not found or couldn't be loaded (i.e. due to an
	// incompatible file format) the returned value will be NULL.
	// You should always check this, and if it occurs, display an error
	// message, then set mLoadingFailed to true, and then immediately return.
	if (mTurbotImg == NULL)
	{
		// The messageBox method displays a standard Windows message box.
		// If in full screen mode, this will appropriately handle things such
		// that the GDI surface is properly rendered and the dialog box appears
		// as expected.
		//Popup("There was an error loading the file: images/turbot_worry");

		/* In SexyKanji use KMiscTools::messageBox() instead of Popup(). */
		KMiscTools::messageBox("Example error", "There was an error loading the file: images/turbot_worry");
		mLoadingFailed = true;
		return;
	}

	mLightningImg = GetImageManager()->GetImage(_S("images/lightning.jpg"));
	if (mLightningImg == NULL)
	{
		KMiscTools::messageBox("Example error", "There was an error loading the file: images/lightning");
		mLoadingFailed = true;
		return;
	}

	// There's something different about mLightningImg. If you look at the
	// image in the iamges directory, you'll notice that it has multiple frames
	// arranged in rows. This is an animation strip and is the main way that we
	// accomplish animation. Rather than putting each frame in a separate file,
	// we place it in one file and make each frame have the same width/height.
	// When loading, the image loader has no idea that we even want to consider
	// the image as being made of multiple frames. So we have to manually tell
	// it how many rows and/or columns it has. In this case, there is only 1
	// column, but there are 8 rows. We set this via SetRows and SetColumns.
	// You'll see why this is important in the Board class.
	mLightningImg->SetRows(8);
	mLightningImg->SetColumns(1);

	/* SexyKanji does not support Palletize() */
	//mLightningImg->Palletize();
	//mTurbotImg->Palletize();

	// This is the same as in Demo2. Check the comments there
	// if you've forgotten how to load and initialize a font.
	mFont = new SexyFont();

	// We need to check to see if the font was properly initialized.
	// If it wasn't, then an error occurred and we need to abort.
	if (!mFont->OpenFontFile(_S("fonts/Huxtable20.xml")))
	{
		delete mFont;
		mLoadingFailed = true;
		//Popup("There was an error loading fonts/Kiloton9.txt");
		KMiscTools::messageBox("Example error", "Couldn't load fonts/Huxtable20.xml");
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
	// 800x600.
	//mBoard->Resize(0, 0, GetWidth(), GetHeight());

	/* In SexyKanji we will use GetWidth() and GetHeight() */
	mBoard->Resize(0, 0, GetWidth(), GetHeight());

	// Also an important step is to add the newly created Board widget to
	// the widget manager so that it will automatically have its update, draw,
	// and input processing methods called.
	//mWidgetManager->AddWidget(mBoard);

	/* In SexyKanji we must use GetWidgetManager() instead of mWidgetManager */
	GetWidgetManager()->AddWidget(mBoard);

	// As part of a new topic in this demo, we're also going to modify the image
	// data for mTurbotImg and make a grayscale version of it. I'll explain
	// each step of the way:

	// 1. Let's make a copy of the image so we don't ruin the original.
	// We should make sure to delete this when we're done.

	/* In SexyKanji you must use the ImageManager to load your images.
	   Again, all images are now SexyImages so there is no need to cast 
	   to anything different (DDImage). */
	mAlteredImg = GetImageManager()->CopyImage(mTurbotImg);

	// 2. Now we need to get the pixel data. The pixel data is stored as
	// an unsigned long array, where each entry represents the RGBA value.
	// The data is actually stored in ARGB format, where alpha is
	// the leftmost byte and blue is the rightmost byte.
	unsigned long* bits = mAlteredImg->GetBits();

	// 3. Now we will loop over each pixel in the image. The size of the bits array
	// is simply the width times the height.
	for (int i = 0; i < mAlteredImg->GetWidth() * mAlteredImg->GetHeight(); i++)
	{
		// 4. Get the ARGB color value for this pixel
		unsigned long c = bits[i];

		// 5. To illustrate the ARGB storage format, we will assign each
		// component to a variable, although we're actually only going to care
		// about the RGB values, for this particular example. The 4 lines below
		// extract out the individual ARGB values.
		unsigned char alpha		= (unsigned char) (c >> 24);
		unsigned char red		= (unsigned char) ((c >> 16) & 0xFF);
		unsigned char green		= (unsigned char) ((c >> 8) & 0xFF);
		unsigned char blue		= (unsigned char) (c & 0xFF);

		// 6. Just like the Color class, the ARGB values are from 0-255.
		// Let's alter these to produce a grayscale image using one of many
		// conversion methods. This method uses 30% of the red value,
		// 59% of the green value, and 11% of the blue value:
		unsigned long gray = (unsigned long) ((float)red * 0.30f + (float)green * 0.59f + (float)blue * 0.11f);

		// 7. Now we need to put the pixel data back into the image's data.
		// We do the opposite of how we extracted the ARGB values above and
		// use a left shift instead of a right shift:

		//		      alpha          red           green       blue
		bits[i] = (alpha << 24) | (gray << 16) | (gray << 8) | gray;
	}

	// The image won't use this modified data until we inform it that we've 
	// done some messing around with it. We do that with the CommitBits()
	// function call. After that, we're all done! Pretty simple. It just
	// depends on what you want to actually do with the RGBA data. Extracting
	// the information and putting it back is as simple as a few shifts.
	//mAlteredImg->BitsChanged();

	/* In SexyKanji you call CommitBits(). */
	mAlteredImg->CommitBits();
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
	delete mTurbotImg;
	delete mLightningImg;
	delete mAlteredImg;
	delete mFont;
	delete mKanjiLogo;
}
