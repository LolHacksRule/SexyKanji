// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji ResourceManager Version 0.4
	-------------------------------------------------------------	
	Created:	2010/03/24
	Filename: 	SexyKanji\ResourceManager.h
	
	Purpose:	Parse and load resources from XML file with modifiers.

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "KSysDefs.h"
#include "Common.h"
#include "SexyImage.h"
#include "SexyColor.h"
#include "SharedImage.h"
#include "SharedFont.h"
#include "XMLParser.h"
#include "SexyConfig.h"

#include <string>
#include <map>

// undef WINAPI LoadImage
#ifdef LoadImage
#undef LoadImage
#endif

namespace Sexy
{
	//////////////////////////////////////////////////////////////////////////
	// forward declarations
	//////////////////////////////////////////////////////////////////////////
	class XMLParser;
	class XMLElement;
	class SexyImage;
	class SexyFont;
#ifndef K_IPHONE
	class SexyMovie;
#endif

	class ResourceManager
	{
	protected:

		//////////////////////////////////////////////////////////////////////////
		// static members to check the modifiers in the XML file
		//////////////////////////////////////////////////////////////////////////

		static const SexyStringSet		MODIFIERS_SETDEFAULTS;
		static const SexyStringSet		MODIFIERS_IMAGE;
		static const SexyStringSet		MODIFIERS_FONT;
		static const SexyStringSet		MODIFIERS_SOUND;
		static const SexyStringSet		MODIFIERS_MUSIC;
		static const SexyStringSet		MODIFIERS_MOVIE;

		//////////////////////////////////////////////////////////////////////////
		// internal data structures for the different resources
		//////////////////////////////////////////////////////////////////////////

		// the supported resource types 
		enum ResType
		{
			ResType_Image,
			ResType_Sound,
			ResType_Music,
			ResType_Font,
			ResType_Movie
		};

		// basic things for every resource type
		struct BaseRes
		{
			ResType				mType;				// resource type, see enum ResType
			SexyString			mId;				// unique resource id (id prefix plus id, e.g. IMAGE_PARROT)
			SexyString			mResGroup;			// the resource group name
			SexyString			mPath;				// path (+ name) to resource
			XMLParamMap			mXMLAttributes;		// all specified attributes and modifier
			bool				mFromProgram;		// program supplied resource?

			virtual				~BaseRes() {}
			virtual void		DeleteResource() { }
		};

		struct ImageRes : public BaseRes
		{
			SharedImageRef		mImage;				// the image
			SexyString			mAlphaFile;			// filename for alpha mask
			SexyString			mAlphaGridFile;		// filename for alpha grid mask
			SexyString			mVariant;			// variant for shared image
			bool				mLookForAlphaFile;	// look for alpha file?
			bool				mNoPurge;			// don´t purge the bits?
			bool				mNoBlend;			// Force linear blending to false
			long				mWrapMode;			// wrapping mode, clamp, repeat, mirror
			int					mRows;				// number of rows
			int					mCols;				// number of cols
			int					mNumCells;			// number of cells
			AnimInfo			mAnimInfo;			// animation info
			SexyColor			mColorizeColor;		// colorize
			int					mRotation;			// rotation angle (90, 180, 270)
			bool				mMirrorX;			// mirror X?
			bool				mMirrorY;			// mirror Y?
			int					mDeltaHue;			// delta hue
			SexyString			mScaleX;			// to extract scale X
			SexyString			mScaleY;			// to extract scale Y

			ImageRes()			{ mType = ResType_Image; mWrapMode = 0/*K_WRAP_CLAMP*/; mNoBlend = false; }
			virtual void		DeleteResource();
		};

		struct SoundRes : public BaseRes
		{
			int					mSampleID;			// a unique sample ID
			int					mVolume;			// the volume for this sample

			SoundRes()			{ mType = ResType_Sound; }
			virtual void		DeleteResource();
		};

		struct MusicRes : public BaseRes
		{
			int					mStreamID;			// a unique stream ID
			int					mVolume;			// the volume for this stream
			bool				mForceLoad;			// force loading stream

			MusicRes()			{ mType = ResType_Music; }
			virtual void		DeleteResource();
		};

		struct FontRes : public BaseRes
		{
			SharedFontRef		mFont;				// the font
			SexyString			mVariant;			// variant for shared font
			bool				mIsFreeType;		// is free type font?
			int					mSize;				// font size (free type font only)

			FontRes()			{ mType = ResType_Font; }
			virtual void		DeleteResource();
		};
		
#ifndef K_IPHONE
		struct MovieRes : public BaseRes
		{
			SexyMovie*			mMovie;				// the movie
			bool				mPreload;			// preload movie?
			
			MovieRes()			{ mType = ResType_Movie; }
			virtual void		DeleteResource();
		};
#endif

		// alias names
		typedef std::map<SexyString, BaseRes*>					ResMap;
		typedef std::list<BaseRes*>								ResList;
		typedef std::map<SexyString, ResList, StringLessNoCase> ResGroupMap; 

		// store the group names (for IsGroupLoaded())
		std::set<SexyString, StringLessNoCase> mLoadedGroups;	

		// store the resource information in corresponding maps
		ResMap					mImageMap;			
		ResMap					mSoundMap;
		ResMap					mMusicMap;
		ResMap					mFontMap;
		ResMap					mMovieMap;

		ResGroupMap				mResGroupMap;		// all the resources 
		SexyString				mCurResGroup;		// current resource group
		ResList*				mCurResGroupList;	// remember the current resources of the group
		ResList::iterator		mCurResGroupListItr;// remember position for LoadNextResource()

		XMLParser*				mXMLParser;			// for parsing resource.xml
		SexyString				mErrorString;		// error explanation
		bool					mHasFailed;			// error occurred?
		SexyString				mDefaultPath;		// path from SetDefaults
		SexyString				mDefaultIdPrefix;	// idprefix from SetDefaults
		bool					mAllowMissingProgramResources;	// set by SetAllowMissingProgramImages()
		bool					mAllowAlreadyDefinedResources;	// for re-parsing file while running
		bool					mHadAlreadyDefinedError;		// set to true if a resource is already defined

		//////////////////////////////////////////////////////////////////////////
		// Internal helpers for parsing and loading resources
		//////////////////////////////////////////////////////////////////////////

		bool					Fail(const SexyString& theErrorText);
		virtual void			ReadIntVector(const SexyString& theVal, std::vector<int>& theVector);

		virtual bool			ParseCommonResource(XMLElement& theElement, BaseRes* theRes, ResMap& theMap);
		virtual bool			ParseImageResource(XMLElement& theElement);
		virtual bool			ParseSoundResource(XMLElement& theElement);
		virtual bool			ParseMusicResource(XMLElement& theElement);
		virtual bool			ParseFontResource(XMLElement& theElement);
#ifndef K_IPHONE
		virtual bool			ParseMovieResource(XMLElement& theElement);
#endif
		virtual bool			ParseSetDefaults(XMLElement& theElement);
		virtual bool			CheckResourceAttributes(XMLElement& theElement);

		virtual bool			ParseResources();
		bool					DoParseResources();

		bool					LoadAlphaGridImage(ImageRes* theRes, SexyImage* theImage);
		bool					LoadAlphaImage(ImageRes* theRes, SexyImage* theImage);
		virtual bool			DoLoadImage(ImageRes* theRes);
		virtual bool			DoLoadSound(SoundRes* theRes);
		virtual bool			DoLoadMusic(MusicRes* theRes);
		virtual bool			DoLoadFont(FontRes* theRes);
#ifndef K_IPHONE
		virtual bool			DoLoadMovie(MovieRes* theRes);
#endif

		int						GetNumResources(const SexyString& theGroup, ResMap& theMap);

		const XMLParamMap&		GetXMLAttributes(const SexyString& theId, ResMap* theResMap);

		void					DeleteMap(ResMap& theMap);
		virtual void			DeleteResources(ResMap& theMap, const SexyString& theGroup);

	public:
		ResourceManager();
		virtual ~ResourceManager();

		//////////////////////////////////////////////////////////////////////////
		// Access methods
		//////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------
		// Parsing the resource file.
		//----------------------------------------------------------------------------
		bool					LoadResourceManifest() { return ParseResourcesFile(DEFAULT_RESOURCE_PATH); }

		bool					ParseResourcesFile(const SexyString& theFilename);
		bool					ReparseResourcesFile(const SexyString& theFilename);

		//----------------------------------------------------------------------------
		// Load resources and apply modifiers.
		//----------------------------------------------------------------------------
		virtual bool			LoadResources(const SexyString& theGroup);

		virtual void			StartLoadResources(const SexyString& theGroup);
		virtual bool			LoadNextResource();

		virtual void			ResourceLoadedHook(BaseRes* theRes);

		//----------------------------------------------------------------------------
		// Error handling.
		//----------------------------------------------------------------------------
		void					ShowResourceError(bool theExit);
		inline SexyString		GetErrorText()	{ return mErrorString; }
		inline bool				HadError()		{ return mHasFailed; }

		//----------------------------------------------------------------------------
		// Replace Methods.
		//----------------------------------------------------------------------------
		bool					ReplaceImage(const SexyString& theId, SexyImage* theImage);
		bool					ReplaceSound(const SexyString& theId, int theSound);
		bool					ReplaceMusic(const SexyString& theId, int theStream);
		bool					ReplaceFont(const SexyString& theId, SexyFont* theFont);
#ifndef K_IPHONE
		bool					ReplaceMovie(const SexyString& theId, SexyMovie* theMovie);
#endif

		//----------------------------------------------------------------------------
		// Image, font, specials.
		//----------------------------------------------------------------------------
		void					DeleteImage(const SexyString& theName);
		SharedImageRef			LoadImage(const SexyString& theName);

		void					DeleteFont(const SexyString& theName);
		SharedFontRef			LoadFont(const SexyString& theName);

		void					SetAllowMissingProgramImages(bool theAllow) { mAllowMissingProgramResources = theAllow; }
		bool					IsGroupLoaded(const SexyString& theGroup) { return mLoadedGroups.find(theGroup) != mLoadedGroups.end(); }

		virtual void			DeleteResources(const SexyString& theGroup);

		//----------------------------------------------------------------------------
		// Getter methods.
		//----------------------------------------------------------------------------
		int						GetNumImages(const SexyString& theGroup);
		int						GetNumSounds(const SexyString& theGroup);
		int						GetNumMusic(const SexyString& theGroup);
		int						GetNumFonts(const SexyString& theGroup);
		int						GetNumMovies(const SexyString& theGroup);

		int						GetNumResources(const SexyString& theGroup);

		SharedImageRef			GetImage(const SexyString& theId);
		int						GetSound(const SexyString& theId);
		int						GetMusic(const SexyString& theId);
		SharedFontRef			GetFont(const SexyString& theId);
#ifndef K_IPHONE
		SexyMovie*				GetMovie(const SexyString& theId);
#endif

		// Returns all the XML attributes associated with the resource
		const XMLParamMap&		GetImageAttributes(const SexyString& theId);
		const XMLParamMap&		GetSoundAttributes(const SexyString& theId);
		const XMLParamMap&		GetMusicAttributes(const SexyString& theId);
		const XMLParamMap&		GetFontAttributes(const SexyString& theId);
		const XMLParamMap&		GetMovieAttributes(const SexyString& theId);

		// These methods throw a ResourceManagerException if the resource is not found
		virtual SharedImageRef	GetImageThrow(const SexyString& theId);
		virtual int				GetSoundThrow(const SexyString& theId);
		virtual int				GetMusicThrow(const SexyString& theId);
		virtual SharedFontRef	GetFontThrow(const SexyString& theId);
#ifndef K_IPHONE
		virtual SexyMovie*		GetMovieThrow(const SexyString& theId);
#endif

		// Get current resource group information
		const ResList*			GetCurResGroupList()	{ return mCurResGroupList; }
		SexyString				GetCurResGroup()		{ return mCurResGroup; }

		void					DumpCurResGroup(SexyString& theDestStr);
	};

	// Exception handling
	struct ResourceManagerException : public std::exception
	{
		SexyString aWhat;
		ResourceManagerException(const SexyString& theWhat) throw() { aWhat = theWhat; }
		virtual ~ResourceManagerException() throw() {}
	};
}

#endif // __RESOURCEMANAGER_H__
