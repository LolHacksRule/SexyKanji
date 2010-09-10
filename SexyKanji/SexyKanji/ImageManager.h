// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include "SharedImage.h"
#include "SexyRect.h"
#include "SexyColor.h"
#include "AutoCrit.h"

#include <string>

namespace Sexy
{
	class SexyImage;

	class ImageManager
	{
	private:
		SharedImageMap							mSharedImageMap;
		KSysLock								mImageManagerCritSect;

	public:
		ImageManager();
		ImageManager(ImageManager const&);
		ImageManager& operator=(ImageManager const&);
		~ImageManager();

		SexyImage*								GetImage(const std::wstring& theFileName, bool lookForAlphaFile = true);
		SexyImage*								GetImage(const std::string& theFileName, bool lookForAlphaFile = true);
		static SexyImage*						CreateCrossfadeImage(SexyImage* theImage1, const SexyRect& theRect1, SexyImage* theImage2, const SexyRect& theRect2, double theFadeFactor);
		static void								ColorizeImage(SexyImage* theImage, const SexyColor& theColor);
		static SexyImage*						CreateColorizedImage(SexyImage* theImage, const SexyColor& theColor);
		static SexyImage*						CopyImage(const SexyImage* theImage);
		static SexyImage*						CopyImage(SexyImage* theImage, const SexyRect& theRect);
		static void								MirrorImage(SexyImage* theImage);
		static void								FlipImage(SexyImage* theImage);
		static void								RotateImageHue(SexyImage* theImage, int theDelta);

		enum{ROT_IMG_90_DEG = 90, ROT_IMG_180_DEG = 180, ROT_IMG_270_DEG = 270,};
		static SexyImage*						CreateRotatedImage(SexyImage* theImage, int theRotation = ROT_IMG_90_DEG);
		static bool								RotateImage(SexyImage* theImage, int theRotation = ROT_IMG_90_DEG);

		static bool								ScaleImage(SexyImage* theImage, float theScaleX, float theScaleY);
		static SexyImage*						CreateScaledImage(SexyImage* theImage, float theScaleX, float theScaleY);


		SharedImageRef							GetSharedImage(const std::wstring& theFileName, const std::wstring& theVariant = L"", bool bLookForAlpha = true, bool* isNew = NULL);
		SharedImageRef							GetSharedImage(const std::string& theFileName, const std::string& theVariant = "", bool bLookForAlpha = true, bool* isNew = NULL);
		void									CleanSharedImages(bool forceDeleteAll = false);
	};
}

#endif // __IMAGE_MANAGER_H__
