// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji SoundManager Version 0.4
	-------------------------------------------------------------	
	Created:	2010/02/24
	Filename: 	SexyKanji\SoundManager.h
	
	Purpose:	SexyAppFramework Wrapper class for sound samples
				play samples with enhanced capabilities:
				play delayed, play chained, looping, fading,
				define minimum time between samples
				
				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "MusicSoundBase.h"

#include <string>
#include <vector>
#include <utility>
#include <map>

#include "SexyConfig.h"
#include "AutoCrit.h"

namespace Sexy
{
	class SoundManager : public MusicSoundBase
	{
	public:
		SoundManager();
		~SoundManager();

	    //----------------------------------------------------------------------------
	    // load sample with a specified ID, any previously loaded sample with this ID 
		// will be released! returns true if the sample could be loaded, false if not
		// optionally specify a initial volume [0..100]
		//----------------------------------------------------------------------------
		virtual bool	LoadSample(unsigned int theSampleID, const SexyString& theFileName, int theVolume = 100);
		
	    //----------------------------------------------------------------------------
		// load sample, the last free SampleID will be used
		// returns new SampleID or -1 for failure
	    //----------------------------------------------------------------------------
		virtual int		LoadSample(const SexyString& theFileName, int theVolume = 100); 

	    //----------------------------------------------------------------------------
		// releases a specific sample or all samples - all loaded samples are 
		// auto-released in the destructor		
	    //----------------------------------------------------------------------------
		virtual void	ReleaseSamples();
		virtual void	ReleaseSample(unsigned int theSampleID);

	    //----------------------------------------------------------------------------
		// plays the given sample immediately
	    //----------------------------------------------------------------------------
		virtual void	PlaySample(unsigned int theSampleID);

	    //----------------------------------------------------------------------------
		// plays the given sample: specify a minimum amount of frames
		// between duplicates to avoid flanging effects (or 0 if you don't care).
	    //----------------------------------------------------------------------------
		virtual void	PlaySample(unsigned int theSampleID, unsigned int theMinFrames);

	    //----------------------------------------------------------------------------
		// plays the given sample after a specified number of frames
	    //----------------------------------------------------------------------------
		virtual void	PlaySampleDelayed(unsigned int theSampleID, unsigned int theFramesDelay);
	
	    //----------------------------------------------------------------------------
		// set or increment the volume of the given sample: 0 = silent, 100 = max volume
	    //----------------------------------------------------------------------------
		virtual void	SetSampleVolume(unsigned int theSampleID, int theVolume);
		virtual void	IncSampleVolume(unsigned int theSampleID, int theVolumeInc);

	    //----------------------------------------------------------------------------
		// get the initial volume set in LoadSample() or the current volume
	    //----------------------------------------------------------------------------
		virtual int		GetInitialSampleVolume(unsigned int theSampleID);
		virtual int		GetCurrentSampleVolume(unsigned int theSampleID);

	    //----------------------------------------------------------------------------
		// plays a sample: when it is done will play the next sample
		// you can specify a (frame) time delay to elapse after the first sample 
		// completes before triggering the next sample
	    //----------------------------------------------------------------------------
		virtual void	PlaySamplesChained(unsigned int theFirstSampleID, unsigned int theNextSampleID, unsigned int theFramesDelay);

	    //----------------------------------------------------------------------------
		// stops playing the given sample 
	    //----------------------------------------------------------------------------
		virtual void	StopSample(unsigned int theSampleID);
		virtual void	StopAllSamples();

	    //----------------------------------------------------------------------------
		// returns an empty sample ID
	    //----------------------------------------------------------------------------
		virtual int		GetFreeSampleId();

	    //----------------------------------------------------------------------------
		// pause or resume playing the sample with the given ID
		// thePause: true to pause, false to resume
	    //----------------------------------------------------------------------------
		virtual void	PauseSample(unsigned int theSampleID, bool thePause);

	    //----------------------------------------------------------------------------
		// check if this sample is currently playing, returns true if playing, false if not
	    //----------------------------------------------------------------------------
		virtual bool	IsSamplePlaying(unsigned int theSampleID);

	    //----------------------------------------------------------------------------
		// loops the given sample with no delay for a continuous sound, optionally
		// specify a fade in speed (based on frames), 0.0f = no fade in
	    //----------------------------------------------------------------------------
		virtual void	LoopSample(unsigned int theSampleID, float theFadeInSpeed = 0.0f);

	    //----------------------------------------------------------------------------
		// stops a looping sample
	    //----------------------------------------------------------------------------
		virtual void	StopLoopingSample(unsigned int theSampleID);

	    //----------------------------------------------------------------------------
		// mutes/unmutes looping samples ONLY
		// theMute: true = mute looping samples, false = unmute looping samples
	    //----------------------------------------------------------------------------
		virtual void	MuteLoopingSamples(bool theMute);

	    //----------------------------------------------------------------------------
		// fades out a looping sample. When the volume reaches 0, 
		// the sample will be removed from the list of looping samples
	    //----------------------------------------------------------------------------
		virtual void	FadeOutLoopingSample(unsigned int theSampleID, float theFadeOutSpeed);

	private:
		// SexyAppBase is a friend, so the Update() method can kept private here
		friend class SexyAppBase;

		// called from SexyAppBase::DoUpdateFrames()
		virtual void Update();

		// alias names
		typedef std::map<unsigned int, unsigned int> UIntUIntMap;									
		typedef std::vector<std::pair<unsigned int, unsigned int> > UIntUIntPairVector;			
		typedef std::map<unsigned int, std::pair<KSound*, float> > LoopingSampleMap;  

		// the structure to play samples chained
		struct ChainedSamples
		{
			unsigned int			mFirstSampleID;	// the SampleID of the first playing sample
			unsigned int			mNextSampleID;	// the SampleID to play after completion of the first sample
			unsigned int			mFramesDelay;	// how long to wait after playing the first sample before playing the second after the first is done
			KSound*					mFirstSoundInstance;	// SoundInstance used to poll when the first sample is done playing

			// c tors
			ChainedSamples() { mFirstSampleID = mNextSampleID = mFramesDelay = MAX_SOUND_SAMPLES; mFirstSoundInstance = NULL; }
			ChainedSamples(int theFirstSampleID, int theNextSampleID, int theFramesDelay, KSound* theSoundInstance) 
				{ mFirstSampleID		= theFirstSampleID; 
				  mNextSampleID			= theNextSampleID; 
				  mFramesDelay			= theFramesDelay; 
				  mFirstSoundInstance	= theSoundInstance; }
		};

		SampleStreamInfo			mSamplesInfo[MAX_SOUND_SAMPLES];	// informations about all the loaded samples
		UIntUIntMap					mSampleTimes;	// keeps track of when a given SampleID last played -> <SampleID, mUpdateCnt>
		UIntUIntPairVector			mDelayedSFX;	// keeps a list of delay times before playing a sample -> <SampleID, FramesDelay>
		std::vector<ChainedSamples>	mChainedSFX;	// a list of samples who will play another sound effect when complete
		LoopingSampleMap			mLoopingSamples;// a list of samples that loop over and over -> <SampleID, <KSound*, FadeSpeed>>
		unsigned int				mUpdateCnt;		// incremented every Update() call to measure frames
		int							mMuteCount;		// counts the callings to MuteLoopingSamples()

		KSysLock					mSoundManagerCritSect;
	};
}

#endif // __SOUND_MANAGER_H__
