// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	created:	2010/02/17
	created:	17:2:2010   23:17
	filename: 	SexyKanji\Image.cpp
	file base:	Image
	file ext:	cpp
	author:		Jeremy Sullivan
	
	purpose:	SexyAppFramework Wrapper for Kanji Graphic
*********************************************************************/
#include "SexyImage.h"

#include "KPTK.h"
#include "Common.h"
#include "Debug.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		Default CTor for SexyImage.
// \details 	<b>Description:</b> Default CTor for SexyImage.
//************************************/
SexyImage::SexyImage() 
{
	mFilePath = _S("");
	mAlphaFilePath = _S("");
	mWidth = 0;
	mHeight = 0;
	mNumRows = 1;
	mNumCols = 1;
    mNumTotal = 1;
	mLoaded = false;
	mUploaded = false;
	mCompressedDataBuffer = NULL;
	mCompressedDataSize = 0;
	mAlphaCompressedDataBuffer = NULL;
	mAlphaCompressedDataSize = 0;
	mKGraphicImage = KPTK::createKGraphic();
	mBits = NULL;
	mBitsDirty = false;
	mPurgeBits = true;
	mNoBlendOverride = false;
	mWrappingMode = K_WRAP_CLAMP;
}

/*!***********************************
// @return    	
// @param     	theImage
//
// \brief		Copy CTor for Sexy Image
// \details 	<b>Description:</b> Copy CTor for Sexy Image
//************************************/
SexyImage::SexyImage(const SexyImage& theImage) :
mFilePath(theImage.mFilePath),
mAlphaFilePath(theImage.mAlphaFilePath),
mWidth(theImage.mWidth),
mHeight(theImage.mHeight),
mNumRows(theImage.mNumRows),
mNumCols(theImage.mNumCols),
mNumTotal(theImage.mNumTotal),
mLoaded(false),
mUploaded(false),
mCompressedDataBuffer(NULL),
mCompressedDataSize(theImage.mCompressedDataSize),
mAlphaCompressedDataBuffer(NULL),
mAlphaCompressedDataSize(theImage.mAlphaCompressedDataSize),
mKGraphicImage(KPTK::createKGraphic()),
mBits(NULL),
mBitsDirty(theImage.mBitsDirty),
mPurgeBits(theImage.mPurgeBits),
mNoBlendOverride(theImage.mNoBlendOverride),
mWrappingMode(theImage.mWrappingMode)
{
	// Copy the data buffer if it exists.
	if (theImage.mCompressedDataBuffer && theImage.mCompressedDataSize)
	{
		mCompressedDataBuffer = new unsigned char[mCompressedDataSize];
		memcpy(mCompressedDataBuffer, theImage.mCompressedDataBuffer, mCompressedDataSize);
	}

	// Copy the alpha data buffer if it exists.
	if (theImage.mAlphaCompressedDataBuffer && theImage.mAlphaCompressedDataSize)
	{
		mAlphaCompressedDataBuffer = new unsigned char[mAlphaCompressedDataSize];
		memcpy(mAlphaCompressedDataBuffer, theImage.mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);
	}

	// Copy the bits if they exist.
	if (theImage.mBits)
	{
		int aSize = theImage.mWidth * theImage.mHeight;
		mBits = new unsigned long [aSize];
		memcpy(mBits, theImage.mBits, aSize * sizeof(unsigned long));
	}

	bool success = false;
	// First try to load the mKGraphicImage with the image data.
	mLoaded = success = CreateKGraphicFromData(LOAD_STEP_LOAD_ONLY);
	if (!success)
	{
		// If that failed, try with the bits.
		success = CreateKGraphicFromBits();
	}

	if (success)
	{
		mKGraphicImage->setTextureQuality(true);
		mWidth = static_cast<int>(mKGraphicImage->getWidth());
		mHeight = static_cast<int>(mKGraphicImage->getHeight());
	}

	mAnimInfo = theImage.mAnimInfo;

	mKGraphicImage->allowTextureWrap(mWrappingMode);
}

/*!***********************************
// @return    	
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Ctor for Sexy Image that loads the parameters.
// \details 	<b>Description:</b> Ctor for Sexy Image that loads the parameters.
//************************************/
SexyImage::SexyImage(std::wstring theFileName, bool lookForAlphaFile) :
mFilePath(WStringToSexyStringFast(theFileName)),
mAlphaFilePath(_S("")),
mWidth(0),
mHeight(0),
mNumRows(0),
mNumCols(0),
mLoaded(false),
mUploaded(false),
mCompressedDataBuffer(NULL),
mCompressedDataSize(0),
mAlphaCompressedDataBuffer(NULL),
mAlphaCompressedDataSize(0),
mKGraphicImage(KPTK::createKGraphic()),
mBits(NULL),
mBitsDirty(false),
mPurgeBits(true),
mNoBlendOverride(false),
mWrappingMode(K_WRAP_CLAMP)
{
	OpenFile(theFileName, lookForAlphaFile);
}

/*!***********************************
// @return    	
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Ctor for Sexy Image that loads the parameters.
// \details 	<b>Description:</b> Ctor for Sexy Image that loads the parameters.
//************************************/
Sexy::SexyImage::SexyImage( std::string theFileName, bool lookForAlphaFile /*= true*/ ):
mFilePath(StringToSexyStringFast(theFileName)),
mAlphaFilePath(_S("")),
mWidth(0),
mHeight(0),
mNumRows(0),
mNumCols(0),
mLoaded(false),
mUploaded(false),
mCompressedDataBuffer(NULL),
mCompressedDataSize(0),
mAlphaCompressedDataBuffer(NULL),
mAlphaCompressedDataSize(0),
mKGraphicImage(KPTK::createKGraphic()),
mBits(NULL),
mBitsDirty(false),
mPurgeBits(true),
mNoBlendOverride(false),
mWrappingMode(K_WRAP_CLAMP)
{
	OpenFile(theFileName, lookForAlphaFile);
}

/*!***********************************
// @return    	
//
// \brief		Destructor
// \details 	<b>Description:</b> Destructor
//************************************/
SexyImage::~SexyImage()
{
	if (mCompressedDataBuffer)
		delete [] mCompressedDataBuffer;
	mCompressedDataBuffer = NULL;

	if (mKGraphicImage)
		delete mKGraphicImage;
	mKGraphicImage = NULL;

	if (mAlphaCompressedDataBuffer)
		delete [] mAlphaCompressedDataBuffer;
	mAlphaCompressedDataBuffer = NULL;

	if (mBits)
		delete [] mBits;
	mBits = NULL;

}

/*!***********************************
// @return    	void
// @param     	theWidth
// @param     	theHeight
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyImage::Create(int theWidth, int theHeight)
{
	delete [] mBits;
	mBits = NULL;

	mWidth = theWidth;
	mHeight = theHeight;

	CommitBits();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyImage::Clear()
{
	unsigned long* aBits = GetBits();
	if (aBits == NULL)
	{
		mBits = new unsigned long [mWidth * mHeight];
		aBits = mBits;
	}

	int aSize = mWidth * mHeight;
	for (int i = 0; i < aSize; i++)
	{
		*aBits++ = 0;
	}

	CommitBits();
}

/*!***********************************
// @return    	void
// @param     	theImageFilePath
//
// \brief		Load the Image specified as the alpha channel of this Graphic.
// \details 	<b>Description:</b> Load the Image specified as the alpha channel of this Graphic.
//************************************/
void SexyImage::LoadAlphaMask(std::wstring theImageFilePath)
{
	// Prerelease any previous data. In case the Alpha channel is automatically loaded and then a 
	// different alpha channel is specified (JPOAG)
	delete [] mAlphaCompressedDataBuffer;
	mAlphaCompressedDataBuffer = NULL;
	mAlphaCompressedDataSize = 0;

	std::wstring aFileDir = GetFileDir(theImageFilePath, true);
	std::wstring aFileName =  GetFileName(theImageFilePath, true);

	std::wstring anAlphaImageFilePath;
	bool bSuccess = false;

	std::vector<std::string> anExtQueue;
	anExtQueue.push_back(".j2k");anExtQueue.push_back(".jp2");anExtQueue.push_back(".jpf");
	anExtQueue.push_back(".bmp");anExtQueue.push_back(".tga");anExtQueue.push_back(".png");
	anExtQueue.push_back(".jpg");anExtQueue.push_back(".gif");

	while (!bSuccess && !anExtQueue.empty())
	{
		// Make _ImageName path.
		anAlphaImageFilePath = aFileDir + L"_" + aFileName + StringToWString(anExtQueue.back());
		anExtQueue.pop_back();

		bSuccess = KResource::loadResource(WStringToString(anAlphaImageFilePath).c_str(), 
			mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);
	}

	// reload
	if(!bSuccess)
	{
		anExtQueue.push_back(".j2k");anExtQueue.push_back(".jp2");anExtQueue.push_back(".jpf");
		anExtQueue.push_back(".bmp");anExtQueue.push_back(".tga");anExtQueue.push_back(".png");
		anExtQueue.push_back(".jpg");anExtQueue.push_back(".gif");
	}

	while (!bSuccess && !anExtQueue.empty())
	{
		// If failed above try ImageName_
		anAlphaImageFilePath = aFileDir + aFileName + L"_" + StringToWString(anExtQueue.back());
		anExtQueue.pop_back();

		bSuccess = KResource::loadResource(WStringToString(anAlphaImageFilePath).c_str(), 
			mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);
	}

	if(bSuccess) 
		mAlphaFilePath = WStringToSexyStringFast(anAlphaImageFilePath);
}

/*!***********************************
// @return    	void
// @param     	theImageFilePath
//
// \brief		Load the Image specified as the alpha channel of this Graphic.
// \details 	<b>Description:</b> Load the Image specified as the alpha channel of this Graphic.
//************************************/
void Sexy::SexyImage::LoadAlphaMask( std::string theImageFilePath )
{
	LoadAlphaMask(StringToWString((theImageFilePath)));
}

/*!***********************************
// @return    	bool
// @param     	theStep { LOAD_STEP_LOAD_UPLOAD, LOAD_STEP_LOAD_ONLY, LOAD_STEP_UPLOAD_ONLY}
//
// \brief		Load or upload the Image data to the KGraphic
// \details 	<b>Description:</b> Load or upload the Image data to the KGraphic
//************************************/
bool SexyImage::CreateKGraphicFromData(int theStep)
{
	// Convert wstring to string.
	std::string aFilePath = SexyStringToStringFast(mFilePath).c_str();
	std::string anAlphaFilePath = SexyStringToStringFast(mAlphaFilePath).c_str();

	// load pic variables (for clarity).
	bool highQuality = true;
	bool useAlpha = true;
	bool splice = true;

	bool success = false;
	if (mCompressedDataBuffer && mCompressedDataSize && aFilePath != "")
	{
		// SexyImage data and Mask data.
		if (mAlphaCompressedDataBuffer && mAlphaCompressedDataSize && anAlphaFilePath != "")
		{
			success = mKGraphicImage->loadPictureWithMaskFromPtr(aFilePath.c_str(), mCompressedDataBuffer, mCompressedDataSize, 
				anAlphaFilePath.c_str(), mAlphaCompressedDataBuffer, mAlphaCompressedDataSize, 
				highQuality, splice, theStep);
		}
		// SexyImage data only.
		else
		{
			success = mKGraphicImage->loadPictureFromPtr(aFilePath.c_str(), mCompressedDataBuffer, mCompressedDataSize, 
				highQuality, useAlpha, splice, theStep);
		}
	}
	// Mask data only.
	else if (mAlphaCompressedDataBuffer && mAlphaCompressedDataSize && anAlphaFilePath != "")
	{
		useAlpha = false; // A mask by definition does not contain alpha.
		success = mKGraphicImage->loadPictureFromPtr(anAlphaFilePath.c_str(), mAlphaCompressedDataBuffer, mAlphaCompressedDataSize, 
			highQuality, useAlpha, splice, theStep);
	}

	// Purge data after successful upload. (JSullivan)
	if (success && (theStep != LOAD_STEP_LOAD_ONLY) && mPurgeBits)
		_PurgeBits();

	return success;
}

/*!***********************************
// @return    	bool
//
// \brief		Uploads the Bits array to the KGraphic.
// \details 	<b>Description:</b> Uploads the Bits array to the KGraphic.
//************************************/
bool SexyImage::CreateKGraphicFromBits()
{
	// load pic variables (for clarity)
	bool highQuality = true;
	bool useAlpha = true;
	bool splice = true;

	bool success = false;
	if (mBits)
	{
		success = mKGraphicImage->makePictureFromArray(mBits, GetWidth(), GetHeight(), 
			highQuality, useAlpha, splice);

		if (success)
		{
			mUploaded = true;
			// Purge data after successful upload. (JSullivan)
			if (mPurgeBits)
				_PurgeBits();
		}
	}

	return success;
}
/*!***********************************
// @return    	unsigned long*
//
// \brief		Stores a local copy of the bits and returns a pointer to them.
// \details 	<b>Description:</b> Stores a local copy of the bits and returns a pointer to them.
//************************************/
unsigned long* SexyImage::GetBits()
{
	if (!mBits)
	{
		// Load the image bits.
		KImage aTempKImage;
		bool haveImageBits = false;
		if (mCompressedDataBuffer && mCompressedDataSize && mFilePath != _S(""))
		{
			// Convert wstring to string.
			std::string aFilePath = SexyStringToStringFast(mFilePath).c_str();
			haveImageBits = aTempKImage.decodeImage(KImage::detectFormat(aFilePath.c_str()), mCompressedDataBuffer, mCompressedDataSize);
			if (haveImageBits)
			{
				delete [] mCompressedDataBuffer;
				mCompressedDataBuffer = NULL;
				mCompressedDataSize = 0;
			}
		}
		else if (mFilePath != _S(""))
		{
			std::string aFilePath = WStringToString(mFilePath);

			// Load the compressed image data from the file.
			bool success = KResource::loadResource(aFilePath.c_str(), mCompressedDataBuffer, mCompressedDataSize);
			if (success)
			{
				std::string aFilePath = SexyStringToStringFast(mFilePath).c_str();
				haveImageBits = aTempKImage.decodeImage(KImage::detectFormat(aFilePath.c_str()), mCompressedDataBuffer, mCompressedDataSize);
				if (haveImageBits)
				{
					delete [] mCompressedDataBuffer;
					mCompressedDataBuffer = NULL;
					mCompressedDataSize = 0;
				}
			}
		}


		// Load the alpha image bits.
		KImage anAlphaTempKImage;
		bool haveAlphaImageBits = false;
		if (mAlphaCompressedDataBuffer && mAlphaCompressedDataSize && mAlphaFilePath != _S(""))
		{
			// Convert wstring to string.
			std::string aFilePath = SexyStringToStringFast(mAlphaFilePath).c_str();
			haveAlphaImageBits = anAlphaTempKImage.decodeImage(KImage::detectFormat(aFilePath.c_str()), 
				mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);
			
			if (haveAlphaImageBits)
			{
				delete [] mAlphaCompressedDataBuffer;
				mAlphaCompressedDataBuffer = NULL;
				mAlphaCompressedDataSize = 0;
			}
		}
		else if (mAlphaFilePath != _S(""))
		{
			std::string aFilePath = WStringToString(mAlphaFilePath);

			// Load the compressed image data from the file.
			bool success = KResource::loadResource(aFilePath.c_str(), mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);
			if (success)
			{
				// Convert wstring to string.
				std::string aFilePath = SexyStringToStringFast(mAlphaFilePath).c_str();
				haveAlphaImageBits = anAlphaTempKImage.decodeImage(KImage::detectFormat(aFilePath.c_str()), 
					mAlphaCompressedDataBuffer, mAlphaCompressedDataSize);

				if (haveAlphaImageBits)
				{
					delete [] mAlphaCompressedDataBuffer;
					mAlphaCompressedDataBuffer = NULL;
					mAlphaCompressedDataSize = 0;
				}
			}
		}


		// Compose alpha channel with image
		if (haveImageBits && haveAlphaImageBits) 
		{
			int width = aTempKImage.getImageWidth();
			int height = aTempKImage.getImageHeight();

			if ((width == anAlphaTempKImage.getImageWidth()) &&
				(height == anAlphaTempKImage.getImageHeight()))
			{
				unsigned long* aBits1 = aTempKImage.getPixels();
				unsigned long* aBits2 = anAlphaTempKImage.getPixels();
				int aSize = width * height;
				mBits = new unsigned long [aSize];
				memset(mBits, 0, aSize * sizeof(unsigned long));

				for (int i = 0; i < aSize; i++)
				{
#ifdef __ppc__ // may not be necessary
					mBits[i] = (aBits1[i] & 0xFFFFFF00) | ((aBits2[i] & 0xFF000000)>>24);
#else
					mBits[i] = (aBits1[i] & 0x00FFFFFF) | ((aBits2[i] & 0xFF) << 24);
#endif
				}
			}
		}
		else if (haveImageBits)
		{
			int width = aTempKImage.getImageWidth();
			int height = aTempKImage.getImageHeight();

			mBits = new unsigned long [width * height];
			memcpy(mBits, aTempKImage.getPixels(), width * height * sizeof(unsigned long));
		}
		else if (haveAlphaImageBits)
		{
			int width = anAlphaTempKImage.getImageWidth();
			int height = anAlphaTempKImage.getImageHeight();

			unsigned long* aBits1 = anAlphaTempKImage.getPixels();
			int aSize = width * height;
			mBits = new unsigned long [aSize];
			memset(mBits, 0, aSize * sizeof(unsigned long));

			for (int i = 0; i < aSize; i++)
			{
				mBits[i] = (0x00FFFFFF) | ((aBits1[i] & 0xFF) << 24);
			}
		}

		mBitsDirty = true; // never been uploaded?
	}
	
	if (mBits)
	{
		mPurgeBits = false;
		return mBits;
	}


	return NULL;
}

/*!***********************************
// @return    	void
// @param     	theBits
// @param     	theWidth
// @param     	theHeight
//
// \brief		Copies theBits to an internal array to be uploaded the next draw.
// \details 	<b>Description:</b> Copies theBits to an internal array to be uploaded the next draw.
//************************************/
void SexyImage::SetBits(unsigned long* theBits, int theWidth, int theHeight)
{
	if (theBits != mBits)
	{
		if ((theWidth != mWidth) || (theHeight != mHeight) || (mBits == NULL))
		{
			delete [] mBits;
			mBits = new unsigned long [theWidth * theHeight];
			mWidth = theWidth;
			mHeight = theHeight;
		}

		memcpy(mBits, theBits, mWidth * mHeight * sizeof(unsigned long));
		mBitsDirty = true;
	}

	// Delete unnecessary data.
	if (mCompressedDataBuffer)
	{
		delete [] mCompressedDataBuffer;
		mCompressedDataBuffer = NULL;
		mCompressedDataSize = 0;
	}

	if (mAlphaCompressedDataBuffer)
	{
		delete [] mAlphaCompressedDataBuffer;
		mAlphaCompressedDataBuffer = NULL;
		mAlphaCompressedDataSize = 0;
	}
}

/*!***********************************
// @return    	void
// @param     	bPurgeBits
//
// \brief		Set whether or not the local data copies are released after upload to texture.
// \details 	<b>Description:</b> Set whether or not the local data copies are released after upload to texture.
//************************************/
void SexyImage::PurgeBits(bool bPurgeBits /*= true*/)
{
	mPurgeBits = bPurgeBits;
}

/*!***********************************
// @return    	void
//
// \brief		Internal bit purging method.
// \details 	<b>Description:</b> Internal bit purging method.
//************************************/
void SexyImage::_PurgeBits()
{
	if (mBits)
	{
		delete mBits;
		mBits = NULL;
	}

	if (mCompressedDataBuffer)
	{
		delete [] mCompressedDataBuffer;
		mCompressedDataBuffer = NULL;
		mCompressedDataSize = 0;
	}

	if (mAlphaCompressedDataBuffer)
	{
		delete [] mAlphaCompressedDataBuffer;
		mAlphaCompressedDataBuffer = NULL;
		mAlphaCompressedDataSize = 0;
	}

	mPurgeBits = false;
}

/*!***********************************
// @return    	KGraphic*
//
// \brief		Return a pointer to the KGraphic.  If not Uploaded to texture, upload now.
// \details 	<b>Description:</b> Return a pointer to the KGraphic.  If not Uploaded to texture, upload now.
//************************************/
KGraphic* SexyImage::GetKanjiImage()
{
	if (mBitsDirty || (!mUploaded && mBits))
	{
		mUploaded = CreateKGraphicFromBits();
		mBitsDirty = false;
	}

	// CreateKGraphicFromBits() sets mUploaded to true so we won't ever be 
	// creating 2 graphics by calling one after the other. (JSullivan)
	if (mLoaded && !mUploaded)
		// Load the mKGraphicImage with the image data.
		mUploaded = CreateKGraphicFromData(LOAD_STEP_UPLOAD_ONLY);

	return mKGraphicImage;
}

/*!***********************************
// @return    	void
// @param     	theAnimInfo
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyImage::SetAnimInfo(const AnimInfo& theAnimInfo)
{
	mAnimInfo = (theAnimInfo);
}

/*!***********************************
// @return    	int
//
// \brief		Return the height of each cel.
// \details 	<b>Description:</b> Return the height of each cel.
//************************************/
int SexyImage::GetCelHeight()
{
	if (mNumRows == 0)
		return 0;

	return mHeight / mNumRows;
}

/*!***********************************
// @return    	int
//
// \brief		Return the Width of Each Cel.
// \details 	<b>Description:</b> Return the Width of Each Cel.
//************************************/
int SexyImage::GetCelWidth()
{
	if (mNumCols == 0)
		return 0;

	return mWidth / mNumCols;
}

/*!***********************************
// @return    	Sexy::SexyRect
// @param     	theCol
// @param     	theRow
//
// \brief		Return a Src SexyRect for the specific cel.
// \details 	<b>Description:</b> Return a Src SexyRect for the specific cel.
//************************************/
SexyRect SexyImage::GetCelRect(int theCol, int theRow)
{
    DBG_ASSERT(theCol >= 0);
    DBG_ASSERT(theRow >= 0);

    if (theCol >= mNumCols) theCol %= mNumCols;
    if (theRow >= mNumRows) theRow %= mNumRows;

	int h = GetCelHeight();
	int w = GetCelWidth();
	int x = theCol * w;
	int y = theRow * h;

	return SexyRect(x, y, w, h);
}

/*!***********************************
// @return    	int
// @param     	theTime
//
// \brief		Return cel number for passed time.
// \details 	<b>Description:</b> Return cel number for passed time.
//************************************/
int	SexyImage::GetAnimCel(int theTime)
{
	return mAnimInfo.GetAnimCel(theTime);
}

/*!***********************************
// @return    	Sexy::SexyRect
// @param     	theTime
//
// \brief		Return a Src SexyRect for the specific time.
// \details 	<b>Description:</b> Return a Src SexyRect for the specific time.
//************************************/
SexyRect SexyImage::GetAnimCelRect(int theTime)
{
	SexyRect aRect;
	int aCel = GetAnimCel(theTime);
	int aCelWidth = GetCelWidth();
	int aCelHeight = GetCelHeight();

	if (mNumCols > 1)
		return SexyRect(aCel * aCelWidth, 0, aCelWidth, mHeight);
	else
		return SexyRect(0, aCel * aCelHeight, mWidth, aCelHeight);
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Provides a way to tell if the image has been loaded or not
// \details 	<b>Description:</b> Provides a way to tell if the image has been loaded or not
//************************************/
bool SexyImage::OpenFile(std::wstring theFileName, bool lookForAlphaFile  )
{
	// Convert wstring to string for loadResource.
	std::string aFilePath = WStringToString(theFileName);

	// Load the compressed image data from the file.
	bool success = KResource::loadResource(aFilePath.c_str(), mCompressedDataBuffer, mCompressedDataSize);
	

	if(!success)
	{
		std::vector<std::string> anExtQueue;
		anExtQueue.push_back(".j2k");anExtQueue.push_back(".jp2");anExtQueue.push_back(".jpf");
		anExtQueue.push_back(".bmp");anExtQueue.push_back(".tga");anExtQueue.push_back(".png");
		anExtQueue.push_back(".jpg");anExtQueue.push_back(".gif");

		while(!success && !anExtQueue.empty())
		{
			aFilePath = WStringToString(theFileName) + anExtQueue.back();
			anExtQueue.pop_back();

			success = KResource::loadResource(aFilePath.c_str(), mCompressedDataBuffer, mCompressedDataSize);
		}
	}

	if (success)
	{
		mFilePath = StringToSexyStringFast(aFilePath);

		if (lookForAlphaFile)
			LoadAlphaMask(mFilePath);

		// Load the mKGraphicImage with the image data.
		mLoaded = success = CreateKGraphicFromData(LOAD_STEP_LOAD_ONLY);

		if (success)
		{
			mKGraphicImage->setTextureQuality(true);
			mWidth = static_cast<int>(mKGraphicImage->getWidth());
			mHeight = static_cast<int>(mKGraphicImage->getHeight());
		}
	}

	return success;
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Provides a way to tell if the image has been loaded or not
// \details 	<b>Description:</b> Provides a way to tell if the image has been loaded or not
//************************************/
bool Sexy::SexyImage::OpenFile( std::string theFileName, bool lookForAlphaFile /*= true*/ )
{
	return OpenFile(StringToWString(theFileName), lookForAlphaFile);
}

/*!***********************************
// @return    	void
// @param     	theWrapMode
//
// \brief		Sets the texutre addressing mode for the Image. 
// \details 	<b>Description:</b> Valid parameters: {K_WRAP_CLAMP, K_WRAP_REPEAT, K_WRAP_MIRROR}
//************************************/
void Sexy::SexyImage::SetTextureWrappingMode( long theWrapMode /*= K_WRAP_CLAMP*/ )
{
	switch(theWrapMode)
	{
	case K_WRAP_CLAMP:
	case K_WRAP_MIRROR:
	case K_WRAP_REPEAT:
		mWrappingMode = theWrapMode;
		if(mKGraphicImage)
			mKGraphicImage->allowTextureWrap(mWrappingMode);
		break;
	}
}

/*!***********************************
// @return    	long
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
long Sexy::SexyImage::GetTextureWrappingMode()
{
	return mWrappingMode;
}

/*!***********************************
// @return    	Sexy::SexyRect
// @param     	theCel
//
// \brief		Returns the cel rect for the specified cel
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyRect Sexy::SexyImage::GetCelRect( int theCel )
{
	int aTotalCelCount = mNumTotal;
	aTotalCelCount = (aTotalCelCount > 0) ? aTotalCelCount : 1; // but not zero
	theCel = theCel % aTotalCelCount;

	int aRow = theCel / mNumCols;
	int aCol = theCel % mNumCols;

	return GetCelRect(aCol, aRow);
}
/* AnimInfo */

/*!***********************************
// @return    	
//
// \brief		Default CTor for AnimInfo.
// \details 	<b>Description:</b> Default CTor for AnimInfo.
//************************************/
AnimInfo::AnimInfo() :
mAnimType(AnimType_None),
mFrameDelay(1),
mNumCels(1),
mPerFrameDelay(),
mFrameMap(),
mTotalAnimTime(0)
{
}

/*!***********************************
// @return    	void
// @param     	theFrame
// @param     	theTime
//
// \brief		Set the delay time for an individual anim frame.
// \details 	<b>Description:</b> Set the delay time for an individual anim frame.
//************************************/
void AnimInfo::SetPerFrameDelay(int theFrame, int theTime)
{
	if (static_cast<int>(mPerFrameDelay.size()) <= theFrame)
	{
		mPerFrameDelay.resize(theFrame+1);
	}

	mPerFrameDelay[theFrame] = theTime;
}

/*!***********************************
// @return    	void
// @param     	theNumCels
// @param     	theBeginFrameTime
// @param     	theEndFrameTime
//
// \brief		Compute the default frame map and delays.
// \details 	<b>Description:</b> Compute the default frame map and delays.
//************************************/
void AnimInfo::Compute(int theNumCels, int theBeginFrameTime, int theEndFrameTime)
{
	mNumCels = theNumCels;
	if (mNumCels <= 0)
	{
		mNumCels = 1;
	}

	if (mFrameDelay <= 0)
	{
		mFrameDelay = 1;
	}

	if ((mAnimType == AnimType_PingPong) && (mNumCels > 1))
	{
		mFrameMap.resize((theNumCels * 2) - 2);
		int index = 0;
		for (int i = 0; i < theNumCels; i++)
		{
			mFrameMap[index++] = i;
		}

		for (int i = theNumCels - 2; i >= 1; i--)
		{
			mFrameMap[index++] = i;
		}
	}

	if (!mFrameMap.empty())
	{
		mNumCels = static_cast<int>(mFrameMap.size());
	}

	if (theBeginFrameTime > 0)
	{
		SetPerFrameDelay(0, theBeginFrameTime);
	}

	if (theEndFrameTime > 0)
	{
		SetPerFrameDelay(mNumCels - 1, theEndFrameTime);
	}

	if (!mPerFrameDelay.empty())
	{
		mTotalAnimTime = 0;
		mPerFrameDelay.resize(mNumCels);

		for (int i = 0; i < mNumCels; i++)
		{
			if (mPerFrameDelay[i] <= 0)
			{
				mPerFrameDelay[i] = mFrameDelay;
			}

			mTotalAnimTime += mPerFrameDelay[i];
		}
	}
	else
	{
		mTotalAnimTime = mFrameDelay * mNumCels;
	}

	if (!mFrameMap.empty())
	{
		mFrameMap.resize(mNumCels);
	}
}

/*!***********************************
// @return    	int the cel number
// @param     	theTime
//
// \brief		Get the cel number for the passed time.
// \details 	<b>Description:</b> Get the cel number for the passed time.
//************************************/
int AnimInfo::GetPerFrameCel(int theTime)
{
	for (int i = 0; i < mNumCels; i++)
	{
		theTime -= mPerFrameDelay[i];
		if (theTime < 0)
		{
			return i;
		}
	}

	return mNumCels - 1;
}

/*!***********************************
// @return    	int the cel number
// @param     	theTime
//
// \brief		Get the cel number for the passed time.
// \details 	<b>Description:</b> Get the cel number for the passed time.
//************************************/
int AnimInfo::GetAnimCel(int theTime)
{
	if (mAnimType == AnimType_Once && theTime >= mTotalAnimTime)
	{
		if (!mFrameMap.empty())
		{
			return mFrameMap[mFrameMap.size() - 1];
		}
		else
		{
			return mNumCels - 1;
		}
	}

	theTime = theTime % mTotalAnimTime;

	int aFrame;
	if (!mPerFrameDelay.empty())
	{
		aFrame = GetPerFrameCel(theTime);
	}
	else
	{
		aFrame = (theTime / mFrameDelay) % mNumCels;
	}

	if (mFrameMap.empty())
	{
		return aFrame;
	}
	else
	{
		return mFrameMap[aFrame];
	}
}

/*!***********************************
// @return    	int the animation cel number
// @param     	theTime
//
// \brief		Get the image cel number for the passed animation frame.
// \details 	<b>Description:</b> Get the image cel number for the passed animation frame.
//************************************/
int AnimInfo::GetImageCel(int theFrame)
{
    if (mAnimType == AnimType_Once && theFrame >= mNumCels)
    {
        return mFrameMap[mFrameMap.size() - 1];
    }

    theFrame %= mNumCels;

    if (mFrameMap.empty())
    {
        return theFrame;
    }
    else
    {
        return mFrameMap[theFrame];
    }
}