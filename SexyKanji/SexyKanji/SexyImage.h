// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	created:	2010/02/17
	created:	17:2:2010   23:06
	modified:	20:4:2010   15:29
	filename: 	SexyKanji\Image.h
	file base:	Image
	file ext:	h
	author:		Jeremy Sullivan
	author:		JPoag
	
	purpose:	SexyAppFramework Wrapper for Kanji Graphic
*********************************************************************/

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <vector>

#include "SexyRect.h"
#include "Common.h"

class KGraphic;
class KImage;

namespace Sexy
{
	enum AnimType
	{
		AnimType_None,
		AnimType_Once,
		AnimType_PingPong,
		AnimType_Loop
	};

	struct AnimInfo
	{
		AnimType				mAnimType;
		int						mFrameDelay; // 1/100s
		int						mNumCels; // Number of items in an animation strip (could be differ from SexyImage::mNumTotal)
		std::vector<int>		mPerFrameDelay;
		std::vector<int>		mFrameMap;
		int						mTotalAnimTime;

		AnimInfo();
		void SetPerFrameDelay(int theFrame, int theTime);
		void Compute(int theNumCels, int theBeginFrameTime = 0, int theEndFrameTime = 0);

		int GetPerFrameCel(int theTime);
		int GetAnimCel(int theTime);
        int GetImageCel(int theFrame);
	};

	class SexyImage
	{
	public:
		SexyString						mFilePath; // Relative path to the image file.
		SexyString						mAlphaFilePath; // Relative path to the alpha channel file.
		int								mWidth;
		int								mHeight;
		int								mNumRows; // Number of rows in an image strip.
		int								mNumCols; // Number of columns in an image strip.
        int								mNumTotal; // Number of items in an image strip.
		AnimInfo						mAnimInfo; // Info struct for animations.
		bool							mLoaded;	// Has the Image been loaded from disk?
		bool							mUploaded; // Has the texture been loaded to the graphics card?
		unsigned char*					mCompressedDataBuffer; // SexyImage data from file.
		size_t							mCompressedDataSize; // SexyImage data size.
		unsigned char*					mAlphaCompressedDataBuffer; // Alpha SexyImage data from file.
		size_t							mAlphaCompressedDataSize; // Alpha SexyImage data size.
		bool							mNoBlendOverride; // Turn off linear blending

		unsigned long*					mBits; // For manipulating the image bits.
		bool							mBitsDirty; // Have the bits been modified?
		bool							mPurgeBits; // Should we purge the bits and data?

		KGraphic*						mKGraphicImage; // For drawing the image.
		long							mWrappingMode; // {K_WRAP_CLAMP, K_WRAP_REPEAT, K_WRAP_MIRROR}

		enum
		{
			LOAD_STEP_LOAD_UPLOAD = 0,
			LOAD_STEP_LOAD_ONLY,
			LOAD_STEP_UPLOAD_ONLY
		};

	public:
		SexyImage();
		SexyImage(const SexyImage& theImage);
		SexyImage(std::wstring theFileName, bool lookForAlphaFile = true);
		SexyImage(std::string theFileName, bool lookForAlphaFile = true);
		~SexyImage();

		virtual void Create(int theWidth, int theHeight);
		virtual void Clear();

		virtual int GetWidth() const			{return mWidth;}
		virtual int GetHeight() const			{return mHeight;}
		virtual int GetRows()const				{return mNumRows;}
		virtual int GetColumns() const			{return mNumCols;}
        virtual int GetTotalCels() const		{return mNumTotal;}

		virtual KGraphic*				GetKanjiImage();
		virtual bool					OpenFile(std::wstring theFileName, bool lookForAlphaFile = true);
		virtual bool					OpenFile(std::string theFileName, bool lookForAlphaFile = true);

		virtual int						GetCelWidth(); // Returns the width of just 1 cel in a strip of images.
		virtual int						GetCelHeight();	// Returns the height of just 1 cel in a strip of images.
		virtual SexyRect				GetCelRect(int theCol, int theRow); // Gets the rectangle for the given cel at the specified row/col.
		virtual SexyRect				GetCelRect(int theCel); // Gets the rectangle for the given cel.
		virtual int						GetAnimCel(int theTime); // use animinfo to return appropriate cel to draw at the time
		virtual SexyRect				GetAnimCelRect(int theTime);

		virtual unsigned long*			GetBits();
		virtual void					SetBits(unsigned long* theBits, int theWidth, int theHeight);
		virtual void					PurgeBits(bool bPurgeBits = true);
		virtual void					CommitBits() { mBitsDirty = true; }

		virtual void					SetTextureWrappingMode(long theWrapMode = 0 /*K_WRAP_CLAMP*/);
		virtual long					GetTextureWrappingMode();

		/*!***********************************
		// @return    	bool
		//
		// \brief		Returns the Linear blending override state.  False by default.
		// \details 	<b>Description:</b> See SetNoBlendOverride()
		//************************************/
		virtual bool					GetNoBlendOverride() const { return mNoBlendOverride; }
		/*!***********************************
		// @return    	void
		// @param     	val
		//
		// \brief		Overrides the graphics settings and turns off linear blending for this 
		//				graphic.  Use for low quality graphics.
		// \details 	<b>Description:</b> Sometimes an image will run up to the edge of the 
		//				surface and linear blending will cause it to use the border color.
		//				try setting this to true if the image has black line artifacts.  The default
		//				setting is false.
		//************************************/
		virtual void					SetNoBlendOverride(bool val = true) { mNoBlendOverride = val; }

		virtual void					SetRows(int theNumRows)		{mNumRows = theNumRows;}
		virtual void					SetColumns(int theNumCols)	{mNumCols = theNumCols;}
        virtual void					SetTotalCels(int theNumTotal) {mNumTotal = theNumTotal;}

		virtual void					SetAnimInfo(const AnimInfo& theAnimInfo);
		virtual Sexy::AnimInfo			GetAnimInfo() const { return mAnimInfo; }

	private:
		virtual bool					CreateKGraphicFromData(int theStep); // 0 = Load&Upload, 1 = Load Only, 2 = Upload Only
		virtual bool					CreateKGraphicFromBits();

		virtual void					LoadAlphaMask(std::wstring theImageFilePath);
		virtual void					LoadAlphaMask(std::string theImageFilePath);
		virtual void					_PurgeBits();
	};
}

#endif // __IMAGE_H__
