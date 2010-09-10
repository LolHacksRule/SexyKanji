// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	created:	2010/02/17
	created:	17:2:2010   23:17
	filename: 	SexyKanji\ImageManager.cpp
	file base:	ImageManager
	file ext:	cpp
	author:		Jeremy Sullivan / James Poag
	
	purpose:	SexyAppFramework Image Manager, Handles loading, modifying
				and copying images.
*********************************************************************/
#include "ImageManager.h"

#include "SexyImage.h"
#include "SexyRect.h"

#include "KPTK.h"
#include "Common.h"

// This needs to be the last include directive.  All other
// #includes should be above this point.
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		Default CTor for ImageManager.
// \details 	<b>Description:</b> Default CTor for ImageManager.
//************************************/
ImageManager::ImageManager() :
mSharedImageMap()
{
}

/*!***********************************
// @return    	
//
// \brief		Destructor
// \details 	<b>Description:</b> Destructor
//************************************/
ImageManager::~ImageManager()
{
	CleanSharedImages(true);
}

/*!***********************************
// @return    	SexyImage*
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Load the Image specified.
// \details 	<b>Description:</b> Load the Image specified. Optionally look for an 
// alpha mask file to use with the image.
//************************************/
SexyImage* ImageManager::GetImage(const std::wstring& theFileName, bool lookForAlphaFile)
{
	SexyImage* aRetImage = new SexyImage();
	if (!aRetImage->OpenFile(theFileName, lookForAlphaFile)) // Basic error handling
	{
		delete aRetImage;
		return NULL;
	}
	return aRetImage;
}

/*!***********************************
// @return    	SexyImage*
// @param     	theFileName
// @param     	lookForAlphaFile
//
// \brief		Load the Image specified.
// \details 	<b>Description:</b> Load the Image specified. Optionally look for an 
// alpha mask file to use with the image.
//************************************/
SexyImage* Sexy::ImageManager::GetImage( const std::string& theFileName, bool lookForAlphaFile /*= true*/ )
{
	SexyImage* aRetImage = new SexyImage();
	if (!aRetImage->OpenFile(StringToSexyStringFast(theFileName), lookForAlphaFile)) // Basic error handling
	{
		delete aRetImage;
		return NULL;
	}
	return aRetImage;
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage1
// @param     	theRect1
// @param     	theImage2
// @param     	theRect2
// @param     	theFadeFactor
//
// \brief		Fade two images together.
// \details 	<b>Description:</b> Fade two images together.
//************************************/
SexyImage* ImageManager::CreateCrossfadeImage(SexyImage* theImage1, const SexyRect& theRect1, SexyImage* theImage2, const SexyRect& theRect2, double theFadeFactor)
{
	if ((theImage1 == NULL) || (theImage2 == NULL)) return NULL;

	if ((theRect1.mX < 0) || (theRect1.mY < 0) || 
		(theRect1.mX + theRect1.mWidth > theImage1->GetWidth()) ||
		(theRect1.mY + theRect1.mHeight > theImage1->GetHeight()))
	{
		return NULL;
	}

	if ((theRect2.mX < 0) || (theRect2.mY < 0) || 
		(theRect2.mX + theRect2.mWidth > theImage2->GetWidth()) ||
		(theRect2.mY + theRect2.mHeight > theImage2->GetHeight()))
	{
		return NULL;
	}

	int aWidth = theRect1.mWidth;
	int aHeight = theRect1.mHeight;

	SexyImage* anImage = new SexyImage();

	int aSize = aWidth * aHeight;
	unsigned long* aDestBits = new unsigned long[aSize];
	unsigned long* aSrcBits1 = theImage1->GetBits();
	unsigned long* aSrcBits2 = theImage2->GetBits();

	if(aDestBits == NULL || aSrcBits1 == NULL || aSrcBits2 == NULL)
	{
		delete [] aDestBits;
		aDestBits = NULL;
		return NULL;
	}

	int aSrc1Width = theImage1->GetWidth();
	int aSrc2Width = theImage2->GetWidth();
	unsigned long aMult = static_cast<unsigned long>(theFadeFactor * 256);
	unsigned long aOMM = (256 - aMult);

	for (int y = 0; y < aHeight; y++)
	{
		unsigned long* s1 = &aSrcBits1[(y+theRect1.mY)*aSrc1Width+theRect1.mX];
		unsigned long* s2 = &aSrcBits2[(y+theRect2.mY)*aSrc2Width+theRect2.mX];
		unsigned long* d = &aDestBits[y*aWidth];

		for (int x = 0; x < aWidth; x++)
		{
			unsigned long p1 = *s1++;
			unsigned long p2 = *s2++;

			*d++ = 
				((((p1 & 0x000000FF)*aOMM + (p2 & 0x000000FF)*aMult)>>8) & 0x000000FF) |
				((((p1 & 0x0000FF00)*aOMM + (p2 & 0x0000FF00)*aMult)>>8) & 0x0000FF00) |
				((((p1 & 0x00FF0000)*aOMM + (p2 & 0x00FF0000)*aMult)>>8) & 0x00FF0000) |
				((((p1 >> 24)*aOMM + (p2 >> 24)*aMult)<<16) & 0xFF000000);
		}
	}

	anImage->SetBits(aDestBits, aWidth, aHeight);
	delete [] aDestBits;
	aDestBits = NULL;

	return anImage;
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theColor
//
// \brief		Colorize (tint) the passed image with the passed color.
// \details 	<b>Description:</b> Colorize (tint) the passed image with the passed color.
//				Modifies the passed image!
//************************************/
void ImageManager::ColorizeImage(SexyImage* theImage, const SexyColor& theColor)
{
	if (theImage == NULL) return;

	unsigned long* aBits = theImage->GetBits();
	if(aBits == NULL) return;

	int aNumColors = theImage->GetWidth() * theImage->GetHeight();

	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) && 
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			unsigned long aColor = aBits[i];

			aBits[i] = 
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00)|
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			unsigned long aColor = aBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}	

	theImage->CommitBits();
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage
// @param     	theColor
//
// \brief		Create a recolored copy of the passed image
// \details 	<b>Description:</b>	Create a copy of the passed image and then 
// colorize (tint) the copy with the passed color. 
//************************************/
SexyImage* ImageManager::CreateColorizedImage(SexyImage* theImage, const SexyColor& theColor)
{
	if (theImage == NULL) return NULL;

	SexyImage* anImage = new SexyImage();

	int aNumColors = theImage->GetWidth() * theImage->GetHeight();		
	if(aNumColors == 0) return anImage;

	unsigned long* aSrcBits = theImage->GetBits();
	unsigned long* aDestBits = new unsigned long[aNumColors];
	if(aSrcBits == NULL || aDestBits == NULL)
	{
		delete anImage;
		return NULL;
	}

	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) && 
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			unsigned long aColor = aSrcBits[i];

			aDestBits[i] = 
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00)|
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			unsigned long aColor = aSrcBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aDestBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}	

	anImage->SetBits(aDestBits, theImage->GetWidth(), theImage->GetHeight());
	delete [] aDestBits;
	aDestBits = NULL;

	return anImage;
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage
//
// \brief		Create a copy of the passed image.
// \details 	<b>Description:</b> Create a copy of the passed image.
//************************************/
SexyImage* ImageManager::CopyImage(const SexyImage* theImage)
{
	if (theImage == NULL) return NULL;

	SexyImage* anImage = new SexyImage(*theImage);
	return anImage;
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage
// @param     	theRect
//
// \brief		Create a copy of the passed image clipped to the passed rect.
// \details 	<b>Description:</b> Create a copy of the passed image clipped to the passed rect.
//************************************/
SexyImage* ImageManager::CopyImage(SexyImage* theImage, const SexyRect& theRect)
{
	if (theImage == NULL) return NULL;

	// The passed rect is the same as the image size, so just do a standard copy.
	if ((theRect.mX == 0) && (theRect.mY == 0) && 
		(theRect.mWidth == theImage->GetWidth()) && (theRect.mHeight == theImage->GetHeight()))
	{
		return CopyImage(theImage);
	}

	if (theRect.mWidth <= 0 || theRect.mHeight <= 0) return NULL;

	SexyImage* anImage = new SexyImage();

	unsigned long* anOriginalBits = theImage->GetBits();
	unsigned long* aNewBits = new unsigned long[theRect.mWidth * theRect.mHeight];
	unsigned long* aNewBItsItr = aNewBits;

	if(anImage == NULL || anOriginalBits == NULL || aNewBits == NULL)
	{
		delete anImage;
		anImage = NULL;

		delete aNewBits;
		aNewBits = NULL;

		return NULL;
	}

	int aWidth = theImage->GetWidth();
	int aHeight = theImage->GetHeight();

	for (int y = 0; y < aHeight; y++)
	{
		for (int x = 0; x < aWidth; x++)
		{
			if ((y >= theRect.mY) && (x >= theRect.mX) && 
				(y < (theRect.mY + theRect.mHeight)) && (x < (theRect.mX + theRect.mWidth)))
			{
				*aNewBItsItr++ = anOriginalBits[y * aWidth + x];
			}
		}
	}

	anImage->SetBits(aNewBits, theRect.mWidth, theRect.mHeight);

	delete [] aNewBits;
	aNewBits = aNewBItsItr = NULL;
	return anImage;
}

/*!***********************************
// @return    	void
// @param     	theImage
//
// \brief		Mirror (reverse horizontaly) the passed image.
// \details 	<b>Description:</b> Mirror (reverse horizontaly) the passed image.
// Modifies the passed image!
//************************************/
void ImageManager::MirrorImage(SexyImage* theImage)
{
	if (theImage == NULL) return;

	unsigned long* aSrcBits = theImage->GetBits();
	if(aSrcBits == NULL)return;

	int aPhysSrcWidth = theImage->GetWidth();
	for (int y = 0; y < theImage->GetHeight(); y++)
	{
		unsigned long* aLeftBits = aSrcBits + (y * aPhysSrcWidth);		
		unsigned long* aRightBits = aLeftBits + (aPhysSrcWidth - 1);

		for (int x = 0; x < (aPhysSrcWidth >> 1); x++)
		{
			unsigned long aSwap = *aLeftBits;

			*(aLeftBits++) = *aRightBits;
			*(aRightBits--) = aSwap;
		}
	}

	theImage->CommitBits();	
}

/*!***********************************
// @return    	void
// @param     	theImage
//
// \brief		Flip (reverse vertically) the passed image.
// \details 	<b>Description:</b>Flip (reverse vertically) the passed image.
// Modifies the passed image! 
//************************************/
void ImageManager::FlipImage(SexyImage* theImage)
{
	if (theImage == NULL) return;

	unsigned long* aSrcBits = theImage->GetBits();
	if(aSrcBits == NULL)return;

	int aPhysSrcHeight = theImage->GetHeight();
	int aPhysSrcWidth = theImage->GetWidth();
	for (int x = 0; x < aPhysSrcWidth; x++)
	{
		unsigned long* aTopBits    = aSrcBits + x;
		unsigned long* aBottomBits = aTopBits + (aPhysSrcWidth * (aPhysSrcHeight - 1));

		for (int y = 0; y < (aPhysSrcHeight >> 1); y++)
		{
			unsigned long aSwap = *aTopBits;

			*aTopBits = *aBottomBits;
			aTopBits += aPhysSrcWidth;
			*aBottomBits = aSwap;
			aBottomBits -= aPhysSrcWidth;
		}
	}

	theImage->CommitBits();	
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theDelta
//
// \brief		Modify the hue of the passed image by passed delta.
// \details 	<b>Description:</b> Modify the hue of the passed image by passed delta.
// Modifies the passed image!
//************************************/
void ImageManager::RotateImageHue(SexyImage* theImage, int theDelta)
{
	if (theImage == NULL) return;

	while (theDelta < 0)
		theDelta += 256;

	int aSize = theImage->GetWidth() * theImage->GetHeight();
	unsigned long* aPtr = theImage->GetBits();
	if (aPtr == NULL)return;

	for (int i = 0; i < aSize; i++)
	{
		unsigned long aPixel = *aPtr;
		int alpha = aPixel&0xff000000;
		int r = (aPixel >> 16) & 0xff;
		int g = (aPixel >> 8) & 0xff;
		int b = aPixel & 0xff;

		int maxval = K_MAX(r, K_MAX(g, b));
		int minval = K_MIN(r, K_MIN(g, b));
		int h = 0;
		int s = 0;
		int l = (minval + maxval) / 2;
		int delta = maxval - minval;

		if (delta != 0)
		{			
			s = (delta * 256) / ((l <= 128) ? (minval + maxval) : (512 - maxval - minval));
			
			if (r == maxval)
				h = (g == minval ? 1280 + (((maxval-b) * 256) / delta) :  256 - (((maxval - g) * 256) / delta));
			else if (g == maxval)
				h = (b == minval ?  256 + (((maxval-r) * 256) / delta) :  768 - (((maxval - b) * 256) / delta));
			else
				h = (r == minval ?  768 + (((maxval-g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));
			
			h /= 6;
		}

		h += theDelta;
		if (h >= 256)
			h -= 256;

		double v = (l < 128) ? (l * (255 + s)) / 255 :
				(l + s - l * s / 255);
		
		int y = (int) (2*l-v);

		int aColorDiv = (6 * h) / 256;
		int x = (int)(y+(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
		if (x > 255)
			x = 255;

		int z = (int) (v-(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
		if (z < 0)
			z = 0;
		
		switch (aColorDiv)
		{
			case 0: r = (int) v; g = x; b = y; break;
			case 1: r = z; g= (int) v; b = y; break;
			case 2: r = y; g= (int) v; b = x; break;
			case 3: r = y; g = z; b = (int) v; break;
			case 4: r = x; g = y; b = (int) v; break;
			case 5: r = (int) v; g = y; b = z; break;
			default: r = (int) v; g = x; b = y; break;
		}

		*aPtr++ = alpha | (r << 16) | (g << 8) | (b);	 

	}

	theImage->CommitBits();
}

/*!***********************************
// @return    	Sexy::SharedImageRef
// @param     	theFileName
// @param     	theVariant
// @param     	bLookForAlpha
// @param     	isNew
//
// \brief		Get a shared image ref.
// \details 	<b>Description:</b> Get a shared image ref.
//************************************/
SharedImageRef ImageManager::GetSharedImage(const std::wstring& theFileName, const std::wstring& theVariant, bool bLookForAlpha, bool* isNew)
{
	SexyString anUpperFileName = StringToUpper(WStringToSexyStringFast(theFileName));
	SexyString anUpperVariant = StringToUpper(WStringToSexyStringFast(theVariant));

	std::pair<SharedImageMap::iterator, bool> aResultPair;
	SharedImageRef aSharedImageRef;

	{
		// Make sure that the SharedImageMap container is thread-safe.
		AutoCrit anAutoLock(mImageManagerCritSect);
		aResultPair = mSharedImageMap.insert(SharedImageMap::value_type(SharedImageMap::key_type(anUpperFileName, anUpperVariant), SharedImage()));
		aSharedImageRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second)
	{
		// Pass in a '!' as the first char of the file name to create a new aRetImage
		if ((theFileName.length() > 0) && (theFileName[0] == '!'))
			aSharedImageRef.mSharedItem->mItem = new SexyImage();
		else
			aSharedImageRef.mSharedItem->mItem = GetImage(theFileName, bLookForAlpha);
	}

	return aSharedImageRef;
}

/*!***********************************
// @return    	Sexy::SharedImageRef
// @param     	theFileName
// @param     	theVariant
// @param     	bLookForAlpha
// @param     	isNew
//
// \brief		Get a shared image ref.
// \details 	<b>Description:</b> Get a shared image ref.
//************************************/
Sexy::SharedImageRef Sexy::ImageManager::GetSharedImage( const std::string& theFileName, const std::string& theVariant /*= ""*/, bool bLookForAlpha /*= true*/, bool* isNew /*= NULL*/ )
{
	SexyString anUpperFileName = StringToUpper(StringToSexyStringFast(theFileName));
	SexyString anUpperVariant = StringToUpper(StringToSexyStringFast(theVariant));

	std::pair<SharedImageMap::iterator, bool> aResultPair;
	SharedImageRef aSharedImageRef;

	{
		// Make sure that the SharedImageMap container is thread-safe.
		AutoCrit anAutoLock(mImageManagerCritSect);
		aResultPair = mSharedImageMap.insert(SharedImageMap::value_type(SharedImageMap::key_type(anUpperFileName, anUpperVariant), SharedImage()));
		aSharedImageRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second)
	{
		// Pass in a '!' as the first char of the file name to create a new aRetImage
		if ((theFileName.length() > 0) && (theFileName[0] == '!'))
			aSharedImageRef.mSharedItem->mItem = new SexyImage();
		else
			aSharedImageRef.mSharedItem->mItem = GetImage(theFileName, bLookForAlpha);
	}

	return aSharedImageRef;
}

/*!***********************************
// @return    	void
// @param     	forceDeleteAll
//
// \brief		Delete all shared images with 0 references or all if forced.
// \details 	<b>Description:</b> Delete all shared images with 0 references or all if forced.
//************************************/
void ImageManager::CleanSharedImages(bool forceDeleteAll)
{
	// Make sure that the SharedImageMap container is thread-safe.
	AutoCrit anAutoLock(mImageManagerCritSect);

	// Delete shared images with reference counts of 0 or all if forced.
	SharedImageMap::iterator aSharedImageItr = mSharedImageMap.begin();
	while (aSharedImageItr != mSharedImageMap.end())
	{
		SharedImage* aSharedImage = &aSharedImageItr->second;
		if ((aSharedImage->mRefCount == 0) || forceDeleteAll)
		{
			delete aSharedImage->mItem;
			mSharedImageMap.erase(aSharedImageItr++);
			// This might throw an error someday (JPOAG)
		}
		else
		{
			++aSharedImageItr;
		}
	}
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage
// @param     	theRotation
//
// \brief		Returns a New Image by rotating the input image.
// \details 	<b>Description:</b>Returns a New Image by rotating the input image.
//				{ROT_IMG_90_DEG = 90, ROT_IMG_180_DEG = 180, ROT_IMG_270_DEG = 270,}; 
//************************************/
SexyImage* Sexy::ImageManager::CreateRotatedImage( SexyImage* theImage, int theRotation /*= ROT_IMG_90_DEG*/ )
{
	if (theImage == NULL) return NULL;

	int aSrcWidth = theImage->GetWidth();
	int aSrcHeight = theImage->GetHeight();
	int aDestWidth = aSrcWidth;
	int aDestHeight = aSrcHeight;

	unsigned long* aDestBits = new unsigned long[aDestWidth * aDestHeight * sizeof(unsigned long)];
	unsigned long* aSrcBits = theImage->GetBits();

	SexyImage* aNewImage = new SexyImage();

	switch(theRotation)
	{
	case ROT_IMG_90_DEG:
		aDestWidth = aDestHeight;
		aDestHeight = aSrcWidth;
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = aSrcY;
				int aDestY = (aDestHeight - aSrcX - 1);
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		aNewImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	case ROT_IMG_180_DEG:
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = (aDestWidth - aSrcX - 1);
				int aDestY = (aDestHeight - aSrcY - 1);
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		aNewImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	case ROT_IMG_270_DEG:
		aDestWidth = aDestHeight;
		aDestHeight = aSrcWidth;
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = (aDestWidth - aSrcY - 1);
				int aDestY = aSrcX;
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		aNewImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	default:
		K_LOG(StrFormat("Sexy::ImageManager::CreateRotatedImage :Warning: Attempt to call with invalid rotation: %d", theRotation).c_str());
		aNewImage->SetBits(theImage->GetBits(), aDestWidth, aDestHeight);
		break;
	}

	delete [] aDestBits;

	return aNewImage;
}

/*!***********************************
// @return    	bool
// @param     	theImage
// @param     	theRotation
//
// \brief		Rotates theImage in place.
// \details 	<b>Description:</b> Rotates theImage in place.  Returns true if successful, false otherwise
//				valid rotations are: {ROT_IMG_90_DEG = 90, ROT_IMG_180_DEG = 180, ROT_IMG_270_DEG = 270};
//************************************/
bool Sexy::ImageManager::RotateImage( SexyImage* theImage, int theRotation /*= ROT_IMG_90_DEG*/ )
{
	if (theImage == NULL)
	{
		K_LOG("Sexy::ImageManager::RotateImage :ERROR: NULL Pointer Passed ");
		return false;
	}

	int aSrcWidth = theImage->GetWidth();
	int aSrcHeight = theImage->GetHeight();
	int aDestWidth = aSrcWidth;
	int aDestHeight = aSrcHeight;

	bool bSuccess = true;

	unsigned long* aDestBits = new unsigned long[aDestWidth * aDestHeight * sizeof(unsigned long)];
	unsigned long* aSrcBits = theImage->GetBits();

	switch(theRotation)
	{
	case ROT_IMG_90_DEG:
		aDestWidth = aDestHeight;
		aDestHeight = aSrcWidth;
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = aSrcY;
				int aDestY = (aDestHeight - aSrcX - 1);
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		theImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	case ROT_IMG_180_DEG:
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = (aDestWidth - aSrcX - 1);
				int aDestY = (aDestHeight - aSrcY - 1);
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		theImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	case ROT_IMG_270_DEG:
		aDestWidth = aDestHeight;
		aDestHeight = aSrcWidth;
		for (int aSrcY = 0; aSrcY < aSrcHeight; aSrcY++)
		{
			for (int aSrcX = 0; aSrcX < aSrcWidth; aSrcX++)
			{
				int aDestX = (aDestWidth - aSrcY - 1);
				int aDestY = aSrcX;
				aDestBits[aDestY*aDestWidth + aDestX] = aSrcBits[aSrcY*aSrcWidth + aSrcX];
			}
		}
		theImage->SetBits(aDestBits, aDestWidth, aDestHeight);
		break;


	default:
		K_LOG(StrFormat("Sexy::ImageManager::CreateRotatedImage :Warning: Attempt to call  with invalid rotation: %d", theRotation).c_str());
		bSuccess = false;
		break;
	}

	delete [] aDestBits;
	return bSuccess;
}

/*!***********************************
// @return    	bool
// @param     	theImage
// @param     	theScaleX
// @param     	theScaleY
//
// \brief		Resample the Image in-place using Bilinear Filtering.
// \details 	<b>Description:</b> Resample the Image in-place using Bilinear Filtering.
//************************************/
bool Sexy::ImageManager::ScaleImage(SexyImage* theImage, float theScaleX, float theScaleY )
{
	if (theImage == NULL)
	{
		K_LOG("Sexy::ImageManager::ScaleImage :ERROR: Attempting to pass NULL Pointer");
			return false;
	}

	if (theScaleX == 0 || theScaleY == 0 || theImage->GetWidth() == 0 || theImage->GetHeight() == 0 || theImage->GetBits() == NULL)
	{
		K_LOG("Sexy::ImageManager::ScaleImage :ERROR: ScaleX: %f ScaleY: %f ImageWidth: %d ImageHeight: %d Bits %X",
			theScaleX, theScaleY, theImage->GetWidth(), theImage->GetHeight(), theImage->GetBits());
			return false;
	}

	long theWidth = theImage->GetWidth();
	long theHeight = theImage->GetHeight();
	KImage aTempImage;

	unsigned long* aTempPixelBuffer = new unsigned long[theWidth * theHeight * sizeof(unsigned long)];
	memcpy(aTempPixelBuffer, theImage->GetBits(), theWidth * theHeight * sizeof(unsigned long));

	// KImage frees aTempPixelBuffer during the Re sampling
	aTempImage.setPixels(theImage->GetWidth(), theImage->GetHeight(), true, aTempPixelBuffer);
	aTempImage.resampleImage((unsigned long)((float)theWidth * theScaleX), (unsigned long)((float)theHeight * theScaleY));

	theImage->SetBits(aTempImage.getPixels(), aTempImage.getImageWidth(), aTempImage.getImageHeight());

	return true;
}

/*!***********************************
// @return    	SexyImage*
// @param     	theImage
// @param     	theScaleX
// @param     	theScaleY
//
// \brief		Creates and returns a new image based on the scaling
// \details 	<b>Description:</b> Creates and returns a new image based on the scaling parameters
//				using Bilinear Filtering.
//************************************/
SexyImage* Sexy::ImageManager::CreateScaledImage( SexyImage* theImage, float theScaleX, float theScaleY )
{
	if (theImage == NULL)
	{
		K_LOG("Sexy::ImageManager::ScaleImage :ERROR: Attempting to pass NULL Pointer");
		return NULL;
	}

	if (theScaleX == 0 || theScaleY == 0 || theImage->GetWidth() == 0 || theImage->GetHeight() == 0 || theImage->GetBits() == NULL)
	{
		K_LOG("Sexy::ImageManager::ScaleImage :ERROR: ScaleX: %f ScaleY: %f ImageWidth: %d ImageHeight: %d Bits %X",
			theScaleX, theScaleY, theImage->GetWidth(), theImage->GetHeight(), theImage->GetBits());
		return NULL;
	}

	long theWidth = theImage->GetWidth();
	long theHeight = theImage->GetHeight();
	KImage aTempImage;

	unsigned long* aTempPixelBuffer = new unsigned long[theWidth * theHeight * sizeof(unsigned long)];
	memcpy(aTempPixelBuffer, theImage->GetBits(), theWidth * theHeight * sizeof(unsigned long));

	// KImage frees aTempPixelBuffer during the Re sampling
	aTempImage.setPixels(theImage->GetWidth(), theImage->GetHeight(), true, aTempPixelBuffer);
	aTempImage.resampleImage((unsigned long)((float)theWidth * theScaleX), (unsigned long)((float)theHeight * theScaleY));

	SexyImage* aNewImage = new SexyImage();
	aNewImage->SetBits(aTempImage.getPixels(), aTempImage.getImageWidth(), aTempImage.getImageHeight());

	return aNewImage;
}

