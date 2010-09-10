// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji ResourceManager Version 0.4
	-------------------------------------------------------------	
	Created:	2010/03/24
	Filename: 	SexyKanji\ResourceManager.cpp
	
	Purpose:	Parse and load resources from XML file with modifiers.

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#include "ResourceManager.h"

#include "SexyAppBase.h"
#include "XMLParser.h"
#include "SoundManager.h"
#include "MusicManager.h"
#include "SexyFont.h"
#include "ImageManager.h"
#include "FontManager.h"
#ifndef K_IPHONE
#include "SexyMovie.h"
#endif

#include <errno.h>

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

//////////////////////////////////////////////////////////////////////////
// initialize static members
//////////////////////////////////////////////////////////////////////////

// create the sets for the allowed modifiers for a later check

const SexyStringSet::value_type DefaultModifiers[] = 
{
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("idprefix"))
};
const SexyStringSet ResourceManager::MODIFIERS_SETDEFAULTS(DefaultModifiers, DefaultModifiers + (sizeof DefaultModifiers / sizeof DefaultModifiers[0]));

const SexyStringSet::value_type ImageModifiers[] = 
{
	SexyStringSet::value_type(_S("id")),
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("nopurge")),
	SexyStringSet::value_type(_S("noalpha")),
	SexyStringSet::value_type(_S("alphaimage")),
	SexyStringSet::value_type(_S("variant")),
	SexyStringSet::value_type(_S("alphagrid")),
	SexyStringSet::value_type(_S("noblend")),
	SexyStringSet::value_type(_S("wrapmode")),
	SexyStringSet::value_type(_S("rows")),
	SexyStringSet::value_type(_S("cols")),
	SexyStringSet::value_type(_S("total")),
	SexyStringSet::value_type(_S("anim")),
	SexyStringSet::value_type(_S("framedelay")),
	SexyStringSet::value_type(_S("begindelay")),
	SexyStringSet::value_type(_S("enddelay")),
	SexyStringSet::value_type(_S("perframedelay")),
	SexyStringSet::value_type(_S("framemap")),
	SexyStringSet::value_type(_S("colorize")),
	SexyStringSet::value_type(_S("rotation")),
	SexyStringSet::value_type(_S("mirrorx")),
	SexyStringSet::value_type(_S("mirrory")),
	SexyStringSet::value_type(_S("hue")),
	SexyStringSet::value_type(_S("scalex")),
	SexyStringSet::value_type(_S("scaley")),
	SexyStringSet::value_type(_S("alias"))
};
const SexyStringSet ResourceManager::MODIFIERS_IMAGE(ImageModifiers, ImageModifiers + (sizeof ImageModifiers / sizeof ImageModifiers[0]));

const SexyStringSet::value_type FontModifiers[] = 
{
	SexyStringSet::value_type(_S("id")),
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("variant")),
	SexyStringSet::value_type(_S("size")),
	SexyStringSet::value_type(_S("alias"))
};
const SexyStringSet ResourceManager::MODIFIERS_FONT(FontModifiers, FontModifiers + (sizeof FontModifiers / sizeof FontModifiers[0]));

const SexyStringSet::value_type SoundModifiers[] = 
{
	SexyStringSet::value_type(_S("id")),
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("volume")),
	SexyStringSet::value_type(_S("alias"))
};
const SexyStringSet ResourceManager::MODIFIERS_SOUND(SoundModifiers, SoundModifiers + (sizeof SoundModifiers / sizeof SoundModifiers[0]));

const SexyStringSet::value_type MusicModifiers[] = 
{
	SexyStringSet::value_type(_S("id")),
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("volume")),
	SexyStringSet::value_type(_S("forceload")),
	SexyStringSet::value_type(_S("alias"))
};
const SexyStringSet ResourceManager::MODIFIERS_MUSIC(MusicModifiers, MusicModifiers + (sizeof MusicModifiers / sizeof MusicModifiers[0]));

const SexyStringSet::value_type MovieModifiers[] = 
{
	SexyStringSet::value_type(_S("id")),
	SexyStringSet::value_type(_S("path")),
	SexyStringSet::value_type(_S("preload")),
	SexyStringSet::value_type(_S("alias"))
};
const SexyStringSet ResourceManager::MODIFIERS_MOVIE(MovieModifiers, MovieModifiers + (sizeof MovieModifiers / sizeof MovieModifiers[0]));

/*!***********************************
// @return    	
//
// \brief		Constructor, initialize members
// \details 	<b>Description:</b> Constructor, initialize members
//************************************/
ResourceManager::ResourceManager() 
{
	mHasFailed						= false;
	mXMLParser						= NULL;
	mAllowMissingProgramResources	= false;
	mAllowAlreadyDefinedResources	= false;
	mCurResGroupList				= NULL;
}

/*!***********************************
// @return    	
//
// \brief		Destructor, free all
// \details 	<b>Description:</b> Destructor, free all
//************************************/
ResourceManager::~ResourceManager()
{
	DeleteMap(mImageMap);
	DeleteMap(mSoundMap);
	DeleteMap(mMusicMap);
	DeleteMap(mFontMap);
	DeleteMap(mMovieMap);
}

/*!***********************************
// @return    	void
//
// \brief		Delete image resource
// \details 	<b>Description:</b> Delete image resource
//************************************/
void ResourceManager::ImageRes::DeleteResource()
{	
	mImage.Release();
}

/*!***********************************
// @return    	void
//
// \brief		Delete sound resource
// \details 	<b>Description:</b> Delete sound resource
//************************************/
void ResourceManager::SoundRes::DeleteResource()
{
	if (mSampleID >= 0)
		gSexyAppBase->GetSoundManager()->ReleaseSample(mSampleID);

	mSampleID = -1;
}

/*!***********************************
// @return    	void
//
// \brief		Delete music resource
// \details 	<b>Description:</b> Delete music resource
//************************************/
void ResourceManager::MusicRes::DeleteResource()
{
	if (mStreamID >= 0)
		gSexyAppBase->GetMusicManager()->ReleaseStream(mStreamID);

	mStreamID = -1;
}

/*!***********************************
// @return    	void
//
// \brief		Delete font resource
// \details 	<b>Description:</b> Delete font resource
//************************************/
void ResourceManager::FontRes::DeleteResource()
{
	mFont.Release();
}

/*!***********************************
// @return    	void
//
// \brief		Delete movie resource
// \details 	<b>Description:</b> Delete movie resource
 //************************************/
#ifndef K_IPHONE
void ResourceManager::MovieRes::DeleteResource()
{
	delete mMovie;
	mMovie = NULL;
}
#endif

//////////////////////////////////////////////////////////////////////////
// INTERNAL HELPERS
//////////////////////////////////////////////////////////////////////////

/*!***********************************
// @return    	void
// @param     	theMap
//
// \brief		Delete the resources of a resource map
// \details 	<b>Description:</b> Delete the resources of a resource map
//************************************/
void ResourceManager::DeleteMap(ResMap& theMap)
{
	for (ResMap::iterator anItr = theMap.begin(); anItr != theMap.end(); ++anItr)
	{
		anItr->second->DeleteResource();
		delete anItr->second;
	}

	theMap.clear();
}

/*!***********************************
// @return    	void
// @param     	theMap
// @param     	theGroup
//
// \brief		Delete the resource of a group from the resource map
// \details 	<b>Description:</b> Delete the resource of a group from the resource map,
//				delete all resources if theGroup is empty
//************************************/
void ResourceManager::DeleteResources(ResMap& theMap, const SexyString& theGroup)
{
	for (ResMap::iterator anItr = theMap.begin(); anItr != theMap.end(); ++anItr)
	{
		if (theGroup.empty() == true || anItr->second->mResGroup == theGroup)
			anItr->second->DeleteResource();
	}
}

/*!***********************************
// @return    	bool
// @param     	theErrorText
//
// \brief		Called when an error occurred
// \details 	<b>Description:</b> Called when an error occurred, gets the line information
//				from XML parser (if possible) and writes the error to K_LOG if neccessary
//				(only ResourceManager generated errors are written to K_LOG)
//************************************/
bool ResourceManager::Fail(const SexyString& theErrorText)
{
	if (mHasFailed == false)
	{
		mHasFailed = true;

		if (mXMLParser == NULL)
		{
			mErrorString = theErrorText;
		}
		else
		{
			// get line info from parser
			int aLineNum = mXMLParser->GetCurrentLineNum();

			SexyString aLineNumStr;
			aLineNumStr = StrFormat(_S("%d"), aLineNum);

			mErrorString = theErrorText;

			if (aLineNum > 0)
				mErrorString += _S(" On Line ") + aLineNumStr;

			if (mXMLParser->GetFileName().length() > 0)
				mErrorString += _S(" in File '") + StringToSexyStringFast(mXMLParser->GetFileName()) + _S("'.");
		}

		// write error to log file, if error is resource manager generated
		if (mErrorString.find(_S("ResourceManager")) != SexyString::npos)
			K_LOG(SexyStringToStringFast(mErrorString).c_str());
	}

	return false;
}

/*!***********************************
// @return    	void
// @param     	theVal
// @param     	theVector
//
// \brief		Read an int vector from a string
// \details 	<b>Description:</b> Read an int vector from a string
//************************************/
void ResourceManager::ReadIntVector(const SexyString& theVal, std::vector<int>& theVector)
{
	theVector.clear();

	SexyString::size_type aPos = 0;

	while (true)
	{
		theVector.push_back(sexyatoi(theVal.c_str() + aPos));
		aPos = theVal.find_first_of(_S(','), aPos);

		if (aPos == SexyString::npos)
			break;

		aPos++;
	}	
}

/*!***********************************
// @return    	bool
// @param     	theElement
// @param     	theRes
// @param     	theMap
//
// \brief		Parse and save the common modifiers of a resource
// \details 	<b>Description:</b> Parse and save the common modifiers of a resource
//************************************/
bool ResourceManager::ParseCommonResource(XMLElement& theElement, BaseRes* theRes, ResMap& theMap)
{
	mHadAlreadyDefinedError = false;

	const SexyString &aPath = theElement.mAttributes[_S("path")];
	
	if (aPath.empty() == true)
		return Fail(_S("Sexy::ResourceManager::ParseCommonResource : ERROR : No path specified."));

	theRes->mXMLAttributes = theElement.mAttributes;
	theRes->mFromProgram = false;

	// check for program supplied resource
	if (aPath[0] == _S('!'))
	{
		theRes->mPath = aPath;
		if (aPath == _S("!program"))
			theRes->mFromProgram = true;
	}
	else
	{
		theRes->mPath = mDefaultPath + aPath;
	}
	
	SexyString anId;
	XMLParamMap::iterator anItr = theElement.mAttributes.find(_S("id"));

	if (anItr == theElement.mAttributes.end())
		anId = mDefaultIdPrefix + GetFileName(theRes->mPath, true);
	else
		anId = mDefaultIdPrefix + anItr->second;

	theRes->mResGroup = mCurResGroup;
	theRes->mId = anId;
	
	// store the resource information in the map
	std::pair<ResMap::iterator, bool> aRet = theMap.insert(ResMap::value_type(anId, theRes));

	if (aRet.second == false)
	{
		mHadAlreadyDefinedError = true;
		return Fail(_S("Sexy::ResourceManager::ParseCommonResource : ERROR : Resource already defined."));
	}

	mCurResGroupList->push_back(theRes);

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the modifiers of a image resource
// \details 	<b>Description:</b> Parse and save the modifiers of a image resource
//************************************/
bool ResourceManager::ParseImageResource(XMLElement& theElement)
{
	ImageRes *aRes = new ImageRes;

	if (ParseCommonResource(theElement, aRes, mImageMap) == false)
	{
		if (mHadAlreadyDefinedError && mAllowAlreadyDefinedResources)
		{
			mErrorString = _S("");
			mHasFailed = false;
			ImageRes *oldRes = aRes;
			aRes = (ImageRes*)mImageMap[oldRes->mId];
			aRes->mPath = oldRes->mPath;
			aRes->mXMLAttributes = oldRes->mXMLAttributes;
			delete oldRes;
		}
		else			
		{
			delete aRes;
			return false;
		}
	}
	
	// get the modifiers

	// (image bits are purged by default)
	aRes->mNoPurge = theElement.mAttributes.find(_S("nopurge")) == theElement.mAttributes.end();

	aRes->mLookForAlphaFile = theElement.mAttributes.find(_S("noalpha")) == theElement.mAttributes.end();	

	aRes->mNoBlend = false;
	aRes->mNoBlend = theElement.mAttributes.find(_S("noblend")) == theElement.mAttributes.end();

	aRes->mWrapMode = K_WRAP_CLAMP;
	if (theElement.mAttributes.find(_S("wrapmode")) != theElement.mAttributes.end())
	{
		SexyString aWrapMode = Lower(theElement.mAttributes[_S("wrapmode")]);

		if		(aWrapMode == _S("mirror"))	aRes->mWrapMode = K_WRAP_MIRROR;
		else if (aWrapMode == _S("repeat"))	aRes->mWrapMode = K_WRAP_REPEAT;
		else 
		{
			return Fail(_S("Sexy::ResourceManager::ParseImageResource : ERROR : Invalid wrap mode: '") + theElement.mAttributes[_S("wrapmode")] + _S("'"));
		}
	}

	XMLParamMap::iterator anItr;

	anItr = theElement.mAttributes.find(_S("variant"));
	if (anItr != theElement.mAttributes.end())
		aRes->mVariant = anItr->second;

	anItr = theElement.mAttributes.find(_S("alphaimage"));
	if (anItr != theElement.mAttributes.end())
	{
		aRes->mAlphaFile = anItr->second;
		// if alphaimage is specified, noalpha is assumed
		aRes->mLookForAlphaFile = false;
	}

	anItr = theElement.mAttributes.find(_S("alphagrid"));
	if (anItr != theElement.mAttributes.end())
	{
		aRes->mAlphaGridFile = anItr->second;
		// if alphagridimage is specified, noalpha is assumed
		aRes->mLookForAlphaFile = false;
	}

	anItr = theElement.mAttributes.find(_S("rows"));
	if (anItr != theElement.mAttributes.end())
		aRes->mRows = sexyatoi(anItr->second.c_str());
	else
		aRes->mRows = 1;

	anItr = theElement.mAttributes.find(_S("cols"));
	if (anItr != theElement.mAttributes.end())
		aRes->mCols = sexyatoi(anItr->second.c_str());
	else
		aRes->mCols = 1;

	anItr = theElement.mAttributes.find(_S("total"));
	if (anItr != theElement.mAttributes.end())
		aRes->mNumCells = sexyatoi(anItr->second.c_str());
	else
		aRes->mNumCells = aRes->mRows * aRes->mCols;

	anItr = theElement.mAttributes.find(_S("anim"));

	AnimType anAnimType = AnimType_None;

	if (anItr != theElement.mAttributes.end())
	{
		SexyString aType = Lower(anItr->second.c_str());

		if		(aType == _S("none"))		anAnimType = AnimType_None;
		else if (aType == _S("once"))		anAnimType = AnimType_Once;
		else if (aType == _S("loop"))		anAnimType = AnimType_Loop;
		else if (aType == _S("pingpong"))	anAnimType = AnimType_PingPong;
		else 
		{
			return Fail(_S("Sexy::ResourceManager::ParseImageResource : ERROR : Invalid animation type: '") + anItr->second + _S("'"));
		}
	}

    // always set default mNumCels
    aRes->mAnimInfo.mNumCels = aRes->mNumCells;
    // animation type
	aRes->mAnimInfo.mAnimType = anAnimType;

	if (anAnimType != AnimType_None)
	{
		int aBeginDelay = 0, anEndDelay = 0;

		anItr = theElement.mAttributes.find(_S("framedelay"));
		if (anItr != theElement.mAttributes.end())
			aRes->mAnimInfo.mFrameDelay = sexyatoi(anItr->second.c_str());

		anItr = theElement.mAttributes.find(_S("begindelay"));
		if (anItr != theElement.mAttributes.end())
			aBeginDelay = sexyatoi(anItr->second.c_str());

		anItr = theElement.mAttributes.find(_S("enddelay"));
		if (anItr != theElement.mAttributes.end())
			anEndDelay = sexyatoi(anItr->second.c_str());

		anItr = theElement.mAttributes.find(_S("perframedelay"));
		if (anItr != theElement.mAttributes.end())
			ReadIntVector(anItr->second,aRes->mAnimInfo.mPerFrameDelay);

		anItr = theElement.mAttributes.find(_S("framemap"));
		if (anItr != theElement.mAttributes.end())
			ReadIntVector(anItr->second,aRes->mAnimInfo.mFrameMap);

		// (Compute sets AnimInfo.mNumCels = aRes->mNumCells)
		aRes->mAnimInfo.Compute(aRes->mNumCells, aBeginDelay, anEndDelay);
	}

	// colorize
	anItr = theElement.mAttributes.find(_S("colorize"));
	if (anItr != theElement.mAttributes.end())
	{
		// r,g,b,a
		std::vector<int> aColorVector;
		ReadIntVector(anItr->second, aColorVector);

		if (aColorVector.size() == 4 &&
			aColorVector[0] >= 0 && aColorVector[0] <= 255 &&
			aColorVector[1] >= 0 && aColorVector[1] <= 255 &&
			aColorVector[2] >= 0 && aColorVector[2] <= 255 &&
			aColorVector[3] >= 0 && aColorVector[3] <= 255)
		{
			aRes->mColorizeColor = SexyColor(aColorVector[0], aColorVector[1], aColorVector[2], aColorVector[3]);
		}
		else
			return Fail(_S("Sexy::ResourceManager::ParseImageResource : ERROR : Colorize color out of range."));
	}
	else
		aRes->mColorizeColor = SexyColor(-1, -1, -1, -1);

	// rotation
	anItr = theElement.mAttributes.find(_S("rotation"));
	if (anItr != theElement.mAttributes.end())
	{
		int aRotation = sexyatoi(anItr->second.c_str());
		
		if (aRotation == ImageManager::ROT_IMG_90_DEG  || 
			aRotation == ImageManager::ROT_IMG_180_DEG ||
			aRotation == ImageManager::ROT_IMG_270_DEG)
			aRes->mRotation = aRotation;
		else
			return Fail(_S("Sexy::ResourceManager::ParseImageResource : ERROR : Invalid rotation value."));

	}
	else
		aRes->mRotation = 0;

	// mirror X, mirror Y
	aRes->mMirrorX = theElement.mAttributes.find(_S("mirrorx")) != theElement.mAttributes.end();
	aRes->mMirrorY = theElement.mAttributes.find(_S("mirrory")) != theElement.mAttributes.end();

	// hue
	anItr = theElement.mAttributes.find(_S("hue"));
	if (anItr != theElement.mAttributes.end())
		aRes->mDeltaHue = sexyatoi(anItr->second.c_str());
	else
		aRes->mDeltaHue = 0;

	// scale, save as SexyString
	anItr = theElement.mAttributes.find(_S("scalex"));
	if (anItr != theElement.mAttributes.end())
		aRes->mScaleX = anItr->second;
	else
		aRes->mScaleX = _S("");

	anItr = theElement.mAttributes.find(_S("scaley"));
	if (anItr != theElement.mAttributes.end())
		aRes->mScaleY = anItr->second;
	else
		aRes->mScaleY = _S("");

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the modifiers of a sound resource
// \details 	<b>Description:</b> Parse and save the modifiers of a sound resource
//************************************/
bool ResourceManager::ParseSoundResource(XMLElement& theElement)
{
	SoundRes* aRes	= new SoundRes;
	aRes->mSampleID	= -1;
	aRes->mVolume	= -1;

	if (ParseCommonResource(theElement, aRes, mSoundMap) == false)
	{
		if (mHadAlreadyDefinedError && mAllowAlreadyDefinedResources)
		{
			mErrorString = _S("");
			mHasFailed = false;
			SoundRes *oldRes = aRes;
			aRes = (SoundRes*)mSoundMap[oldRes->mId];
			aRes->mPath = oldRes->mPath;
			aRes->mXMLAttributes = oldRes->mXMLAttributes;
			delete oldRes;
		}
		else			
		{
			delete aRes;
			return false;
		}
	}

	XMLParamMap::iterator anItr;

	anItr = theElement.mAttributes.find(_S("volume"));

	if (anItr != theElement.mAttributes.end())
	{
		aRes->mVolume = sexyatoi(anItr->second.c_str());

		if (aRes->mVolume < MIN_VOLUME || aRes->mVolume > MAX_VOLUME)
			return Fail(_S("Sexy::ResourceManager::ParseSoundResource : ERROR : Failed to load sound: ") + aRes->mPath + _S(", modifier volume must be in range [MIN_VOLUME..MAX_VOLUME]."));
	}
	else
		aRes->mVolume = MAX_VOLUME;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the modifiers of a music resource
// \details 	<b>Description:</b> Parse and save the modifiers of a music resource
//************************************/
bool ResourceManager::ParseMusicResource(XMLElement& theElement)
{
	MusicRes* aRes	= new MusicRes;
	aRes->mStreamID	= -1;
	aRes->mVolume	= -1;

	if (ParseCommonResource(theElement, aRes, mMusicMap) == false)
	{
		if (mHadAlreadyDefinedError && mAllowAlreadyDefinedResources)
		{
			mErrorString = _S("");
			mHasFailed = false;
			MusicRes *oldRes = aRes;
			aRes = (MusicRes*)mMusicMap[oldRes->mId];
			aRes->mPath = oldRes->mPath;
			aRes->mXMLAttributes = oldRes->mXMLAttributes;
			delete oldRes;
		}
		else			
		{
			delete aRes;
			return false;
		}
	}

	XMLParamMap::iterator anItr;

	anItr = theElement.mAttributes.find(_S("volume"));

	if (anItr != theElement.mAttributes.end())
	{
		aRes->mVolume = sexyatoi(anItr->second.c_str());

		if (aRes->mVolume < MIN_VOLUME || aRes->mVolume > MAX_VOLUME)
			return Fail(_S("Sexy::ResourceManager::ParseMusicResource : ERROR : Failed to load stream: ") + aRes->mPath + _S(", modifier volume must be in range [MIN_VOLUME..MAX_VOLUME]"));
	}
	else
		aRes->mVolume = MAX_VOLUME;

	aRes->mForceLoad = theElement.mAttributes.find(_S("forceload")) != theElement.mAttributes.end();

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the modifiers of a font resource
// \details 	<b>Description:</b> Parse and save the modifiers of a font resource
//************************************/
bool ResourceManager::ParseFontResource(XMLElement& theElement)
{
	FontRes *aRes = new FontRes;

	if (ParseCommonResource(theElement, aRes, mFontMap) == false)
	{
		if (mHadAlreadyDefinedError && mAllowAlreadyDefinedResources)
		{
			mErrorString = _S("");
			mHasFailed = false;
			FontRes *oldRes = aRes;
			aRes = (FontRes*)mFontMap[oldRes->mId];
			aRes->mPath = oldRes->mPath;
			aRes->mXMLAttributes = oldRes->mXMLAttributes;
			delete oldRes;
		}
		else			
		{
			delete aRes;
			return false;
		}
	}

	XMLParamMap::iterator anItr;

	anItr = theElement.mAttributes.find(_S("variant"));
	if (anItr != theElement.mAttributes.end())
		aRes->mVariant = anItr->second;

	// get file extension
	int aLastDotPos = aRes->mPath.rfind('.');
	SexyString anExt = aRes->mPath.substr(aLastDotPos, aRes->mPath.length() - aLastDotPos);
	anExt = StringToLower(anExt);

	// is it a FreeType font ? -> need modifier "size"
	if (anExt == _S(".ttf"))
	{
		anItr = theElement.mAttributes.find(_S("size"));

		if (anItr == theElement.mAttributes.end())
			return Fail(_S("Sexy::ResourceManager::ParseFontResource : ERROR : FreeType Font needs modifier 'size'."));

		aRes->mSize = sexyatoi(anItr->second.c_str());

		if (aRes->mSize <= 0)
			return Fail(_S("Sexy::ResourceManager::ParseFontResource : ERROR : Free Type Font modifier 'size' out of range."));

		aRes->mIsFreeType = true;
	}
	else
		aRes->mIsFreeType = false;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the modifiers of a movie resource
// \details 	<b>Description:</b> Parse and save the modifiers of a movie resource
 //************************************/
#ifndef K_IPHONE
bool ResourceManager::ParseMovieResource(XMLElement& theElement)
{
	MovieRes *aRes	= new MovieRes;
	aRes->mMovie	= NULL;

	if (ParseCommonResource(theElement, aRes, mMovieMap) == false)
	{
		if (mHadAlreadyDefinedError && mAllowAlreadyDefinedResources)
		{
			mErrorString = _S("");
			mHasFailed = false;
			MovieRes *oldRes = aRes;
			aRes = (MovieRes*)mMovieMap[oldRes->mId];
			aRes->mPath = oldRes->mPath;
			aRes->mXMLAttributes = oldRes->mXMLAttributes;
			delete oldRes;
		}
		else			
		{
			delete aRes;
			return false;
		}
	}

	// preload modifier:
	// true to entirely load the compressed video file in memory (and avoid disk access during playback),
	// false to stream from disk as needed
	aRes->mPreload = theElement.mAttributes.find(_S("preload")) != theElement.mAttributes.end();	

	return true;
}
#endif

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Parse and save the SetDefaults modifiers
// \details 	<b>Description:</b> Parse and save the SetDefaults modifiers
//************************************/
bool ResourceManager::ParseSetDefaults(XMLElement& theElement)
{
	XMLParamMap::iterator anItr = theElement.mAttributes.find(_S("path"));
	
	if (anItr != theElement.mAttributes.end())
		mDefaultPath = StringToSexyStringFast(RemoveTrailingSlash(SexyStringToStringFast(anItr->second)) + '/');

	anItr = theElement.mAttributes.find(_S("idprefix"));
	
	if (anItr != theElement.mAttributes.end())
		mDefaultIdPrefix = StringToSexyStringFast(RemoveTrailingSlash(SexyStringToStringFast(anItr->second)));	

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theElement
//
// \brief		Check the allowed modifier
// \details 	<b>Description:</b> Check the allowed modifier
//************************************/
bool ResourceManager::CheckResourceAttributes(XMLElement& theElement)
{
	for (XMLParamMap::iterator anItr = theElement.mAttributes.begin(); anItr != theElement.mAttributes.end(); ++anItr)
	{
		if (theElement.mValue == _S("SetDefaults"))
		{
			if (MODIFIERS_SETDEFAULTS.find(anItr->first) == MODIFIERS_SETDEFAULTS.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown SetDefaults modifier: '") + anItr->first + _S("'."));

		}
		else if (theElement.mValue == _S("Image"))
		{
			if (MODIFIERS_IMAGE.find(anItr->first) == MODIFIERS_IMAGE.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown image modifier: '") + anItr->first + _S("'."));
		}
		else if (theElement.mValue == _S("Font"))
		{
			if (MODIFIERS_FONT.find(anItr->first) == MODIFIERS_FONT.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown font modifier: '") + anItr->first + _S("'."));
		}
		else if (theElement.mValue == _S("Sound"))
		{
			if (MODIFIERS_SOUND.find(anItr->first) == MODIFIERS_SOUND.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown sound modifier: '") + anItr->first + _S("'."));
		}
		else if (theElement.mValue == _S("Music"))
		{
			if (MODIFIERS_MUSIC.find(anItr->first) == MODIFIERS_MUSIC.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown music modifier: '") + anItr->first + _S("'."));
		}
		else if (theElement.mValue == _S("Movie"))
		{
			if (MODIFIERS_MOVIE.find(anItr->first) == MODIFIERS_MOVIE.end())
				return Fail(_S("Sexy::ResourceManager::CheckResourceAttributes : ERROR : Unknown movie modifier: '") + anItr->first + _S("'."));
		}
	}

	return true;
}

/*!***********************************
// @return    	bool
//
// \brief		Iterate through the XML and parse the appropriate resource type
// \details 	<b>Description:</b> Iterate through the XML and parse the appropriate resource type
//************************************/
bool ResourceManager::ParseResources()
{
	for (;;)
	{
		XMLElement aXMLElement;
		if (mXMLParser->NextElement(&aXMLElement) == false)
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (CheckResourceAttributes(aXMLElement) == false)
				return false;

			if (aXMLElement.mValue == _S("Image"))
			{
				if (ParseImageResource(aXMLElement) == false)
					return false;

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else if (aXMLElement.mValue == _S("Sound"))
			{
				if (ParseSoundResource(aXMLElement) == false)
					return false;

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else if (aXMLElement.mValue == _S("Font"))
			{
				if (ParseFontResource(aXMLElement) == false)
					return false;

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else if (aXMLElement.mValue == _S("SetDefaults"))
			{
				if (ParseSetDefaults(aXMLElement) == false)
					return false;

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else if (aXMLElement.mValue == _S("Music"))
			{
				if (ParseMusicResource(aXMLElement) == false)
					return false;

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else if (aXMLElement.mValue == _S("Movie"))
			{
#ifndef K_IPHONE
				if (ParseMovieResource(aXMLElement) == false)
					return false;
#endif

				if (mXMLParser->NextElement(&aXMLElement) == false)
					return false;

				if (aXMLElement.mType != XMLElement::TYPE_END)
					return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Unexpected element found '") + aXMLElement.mValue + _S("'."));
			}
			else
			{
				return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Invalid Section '") + aXMLElement.mValue + _S("'."));
			}
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			return Fail(_S("Sexy::ResourceManager::ParseResources : ERROR : Element Not Expected '") + aXMLElement.mValue + _S("'."));
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

/*!***********************************
// @return    	bool
//
// \brief		Parse all resource groups in the XML file
// \details 	<b>Description:</b> Parse all resource groups in the XML file
//************************************/
bool ResourceManager::DoParseResources()
{
	if (mXMLParser->HasFailed() == false)
	{
		for (;;)
		{
			XMLElement aXMLElement;
			if (mXMLParser->NextElement(&aXMLElement) == false)
				break;

			if (aXMLElement.mType == XMLElement::TYPE_START)
			{
				if (aXMLElement.mValue == _S("Resources"))
				{
					mCurResGroup = aXMLElement.mAttributes[_S("id")];
					mCurResGroupList = &mResGroupMap[mCurResGroup];

					if (mCurResGroup.empty() == true)
					{
						Fail(_S("Sexy::ResourceManager::DoParseResources : ERROR : No id specified."));
						break;
					}

					if (ParseResources() == false)
						break;
				}
				else 
				{
					Fail(_S("Sexy::ResourceManager::DoParseResources : ERROR : Invalid Section '") + aXMLElement.mValue + _S("'."));
					break;
				}
			}
			else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
			{
				Fail(_S("Sexy::ResourceManager::DoParseResources : ERROR : Element Not Expected '") + aXMLElement.mValue + _S("'."));
				break;
			}
		}
	}

	if (mXMLParser->HasFailed() == true)
		Fail(_S("Sexy::ResourceManager::DoParseResources : ERROR : ") + mXMLParser->GetErrorText());

	delete mXMLParser;
	mXMLParser = NULL;

	return !mHasFailed;
}

/*!***********************************
// @return    	bool
// @param     	theRes
// @param     	theImage
//
// \brief		Load and apply an alpha grid image to an image resource
// \details 	<b>Description:</b> Load and apply an alpha grid image to an image resource
//************************************/
bool ResourceManager::LoadAlphaGridImage(ImageRes* theRes, SexyImage* theImage)
{	
	SexyImage* anAlphaImage = gSexyAppBase->GetImageManager()->GetImage(theRes->mAlphaGridFile, true);	

	if (anAlphaImage == NULL)
		return Fail(_S("Failed to load AlphaGridImage: ") + theRes->mAlphaGridFile);

	std::auto_ptr<SexyImage> aDelAlphaImage(anAlphaImage);

	int aNumRows = theRes->mRows;
	int aNumCols = theRes->mCols;

	int aCelWidth = theImage->GetWidth()	/ aNumCols;
	int aCelHeight = theImage->GetHeight()	/ aNumRows;

	if (anAlphaImage->GetWidth() != aCelWidth || anAlphaImage->GetHeight() != aCelHeight)
		return Fail(_S("Sexy::ResourceManager::LoadAlphaGridImage : ERROR : AlphaGridImage size mismatch between ") + theRes->mPath + _S(" and ") + theRes->mAlphaGridFile);

	unsigned long *aMasterRowPtr = theImage->GetBits();

	for (int i=0; i < aNumRows; i++)
	{
		unsigned long *aMasterColPtr = aMasterRowPtr;
		for (int j=0; j < aNumCols; j++)
		{
			unsigned long* aRowPtr = aMasterColPtr;
			unsigned long* anAlphaBits = anAlphaImage->GetBits();

			for (int y=0; y<aCelHeight; y++)
			{
				unsigned long *aDestPtr = aRowPtr;
				for (int x=0; x<aCelWidth; x++)
				{
					*aDestPtr = (*aDestPtr & 0x00FFFFFF) | ((*anAlphaBits & 0xFF) << 24);
					++anAlphaBits;
					++aDestPtr;
				}
				aRowPtr += theImage->GetWidth();
			}

			aMasterColPtr += aCelWidth;
		}
		aMasterRowPtr += aCelHeight*theImage->GetWidth();
	}

	theImage->CommitBits();

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theRes
// @param     	theImage
//
// \brief		Load and apply an alpha image to an image resource
// \details 	<b>Description:</b> Load and apply an alpha image to an image resource
//************************************/
bool ResourceManager::LoadAlphaImage(ImageRes *theRes, SexyImage* theImage)
{
	SexyImage* anAlphaImage = gSexyAppBase->GetImageManager()->GetImage(theRes->mAlphaFile, true);

	if (anAlphaImage == NULL)
		return Fail(_S("Failed to load alpha image: ") + theRes->mAlphaFile);

	std::auto_ptr<SexyImage> aDelAlphaImage(anAlphaImage);

	if (anAlphaImage->GetWidth() != theImage->GetWidth() || anAlphaImage->GetHeight() != theImage->GetHeight())
		return Fail(_S("Sexy::ResourceManager::LoadAlphaImage : ERROR : AlphaImage size mismatch between ") + theRes->mPath + _S(" and ") + theRes->mAlphaFile);

	unsigned long* aBits1 = theImage->GetBits();
	unsigned long* aBits2 = anAlphaImage->GetBits();
	int aSize = theImage->GetWidth() * theImage->GetHeight();

	for (int i = 0; i < aSize; i++)
	{
		*aBits1 = (*aBits1 & 0x00FFFFFF) | ((*aBits2 & 0xFF) << 24);
		++aBits1;
		++aBits2;
	}

	theImage->CommitBits();

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theRes
//
// \brief		Load a image and apply modifiers
// \details 	<b>Description:</b> Load a image and apply modifiers
//************************************/
bool ResourceManager::DoLoadImage(ImageRes* theRes)
{
	bool isNew;

	SharedImageRef aSharedImageRef = gSexyAppBase->GetImageManager()->GetSharedImage(theRes->mPath, 
													theRes->mVariant, theRes->mLookForAlphaFile, &isNew);

	SexyImage* aImage = (SexyImage*)aSharedImageRef;

	if (aImage == NULL)
		return Fail(_S("Failed to load image: ") + theRes->mPath);

	if (isNew == true)
	{
		if (theRes->mAlphaFile.empty() == false)
		{
			if (LoadAlphaImage(theRes, aSharedImageRef) == false)
				return false;
		}

		if (theRes->mAlphaGridFile.empty() == false)
		{
			if (LoadAlphaGridImage(theRes, aSharedImageRef) == false)
				return false;
		}
	}

	aImage->CommitBits();
	theRes->mImage = aSharedImageRef;
	aImage->PurgeBits(theRes->mNoPurge);

	if (theRes->mAnimInfo.mAnimType != AnimType_None)
		aImage->SetAnimInfo(AnimInfo(theRes->mAnimInfo));

	aImage->SetRows(theRes->mRows);
	aImage->SetColumns(theRes->mCols);
    aImage->SetTotalCels(theRes->mNumCells);

	// blending, wrapping mode
	if(theRes->mNoBlend) aImage->SetNoBlendOverride();
	if(theRes->mWrapMode != K_WRAP_CLAMP) 
		aImage->SetTextureWrappingMode(theRes->mWrapMode);

	// colorize
	if (theRes->mColorizeColor.GetRed() != -1)
		gSexyAppBase->GetImageManager()->ColorizeImage(aImage, theRes->mColorizeColor);

	// rotation
	if (theRes->mRotation != 0.0f)
		gSexyAppBase->GetImageManager()->RotateImage(aImage, theRes->mRotation);

	// mirror X
	if (theRes->mMirrorX == true)
		gSexyAppBase->GetImageManager()->MirrorImage(aImage);

	// mirror Y
	if (theRes->mMirrorY == true)
		gSexyAppBase->GetImageManager()->FlipImage(aImage);

	// hue
	if (theRes->mDeltaHue != 0)
		gSexyAppBase->GetImageManager()->RotateImageHue(aImage, theRes->mDeltaHue);

	// scale (values can be: "50%" "300px" "0.65")
	// the scale values are parsed here (and not in ParseImageResource()) 
	// because we need the image dimension to calc the scale if the value is given in pixel
	// disadvantage: no line numbers from XML reader here...
	float aScaleX = 1.0f;

	if (theRes->mScaleX != _S(""))
	{
		SexyString::size_type aPos;
		int aRet;

		// find suffix
		if ((aPos = theRes->mScaleX.find('%')) != SexyString::npos)
		{
			// percentage
			aRet = sexyatoi(theRes->mScaleX.substr(0, aPos).c_str());

			if (aRet == 0 /*|| errno == ERANGE*/)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale X out of range: ") + theRes->mId);
			else
			{
				aScaleX = static_cast<float>(aRet);
				aScaleX /= 100.0f;
			}
		}
		else if ((aPos = theRes->mScaleX.find(_S("px"))) != SexyString::npos)
		{
			// pixel
			aRet = sexyatoi(theRes->mScaleX.substr(0, aPos).c_str());

			if (aRet == 0 || errno == ERANGE)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale X out of range: ") + theRes->mId);
			else
			{
				aScaleX = static_cast<float>(aRet);
				aScaleX /= aImage->GetWidth();
			}
		}
		else
		{
			// scalar value
			if ((aRet = sexysscanf(theRes->mScaleX.c_str(), _S("%f"), &aScaleX)) == EOF || aRet != 1)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale X out of range: ") + theRes->mId);
		}
	}

	float aScaleY = 1.0f;

	if (theRes->mScaleY != _S(""))
	{
		SexyString::size_type aPos;
		int aRet;

		// find suffix
		if ((aPos = theRes->mScaleY.find('%')) != SexyString::npos)
		{
			// percentage
			aRet = sexyatoi(theRes->mScaleY.substr(0, aPos).c_str());

			if (aRet == 0 || errno == ERANGE)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale Y out of range: ") + theRes->mId);
			else
			{
				aScaleY = static_cast<float>(aRet);
				aScaleY /= 100.0f;
			}
		}
		else if ((aPos = theRes->mScaleY.find(_S("px"))) != SexyString::npos)
		{
			// pixel
			aRet = sexyatoi(theRes->mScaleY.substr(0, aPos).c_str());

			if (aRet == 0 || errno == ERANGE)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale Y out of range: ") + theRes->mId);
			else
			{
				aScaleY = static_cast<float>(aRet);
				aScaleY /= aImage->GetHeight();
			}
		}
		else
		{
			// scalar value
			if ((aRet = sexysscanf(theRes->mScaleY.c_str(), _S("%f"), &aScaleY)) == EOF || aRet != 1)
				return Fail(_S("Sexy::ResourceManager::DoLoadImage : ERROR : Scale Y out of range: ") + theRes->mId);
		}
	}

	// call scale
	if (aScaleX != 1.0f || aScaleY != 1.0f)
		gSexyAppBase->GetImageManager()->ScaleImage(aImage, aScaleX, aScaleY);

	ResourceLoadedHook(theRes);

	return true;
}

/*!***********************************
// @return    	void
// @param     	theName
//
// \brief		Delete an image
// \details 	<b>Description:</b> Delete an image
//************************************/
void ResourceManager::DeleteImage(const SexyString& theName)
{
	ReplaceImage(theName, NULL);
}

/*!***********************************
// @return    	Sexy::SharedImageRef
// @param     	theName
//
// \brief		Start loading an image
// \details 	<b>Description:</b> Start loading an image
//************************************/
SharedImageRef ResourceManager::LoadImage(const SexyString& theName)
{
	ResMap::iterator anItr = mImageMap.find(theName);

	// image found?
	if (anItr == mImageMap.end())
		return NULL;

	ImageRes *aRes = (ImageRes*)anItr->second;

	// image already loaded?
	if ((SexyImage*)aRes->mImage != NULL) 
		return aRes->mImage;

	if (aRes->mFromProgram == true)
		return NULL;

	// load the image
	if (DoLoadImage(aRes) == false)
		return NULL;

	return aRes->mImage;
}

/*!***********************************
// @return    	bool
// @param     	theRes
//
// \brief		Load a sample
// \details 	<b>Description:</b> Load a sample
//************************************/
bool ResourceManager::DoLoadSound(SoundRes* theRes)
{
	int aSampleID = gSexyAppBase->GetSoundManager()->LoadSample(theRes->mPath, theRes->mVolume);

	if (aSampleID == -1)
		return Fail(_S("Failed to load sound: ") + theRes->mPath);

	theRes->mSampleID = aSampleID;

	ResourceLoadedHook(theRes);

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theRes
//
// \brief		Load a stream
// \details 	<b>Description:</b> Load a stream
//************************************/
bool ResourceManager::DoLoadMusic(MusicRes* theRes)
{
	int aStreamID = gSexyAppBase->GetMusicManager()->AutoLoadStream(theRes->mPath, theRes->mForceLoad);

	if (aStreamID == -1)
		return Fail(_S("Failed to load stream: ") + theRes->mPath);

	gSexyAppBase->GetMusicManager()->SetStreamVolume(aStreamID, theRes->mVolume);
	
	theRes->mStreamID = aStreamID;

	ResourceLoadedHook(theRes);

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theRes
//
// \brief		Load a font
// \details 	<b>Description:</b> Load a font
//************************************/
bool ResourceManager::DoLoadFont(FontRes* theRes)
{
	bool isNew;

	SharedFontRef aSharedFontRef = gSexyAppBase->GetFontManager()->GetSharedFont(theRes->mPath, theRes->mVariant, &isNew);

	SexyFont* aFont = (SexyFont*)aSharedFontRef;

	if (aFont == NULL)
		return Fail(_S("Failed to load font: ") + theRes->mPath);

	theRes->mFont = aSharedFontRef;

	// FreeType fonts MUST have a size
	if (theRes->mIsFreeType == true)
	{
		aFont->SetFontPointHeight(theRes->mSize);
	}

	ResourceLoadedHook(theRes);

	return true;
}

/*!***********************************
// @return    	void
// @param     	theName
//
// \brief		Delete a font
// \details 	<b>Description:</b> Delete a font
//************************************/
void ResourceManager::DeleteFont(const SexyString& theName)
{
	ReplaceFont(theName, NULL);
}

/*!***********************************
// @return    	SharedFontRef
// @param     	theName
//
// \brief		Start loading a font
// \details 	<b>Description:</b> Start loading a font
//************************************/
SharedFontRef ResourceManager::LoadFont(const SexyString &theName)
{
	ResMap::iterator anItr = mFontMap.find(theName);

	if (anItr == mFontMap.end())
		return NULL;

	FontRes* aRes = (FontRes*)anItr->second;

	if (aRes->mFont != NULL)
		return aRes->mFont;

	if (aRes->mFromProgram == true)
		return NULL;

	if (DoLoadFont(aRes) == false)
		return NULL;

	return aRes->mFont;
}

/*!***********************************
// @return    	bool
// @param     	theRes
//
// \brief		Load a Movie
// \details 	<b>Description:</b> Load a Movie
 //************************************/
#ifndef K_IPHONE
bool ResourceManager::DoLoadMovie(MovieRes* theRes)
{
	theRes->mMovie = new SexyMovie();
	
	if (theRes->mMovie == NULL)
		return Fail(_S("Sexy::ResourceManager::DoLoadMovie : ERROR : Failed to load Movie: ") + theRes->mPath);

	if (theRes->mMovie->OpenFile(theRes->mPath, theRes->mPreload) == false)
	{
		delete theRes->mMovie;
		return Fail(_S("Sexy::ResourceManager::DoLoadMovie : ERROR : Failed to load Movie: ") + theRes->mPath);
	}

	ResourceLoadedHook(theRes);

	return true; 
}
#endif

//////////////////////////////////////////////////////////////////////////
// ACCESS METHODS
//////////////////////////////////////////////////////////////////////////

/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		Search for ResourceManifest tag and start parsing
// \details 	<b>Description:</b> Search for ResourceManifest tag and start parsing
//************************************/
bool ResourceManager::ParseResourcesFile(const SexyString& theFilename)
{
	mXMLParser = new XMLParser();

	if (mXMLParser->OpenFile(theFilename) == false)
		Fail(_S("Sexy::ResourceManager::ParseResourcesFile : ERROR : Resource file not found: ") + theFilename);

	XMLElement aXMLElement;
	while (mXMLParser->HasFailed() == false)
	{
		if (mXMLParser->NextElement(&aXMLElement) == false)
			Fail(_S("Sexy::ResourceManager::ParseResourcesFile : ERROR : ") + mXMLParser->GetErrorText());

		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (aXMLElement.mValue != _S("ResourceManifest"))
				break;
			else
				return DoParseResources();
		}
	}

	Fail(_S("Sexy::ResourceManager::ParseResourceFile : ERROR : Expecting ResourceManifest tag."));

	return false;	
}

/*!***********************************
// @return    	bool
// @param     	theFilename
//
// \brief		Re-parse a already parsed resource file
// \details 	<b>Description:</b> Re-parse a already parsed resource file
//************************************/
bool ResourceManager::ReparseResourcesFile(const SexyString& theFilename)
{
	bool oldDefined = mAllowAlreadyDefinedResources;

	mAllowAlreadyDefinedResources = true;

	bool aResult = ParseResourcesFile(theFilename);

	mAllowAlreadyDefinedResources = oldDefined;

	return aResult;
}

/*!***********************************
// @return    	bool
//
// \brief		Load next resource with appropriate resource type from current group
// \details 	<b>Description:</b> Load next resource with appropriate resource type from current group
//************************************/
bool ResourceManager::LoadNextResource()
{
	if (HadError() == true)
		return false;

	if (mCurResGroupList == NULL)
		return false;

	while (mCurResGroupListItr != mCurResGroupList->end())
	{
		BaseRes* aRes = *mCurResGroupListItr++;

		if (aRes->mFromProgram == true)
			continue;

		switch (aRes->mType)
		{
			case ResType_Image: 
			{
				ImageRes* anImageRes = (ImageRes*)aRes;

				if ((SexyImage*)anImageRes->mImage != NULL) 
					continue;

				return DoLoadImage(anImageRes); 
			}
			
			case ResType_Sound: 
			{
				SoundRes* aSoundRes = (SoundRes*)aRes;

				if (aSoundRes->mSampleID != -1)
					continue;

				return DoLoadSound(aSoundRes); 
			}

			case ResType_Music: 
			{
				MusicRes* aMusicRes = (MusicRes*)aRes;

				if (aMusicRes->mStreamID != -1)
					continue;

				return DoLoadMusic(aMusicRes); 
			}
			
			case ResType_Font: 
			{
				FontRes* aFontRes = (FontRes*)aRes;

				if (aFontRes->mFont != NULL)
					continue;

				return DoLoadFont(aFontRes);
			}
				
#ifndef K_IPHONE
			case ResType_Movie: 
			{
				MovieRes* aMovieRes = (MovieRes*)aRes;

				if (aMovieRes->mMovie != NULL)
					continue;

				return DoLoadMovie(aMovieRes);
			}
#endif
		}
	}

	return false;
}

/*!***********************************
// @return    	void
// @param     	theRes
//
// \brief		Notification that a resource finished loading
// \details 	<b>Description:</b> Notification that a resource finished loading
//************************************/
void ResourceManager::ResourceLoadedHook(BaseRes* theRes)
{

}

/*!***********************************
// @return    	void
// @param     	theGroup
//
// \brief		Set the prerequisites for LoadNextResource()
// \details 	<b>Description:</b> Set the prerequisites for LoadNextResource()
//************************************/
void ResourceManager::StartLoadResources(const SexyString& theGroup)
{
	mErrorString = _S("");
	mHasFailed = false;

	mCurResGroup = theGroup;
	mCurResGroupList = &mResGroupMap[theGroup];
	mCurResGroupListItr = mCurResGroupList->begin();
}

/*!***********************************
// @return    	bool
// @param     	theGroup
//
// \brief		Load all resources of a group
// \details 	<b>Description:</b> Load all resources of a group
//************************************/
bool ResourceManager::LoadResources(const SexyString& theGroup)
{
	mErrorString = _S("");
	mHasFailed = false;

	StartLoadResources(theGroup);

	while (LoadNextResource() == true)
	{
		// twiddle thumbs 
	}

	if (HadError() == false)
	{
		mLoadedGroups.insert(theGroup);
		return true;
	}
	else
		return false;
}

/*!***********************************
// @return    	void
// @param     	theExit
//
// \brief		Display a message box with error information 
// \details 	<b>Description:</b> Display a message box with error information, 
//				initiate shutdown if theExit is true. 
//				Remark: ResourceManager does NOT display message box dialogs,
//				thats the role of the caller.
//************************************/
void ResourceManager::ShowResourceError(bool theExit)
{
	KMiscTools::messageBox("ResourceManager Error", SexyStringToStringFast(GetErrorText()).c_str());

	if (theExit == true)
	{
		gSexyAppBase->Shutdown();
	}
}

/*!***********************************
// @return    	void
// @param     	theGroup
//
// \brief		Delete all resources of a group
// \details 	<b>Description:</b> Delete all resources of a group
//************************************/
void ResourceManager::DeleteResources(const SexyString& theGroup)
{
	DeleteResources(mImageMap, theGroup);
	DeleteResources(mSoundMap, theGroup);
	DeleteResources(mMusicMap, theGroup);
	DeleteResources(mFontMap,  theGroup);
	DeleteResources(mMovieMap, theGroup);

	mLoadedGroups.erase(theGroup);
}

/*!***********************************
// @return    	int
// @param     	theGroup
// @param     	theMap
//
// \brief		
// \details 	<b>Description:</b> Helper to get the number of resources of a group
//************************************/
int	ResourceManager::GetNumResources(const SexyString& theGroup, ResMap& theMap)
{
	if (theGroup.empty() == true)
		return theMap.size();

	int aCount = 0;

	for (ResMap::iterator anItr = theMap.begin(); anItr != theMap.end(); ++anItr)
	{
		BaseRes *aRes = anItr->second;
		if (aRes->mResGroup==theGroup && !aRes->mFromProgram)
			++aCount;
	}

	return aCount;
}

/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of images in a group
// \details 	<b>Description:</b> Get the number of images in a group
//************************************/
int	ResourceManager::GetNumImages(const SexyString& theGroup)
{
	return GetNumResources(theGroup, mImageMap);
}
	
/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of samples in a group
// \details 	<b>Description:</b> Get the number of samples in a group
//************************************/
int	ResourceManager::GetNumSounds(const SexyString& theGroup)
{
	return GetNumResources(theGroup, mSoundMap);
}
	
/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of streams in a group
// \details 	<b>Description:</b> Get the number of streams in a group
//************************************/
int	ResourceManager::GetNumMusic(const SexyString& theGroup)
{
	return GetNumResources(theGroup, mMusicMap);
}

/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of fonts in a group
// \details 	<b>Description:</b> Get the number of fonts in a group
//************************************/
int ResourceManager::GetNumFonts(const SexyString& theGroup)
{
	return GetNumResources(theGroup, mFontMap);
}

/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of movies in a group
// \details 	<b>Description:</b> Get the number of movies in a group
//************************************/
int ResourceManager::GetNumMovies(const SexyString& theGroup)
{
	return GetNumResources(theGroup, mMovieMap);
}

/*!***********************************
// @return    	int
// @param     	theGroup
//
// \brief		Get the number of all resources of a group
// \details 	<b>Description:</b> Get the number of all resources of a group
//************************************/
int	ResourceManager::GetNumResources(const SexyString& theGroup)
{
	return GetNumImages(theGroup) + GetNumSounds(theGroup) + GetNumMusic(theGroup) + GetNumFonts(theGroup) + GetNumMovies(theGroup);
}

/*!***********************************
// @return    	Sexy::SharedImageRef
// @param     	theId
//
// \brief		Get the image with specified ID
// \details 	<b>Description:</b> Get the image with specified ID
//************************************/
SharedImageRef ResourceManager::GetImage(const SexyString& theId)
{
	ResMap::iterator anItr = mImageMap.find(theId);

	if (anItr != mImageMap.end())
		return ((ImageRes*)anItr->second)->mImage;
	else
		return NULL;
}
	
/*!***********************************
// @return    	int
// @param     	theId
//
// \brief		Get the sound with specified ID
// \details 	<b>Description:</b> Get the sound with specified ID
//************************************/
int	ResourceManager::GetSound(const SexyString& theId)
{
	ResMap::iterator anItr = mSoundMap.find(theId);
	if (anItr != mSoundMap.end())
		return ((SoundRes*)anItr->second)->mSampleID;
	else
		return -1;
}

/*!***********************************
// @return    	int
// @param     	theId
//
// \brief		Get the music with specified ID
// \details 	<b>Description:</b> Get the music with specified ID
//************************************/
int	ResourceManager::GetMusic(const SexyString& theId)
{
	ResMap::iterator anItr = mMusicMap.find(theId);
	if (anItr != mMusicMap.end())
		return ((MusicRes*)anItr->second)->mStreamID;
	else
		return -1;
}

/*!***********************************
// @return    	SharedFontRef
// @param     	theId
//
// \brief		Get the font with specified ID
// \details 	<b>Description:</b> Get the font with specified ID
//************************************/
SharedFontRef ResourceManager::GetFont(const SexyString& theId)
{
	ResMap::iterator anItr = mFontMap.find(theId);

	if (anItr != mFontMap.end())
		return ((FontRes*)anItr->second)->mFont;
	else
		return NULL;
}

/*!***********************************
// @return    	SexyMovie*
// @param     	theId
//
// \brief		Get the movie with specified ID
// \details 	<b>Description:</b> Get the movie with specified ID
 //************************************/
#ifndef K_IPHONE
SexyMovie* ResourceManager::GetMovie(const SexyString& theId)
{
	ResMap::iterator anItr = mMovieMap.find(theId);
	
	if (anItr != mMovieMap.end())
		return ((MovieRes*)anItr->second)->mMovie;
	else
		return NULL;
}
#endif

/*!***********************************
// @return    	Sexy::SharedImageRef
// @param     	theId
//
// \brief		Get the image with specified ID
// \details 	<b>Description:</b> Get the image with specified ID
//				throw a ResourceManagerException if the resource is not found
//************************************/
SharedImageRef ResourceManager::GetImageThrow(const SexyString& theId)
{
	ResMap::iterator anItr = mImageMap.find(theId);

	if (anItr != mImageMap.end())
	{
		ImageRes *aRes = (ImageRes*)anItr->second;

		if ((SexyImage*)aRes->mImage != NULL)
			return aRes->mImage;

		if (mAllowMissingProgramResources && aRes->mFromProgram)
			return NULL;
	}

	Fail(_S("Sexy::ResourceManager::GetImageThrow : ERROR : Image resource not found: ") + theId);
	
	throw ResourceManagerException(GetErrorText());
}

/*!***********************************
// @return    	int
// @param     	theId
//
// \brief		Get the sound with specified ID
// \details 	<b>Description:</b> Get the sound with specified ID
//				throw a ResourceManagerException if the resource is not found
//************************************/
int	ResourceManager::GetSoundThrow(const SexyString& theId)
{
	ResMap::iterator anItr = mSoundMap.find(theId);

	if (anItr != mSoundMap.end())
	{
		SoundRes *aRes = (SoundRes*)anItr->second;

		if (aRes->mSampleID != -1)
			return aRes->mSampleID;

		if (mAllowMissingProgramResources && aRes->mFromProgram)
			return -1;
	}

	Fail(_S("Sexy::ResourceManager::GetSoundThrow : ERROR : Sound resource not found: ") + theId);
	
	throw ResourceManagerException(GetErrorText());		
}

/*!***********************************
// @return    	int
// @param     	theId
//
// \brief		Get the music with specified ID
// \details 	<b>Description:</b> Get the music with specified ID
//				throw a ResourceManagerException if the resource is not found
//************************************/
int	ResourceManager::GetMusicThrow(const SexyString& theId)
{
	ResMap::iterator anItr = mMusicMap.find(theId);

	if (anItr != mMusicMap.end())
	{
		MusicRes *aRes = (MusicRes*)anItr->second;

		if (aRes->mStreamID != -1)
			return aRes->mStreamID;

		if (mAllowMissingProgramResources && aRes->mFromProgram)
			return -1;
	}

	Fail(_S("Sexy::ResourceManager::GetMusicThrow : ERROR : Music resource not found: ") + theId);
	
	throw ResourceManagerException(GetErrorText());		
}

/*!***********************************
// @return    	SharedFontRef
// @param     	theId
//
// \brief		Get the font with specified ID
// \details 	<b>Description:</b> Get the font with specified ID
//				throw a ResourceManagerException if the resource is not found
//************************************/
SharedFontRef ResourceManager::GetFontThrow(const SexyString& theId)
{
	ResMap::iterator anItr = mFontMap.find(theId);

	if (anItr != mFontMap.end())
	{
		FontRes *aRes = (FontRes*)anItr->second;

		if (aRes->mFont != NULL)
			return aRes->mFont;

		if (mAllowMissingProgramResources && aRes->mFromProgram)
			return NULL;
	}

	Fail(_S("Sexy::ResourceManager::GetFontThrow : ERROR : Font resource not found: ") + theId);
	
	throw ResourceManagerException(GetErrorText());
}

/*!***********************************
// @return    	SexyMovie*
// @param     	theId
//
// \brief		Get the movie with specified ID
// \details 	<b>Description:</b> Get the movie with specified ID
//				throw a ResourceManagerException if the resource is not found
 //************************************/
#ifndef K_IPHONE
SexyMovie* ResourceManager::GetMovieThrow(const SexyString& theId)
{
	ResMap::iterator anItr = mMovieMap.find(theId);

	if (anItr != mMovieMap.end())
	{
		MovieRes *aRes = (MovieRes*)anItr->second;

		if (aRes->mMovie != NULL)
			return aRes->mMovie;

		if (mAllowMissingProgramResources && aRes->mFromProgram)
			return NULL;
	}

	Fail(_S("Sexy::ResourceManager::GetMovieThrow : ERROR : Movie resource not found: ") + theId);
	
	throw ResourceManagerException(GetErrorText());
}
#endif

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theImage
//
// \brief		Replace the image of the specified ID
// \details 	<b>Description:</b> Replace the image of the specified ID
//************************************/
bool ResourceManager::ReplaceImage(const SexyString& theId, SexyImage* theImage)
{
	ResMap::iterator anItr = mImageMap.find(theId);
	
	if (anItr != mImageMap.end())
	{
		anItr->second->DeleteResource();
		((ImageRes*)anItr->second)->mImage = (SexyImage*)theImage;
		((ImageRes*)anItr->second)->mImage.mOwnsUnshared = true;
		return true;
	}
	else
		return false;
}

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theSound
//
// \brief		Replace the sound of the specified ID
// \details 	<b>Description:</b> Replace the sound of the specified ID
//************************************/
bool ResourceManager::ReplaceSound(const SexyString& theId, int theSound)
{
	ResMap::iterator anItr = mSoundMap.find(theId);

	if (anItr != mSoundMap.end())
	{
		anItr->second->DeleteResource();
		((SoundRes*)anItr->second)->mSampleID = theSound;
		return true;
	}
	else
		return false;
}

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theStream
//
// \brief		Replace the music of the specified ID
// \details 	<b>Description:</b> Replace the music of the specified ID
//************************************/
bool ResourceManager::ReplaceMusic(const SexyString& theId, int theStream)
{
	ResMap::iterator anItr = mMusicMap.find(theId);

	if (anItr != mMusicMap.end())
	{
		anItr->second->DeleteResource();
		((MusicRes*)anItr->second)->mStreamID = theStream;
		return true;
	}
	else
		return false;
}

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theFont
//
// \brief		Replace the font of the specified ID
// \details 	<b>Description:</b> Replace the font of the specified ID
//************************************/
bool ResourceManager::ReplaceFont(const SexyString& theId, SexyFont* theFont)
{
	ResMap::iterator anItr = mFontMap.find(theId);

	if (anItr != mFontMap.end())
	{
		anItr->second->DeleteResource();
		((FontRes*)anItr->second)->mFont = theFont;
		return true;
	}
	else
		return false;
}

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theMovie
//
// \brief		Replace the movie of the specified ID
// \details 	<b>Description:</b> Replace the movie of the specified ID
 //************************************/
#ifndef K_IPHONE
bool ResourceManager::ReplaceMovie(const SexyString& theId, SexyMovie* theMovie)
{
	ResMap::iterator anItr = mMovieMap.find(theId);

	if (anItr != mMovieMap.end())
	{
		anItr->second->DeleteResource();
		((MovieRes*)anItr->second)->mMovie = theMovie;
		return true;
	}
	else
		return false;
}
#endif

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
// @param     	theResMap
//
// \brief		Helper to get the XML Attributes
// \details 	<b>Description:</b> Helper to get the XML Attributes
//				from a resource with specified ID of the specified resource map
//************************************/
const XMLParamMap& ResourceManager::GetXMLAttributes(const SexyString& theId, ResMap* theResMap)
{
	static XMLParamMap aStrMap;

	ResMap::iterator anItr = theResMap->find(theId);

	if (anItr != theResMap->end())
		return anItr->second->mXMLAttributes;
	else
		return aStrMap;
}

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
//
// \brief		Get the XML attributes of the image with the specified id
// \details 	<b>Description:</b> Get the XML attributes of the image with the specified id
//************************************/
const XMLParamMap& ResourceManager::GetImageAttributes(const SexyString& theId)
{
	return GetXMLAttributes(theId, &mImageMap);
}

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
//
// \brief		Get the XML attributes of the sample with the specified id
// \details 	<b>Description:</b> Get the XML attributes of the sample with the specified id
//************************************/
const XMLParamMap& ResourceManager::GetSoundAttributes(const SexyString& theId)
{
	return GetXMLAttributes(theId, &mSoundMap);
}

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
//
// \brief		Get the XML attributes of the stream with the specified id
// \details 	<b>Description:</b> Get the XML attributes of the stream with the specified id
//************************************/
const XMLParamMap& ResourceManager::GetMusicAttributes(const SexyString& theId)
{
	return GetXMLAttributes(theId, &mMusicMap);
}

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
//
// \brief		Get the XML attributes of the font with the specified id
// \details 	<b>Description:</b> Get the XML attributes of the font with the specified id
//************************************/
const XMLParamMap& ResourceManager::GetFontAttributes(const SexyString& theId)
{
	return GetXMLAttributes(theId, &mFontMap);
}

/*!***********************************
// @return    	const XMLParamMap&
// @param     	theId
//
// \brief		Get the XML attributes of the movie with the specified id
// \details 	<b>Description:</b> Get the XML attributes of the movie with the specified id
//************************************/
const XMLParamMap& ResourceManager::GetMovieAttributes(const SexyString& theId)
{
	return GetXMLAttributes(theId, &mMovieMap);
}

/*!***********************************
// @return    	void
// @param     	theDestStr
//
// \brief		Dump the current resource group
// \details 	<b>Description:</b> Dump the current resource group
//************************************/
void ResourceManager::DumpCurResGroup(SexyString& theDestStr)
{
	const ResList* rl = &mResGroupMap.find(mCurResGroup)->second;
	ResList::const_iterator it = rl->begin();
	theDestStr = StrFormat(_S("About to dump %d elements from current res group: "), rl->size());
	theDestStr += mCurResGroup + _S("\r\n");

	ResList::const_iterator rl_end = rl->end();
	while (it != rl_end)
	{
		BaseRes* br = *it++;
		SexyString prefix = br->mId + _S(": ") + br->mPath;
		theDestStr += prefix;
		if (br->mFromProgram)
			theDestStr += _S(" res is from program\r\n");
		else if (br->mType == ResType_Image)
			theDestStr += _S(" res is an image\r\n");
		else if (br->mType == ResType_Sound)
			theDestStr += _S(" res is a sound\r\n");
		else if (br->mType == ResType_Music)
			theDestStr += _S(" res is a music\r\n");
		else if (br->mType == ResType_Font)
			theDestStr += _S(" res is a font\r\n");
		else if (br->mType == ResType_Movie)
			theDestStr += _S(" res is a movie\r\n");

		if (it == mCurResGroupListItr)
			theDestStr += _S("iterator has reached mCurResGroupItr\r\n");
	}

	theDestStr += _S("Done dumping resources\r\n");
}
