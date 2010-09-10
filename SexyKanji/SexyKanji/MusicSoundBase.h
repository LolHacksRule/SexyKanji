// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji MusicSoundBase Version 0.4
	-------------------------------------------------------------	
	Created:	2010/02/25
	Filename: 	SexyKanji\MusicSoundBase.h
	
	Purpose:	SexyAppFramework Wrapper interface for music and sound
	
				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#ifndef __MUSICSOUNDBASE_H__
#define __MUSICSOUNDBASE_H__

#include "SexyAppBase.h"

#include <string>
#include <map>

// this is the only place in the entire solution, where we have to include KSound.h
#include "KSound.h"

#define MIN_VOLUME	0
#define MAX_VOLUME	100

namespace Sexy
{
	// define the interface as base class for music and sound manager
	class MusicSoundBase
	{
	public:
		MusicSoundBase();
		~MusicSoundBase();

	    //----------------------------------------------------------------------------
	    // set the global music volume [MIN_VOLUME..MAX_VOLUME]
		//----------------------------------------------------------------------------
		void		SetGlobalMusicVolume(int theGlobalMusicVolume);
		
	    //----------------------------------------------------------------------------
	    // set the global sound volume [MIN_VOLUME..MAX_VOLUME]
		//----------------------------------------------------------------------------
		void		SetGlobalSoundVolume(int theGlobalSoundVolume);
		
	    //----------------------------------------------------------------------------
	    // return the global music volume [MIN_VOLUME..MAX_VOLUME]
		//----------------------------------------------------------------------------
		inline int	GetGlobalMusicVolume() { return mGlobalMusicVolume; }
		
	    //----------------------------------------------------------------------------
	    // return the global sound volume [MIN_VOLUME..MAX_VOLUME]
		//----------------------------------------------------------------------------
		inline int	GetGlobalSoundVolume() { return mGlobalSoundVolume; }

	    //----------------------------------------------------------------------------
		// suspend all sound activity, for instance after losing focus
		//----------------------------------------------------------------------------
		void		SuspendAll();
		
	    //----------------------------------------------------------------------------
	    // return true if all sound and music is suspended, otherwise false
		//----------------------------------------------------------------------------
		inline bool	IsSuspended() { return mSuspended; }

	    //----------------------------------------------------------------------------
		// resume sound activity, for instance after regaining focus 
		//----------------------------------------------------------------------------
		void		ResumeAll();

	    //----------------------------------------------------------------------------
		// detect sound effect format from the supplied filename's extension (.wav, .ogg)
		// param theFileName to detect image format from
		// return detected format (SAMPLE_xxx), or SAMPLE_UNKNOWN if unsupported
		//----------------------------------------------------------------------------
		long		DetectFormat(const SexyString& theFileName);

	    //----------------------------------------------------------------------------
		// free all lingering resources when not using audio anymore. This method may 
		// be called by the game just before exiting, to check for memory leaks, 
		// otherwise it is not necessary.
		//----------------------------------------------------------------------------
		void		CleanupAll();

	protected:
		// pure virtual, this abstract class is a interface
		// derived classes have to implement Update()
		virtual void Update() = 0;

		// the structure with all the information of a sample/stream
		struct SampleStreamInfo
		{
			KSound*				mSoundInstance;	// the sound instance for this sample/stream
			SexyString			mFileName;		// the filename of the sample/stream
			int					mInitialVolume;	// default 100, standard parameter from LoadSample()/LoadStream()
			float				mFadeVolume;	// volume is changing during fade
			int					mCurrentVolume;	// the currently set volume
		};

	private:

		KSound*					mSoundInstance;			// to access the methods shared by sound and music manager
		static int				mGlobalMusicVolume;		// global volume for all music streams
		static int				mGlobalSoundVolume;		// global volume for all sound samples
		static bool				mSuspended;				// suspend state
	};
}

#endif // __MUSICSOUNDBASE_H__
