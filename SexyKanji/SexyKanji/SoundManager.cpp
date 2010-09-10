// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji SoundManager Version 0.4
	-------------------------------------------------------------	
	Created:	2010/02/24
	Filename: 	SexyKanji\SoundManager.cpp
	
	Purpose:	SexyAppFramework Wrapper class for sound samples
				play samples with enhanced capabilities:
				play delayed, play chained, looping, fading,
				define minimum time between samples

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#include "SoundManager.h"

#include "KMiscTools.h"

#include "SexyAppBase.h"

// This needs to be the last include directive.  All other
// #includes should be above this point.
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		Constructor, member initialization
// \details 	<b>Description:</b> Constructor, member initialization
//************************************/
SoundManager::SoundManager()
{
	// init member
	mUpdateCnt = 0;
	mMuteCount = 0;

	// init sample information
	for (unsigned int i = 0; i < MAX_SOUND_SAMPLES; i++)
	{
		mSamplesInfo[i].mSoundInstance	= NULL;
		mSamplesInfo[i].mFileName		= _S(""); 
		mSamplesInfo[i].mInitialVolume	= MIN_VOLUME;
		mSamplesInfo[i].mFadeVolume		= MIN_VOLUME;
		mSamplesInfo[i].mCurrentVolume	= MIN_VOLUME;
	}
}

/*!***********************************
// @return    	
//
// \brief		Destructor, releases all samples
// \details 	<b>Description:</b> Destructor, releases all samples
//************************************/
SoundManager::~SoundManager()
{
	ReleaseSamples();
}

/*!***********************************
// @return    	bool true if the sample could be loaded, false if not
// @param     	theSampleID unique sample identification
// @param     	theFileName filename of the sample
// @param     	theVolume optional initial volume [0..100]
//
// \brief		load sample with a specified ID
// \details 	<b>Description:</b> load sample with a specified ID, any previously 
//				loaded sample with this ID will be released! 
//************************************/
bool SoundManager::LoadSample(unsigned int theSampleID, const SexyString& theFileName, int theVolume)
{
	// Must pass in a valid ID. (JSullivan)
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::LoadSample: WARNING : Invalid sample ID.");
		return false;
	}

	// clamp the volume
	if (theVolume < MIN_VOLUME) theVolume = MIN_VOLUME;
	if (theVolume > MAX_VOLUME) theVolume = MAX_VOLUME;

	// Make sure any sample already using this Id is released first. (JSullivan)
	ReleaseSample(theSampleID);

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		// get file path
		std::string aFilePath = (SexyStringToStringFast(theFileName).c_str());
		std::string anOggFilePath = (SexyStringToStringFast(theFileName + _S(".ogg")).c_str());
		std::string aWavFilePath = (SexyStringToStringFast(theFileName + _S(".wav")).c_str());

		// get a new sound instance and load the sample
		mSamplesInfo[theSampleID].mSoundInstance = new KSound();
		KSound::resume();
		bool aSuccess = mSamplesInfo[theSampleID].mSoundInstance->loadSample(aFilePath.c_str(), theVolume);
		if(!aSuccess) // attemp OGG extension
		{
			aSuccess = mSamplesInfo[theSampleID].mSoundInstance->loadSample(anOggFilePath.c_str(), theVolume);
			aFilePath = anOggFilePath;
		}
		if(!aSuccess) // attemp WAV extension
		{
			aSuccess = mSamplesInfo[theSampleID].mSoundInstance->loadSample(aWavFilePath.c_str(), theVolume);
			aFilePath = aWavFilePath;
		}
		if(!gSexyAppBase->HasFocus()) KSound::suspend();

		if (aSuccess == true)
		{
			// save filename and volume for this sample
			mSamplesInfo[theSampleID].mFileName = theFileName;
			mSamplesInfo[theSampleID].mInitialVolume = theVolume;
			mSamplesInfo[theSampleID].mCurrentVolume = theVolume;
		}
		else
		{
			// loading fails, delete the SoundInstance and clear the place
			delete mSamplesInfo[theSampleID].mSoundInstance;
			mSamplesInfo[theSampleID].mSoundInstance = NULL;
		}

		return aSuccess;
	}


}

/*!***********************************
// @return    	int a new unique SampleID or -1 for failure
// @param     	theFileName filename of the sample
// @param     	theVolume optional initial volume [0..100]
//
// \brief		load a sample, the SampleID will be auto assigned
// \details 	<b>Description:</b> load a sample, the SampleID will be auto assigned
//************************************/
int SoundManager::LoadSample(const SexyString& theFileName, int theVolume)
{
	unsigned int aSampleID;
	int aFailedID = -1;

	// If we've already loaded the sample, just return its ID. (JSullivan)
	for (aSampleID = 0; aSampleID < MAX_SOUND_SAMPLES; aSampleID++)
	{
		if (mSamplesInfo[aSampleID].mFileName == theFileName)
		{
			return aSampleID;
		}
	}

	// We haven't already loaded the sample, so find an available ID and 
	// try to load it, then return the new ID. (JSullivan)
	for (aSampleID = MAX_SOUND_SAMPLES - 1; aSampleID >= 0; aSampleID--)
	{		
		if (mSamplesInfo[aSampleID].mSoundInstance == NULL)
		{
			if (!LoadSample(aSampleID, theFileName, theVolume))
			{
				return aFailedID;
			}
			else
			{
				return aSampleID;
			}
		}
	}	

	return aFailedID;
}

/*!***********************************
// @return    	void
//
// \brief		release all previous loaded samples
// \details 	<b>Description:</b>release all previous loaded samples 
//************************************/
void SoundManager::ReleaseSamples()
{
	for (unsigned int i = 0; i < MAX_SOUND_SAMPLES; i++)
	{
		ReleaseSample(i);
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
//
// \brief		release a loaded sample with the given SampleID
// \details 	<b>Description:</b> release a loaded sample with the given SampleID
//************************************/
void SoundManager::ReleaseSample(unsigned int theSampleID)
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	// free and delete the SoundInstance and clear the place
	if (mSamplesInfo[theSampleID].mSoundInstance != NULL)
	{
		mSamplesInfo[theSampleID].mSoundInstance->stopSample();
		mSamplesInfo[theSampleID].mSoundInstance->freeSound();

		delete mSamplesInfo[theSampleID].mSoundInstance;
		mSamplesInfo[theSampleID].mSoundInstance = NULL;
	}

	mSamplesInfo[theSampleID].mFileName			= _S(""); 
	mSamplesInfo[theSampleID].mInitialVolume	= MIN_VOLUME;
	mSamplesInfo[theSampleID].mFadeVolume		= MIN_VOLUME;
	mSamplesInfo[theSampleID].mCurrentVolume	= MIN_VOLUME;
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
//
// \brief		immediately play a sample with the given ID
// \details 	<b>Description:</b> immediately play a sample with the given ID
//************************************/
void SoundManager::PlaySample(unsigned int theSampleID)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::PlaySample: WARNING : Invalid sample ID.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		if (mSamplesInfo[theSampleID].mSoundInstance != NULL)
		{
			mSamplesInfo[theSampleID].mSoundInstance->playSample();
			// remember the starting time
			mSampleTimes.insert(UIntUIntMap::value_type(theSampleID, mUpdateCnt));
		}
		else
		{
			K_LOG("Sexy::SoundManager::PlaySample: WARNING : No sound instance for specified sample ID.");
		}
	}	
}


/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theMinFrames minimum amount of frames between duplicates
//
// \brief		play a sample with the given ID
// \details 	<b>Description:</b> play a sample with the given ID, if the number of 
//				theMinFrames has not yet been reached, the sample does not play
//************************************/
void SoundManager::PlaySample(unsigned int theSampleID, unsigned int theMinFrames)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::PlaySample: WARNING : Invalid sample ID.");
		return;
	}

	UIntUIntMap::iterator aIterator = mSampleTimes.find(theSampleID);

	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	// make sure we aren't playing the sample too recently
	if ((aIterator == mSampleTimes.end()) || (mUpdateCnt - aIterator->second > theMinFrames))
	{
		if (mSamplesInfo[theSampleID].mSoundInstance != NULL)
		{
			mSamplesInfo[theSampleID].mSoundInstance->playSample();
			mSampleTimes.insert(UIntUIntMap::value_type(theSampleID, mUpdateCnt));
		}
		else
		{
			K_LOG("Sexy::SoundManager::PlaySample: WARNING : No sound instance for specified sample ID.");
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theFramesDelay amount of frames to wait before playing
//
// \brief		play a sample delayed
// \details 	<b>Description:</b> play a sample delayed
//************************************/
void SoundManager::PlaySampleDelayed(unsigned int theSampleID, unsigned int theFramesDelay)
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	// playback of this sample is started via Update()
	mDelayedSFX.push_back(UIntUIntPairVector::value_type(theSampleID, theFramesDelay));
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theVolume new sample volume
//
// \brief		set the volume of a sample
// \details 	<b>Description:</b> set the volume of a sample
//************************************/
void SoundManager::SetSampleVolume(unsigned int theSampleID, int theVolume)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::SetSampleVolume: WARNING : Invalid sample ID.");
		return;
	}
	
	// clamp the Volume
	if (theVolume < MIN_VOLUME) theVolume = MIN_VOLUME;
	if (theVolume > MAX_VOLUME) theVolume = MAX_VOLUME;
	
	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		if (mSamplesInfo[theSampleID].mSoundInstance != NULL)
		{
			mSamplesInfo[theSampleID].mCurrentVolume = theVolume;
			mSamplesInfo[theSampleID].mSoundInstance->setVolume(theVolume);

			// if playing a looping sample, then change volume of looping sample
			LoopingSampleMap::iterator aIterator = mLoopingSamples.find(theSampleID);
			if (aIterator != mLoopingSamples.end())
				aIterator->second.first->setVolume(theVolume);
		}
		else
		{
			K_LOG("Sexy::SoundManager::SetSampleVolume: WARNING : No sound instance for specified sample ID.");
		}
	}

}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theVolumeInc volume increment
//
// \brief		Increment (or decrement if negative) the volume of the sample
// \details 	<b>Description:</b>  Increment (or decrement if negative) the volume of the sample
//************************************/
void SoundManager::IncSampleVolume(unsigned int theSampleID, int theVolumeInc)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::IncSampleVolume: WARNING : Invalid sample ID.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;

		if (aSoundInstance != NULL)
		{
			// get, increment and clamp the volume
			int aVolume = mSamplesInfo[theSampleID].mCurrentVolume + theVolumeInc;
			if (aVolume < MIN_VOLUME) aVolume = MIN_VOLUME;
			if (aVolume > MAX_VOLUME) aVolume = MAX_VOLUME;
			
			mSamplesInfo[theSampleID].mCurrentVolume = aVolume;
			aSoundInstance->setVolume(aVolume);

			// if playing a looping sample, then change volume of looping sample
			LoopingSampleMap::iterator aIterator = mLoopingSamples.find(theSampleID);
			if (aIterator != mLoopingSamples.end())
				aIterator->second.first->setVolume(aVolume);
		}
		else
		{
			K_LOG("Sexy::SoundManager::IncSampleVolume: WARNING : No sound instance for specified sample ID.");
		}
	}
}

/*!***********************************
// @return    	int the initial volume
// @param     	theSampleID unique sample identification
//
// \brief		return the initial volume of the sample
// \details 	<b>Description:</b> return the initial volume of the sample
//************************************/
int SoundManager::GetInitialSampleVolume(unsigned int theSampleID)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::GetInitialSampleVolume: WARNING : Invalid sample ID.");
		return MIN_VOLUME;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);
		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;
		int aVolume = MIN_VOLUME;

		if (aSoundInstance != NULL)
			aVolume = mSamplesInfo[theSampleID].mInitialVolume;
		else
			K_LOG("Sexy::SoundManager::GetInitialSampleVolume: WARNING : No sound instance for specified sample ID.");

		return aVolume;
	}

}

/*!***********************************
// @return    	int the current volume
// @param     	theSampleID unique sample identification
//
// \brief		return the current volume of the sample
// \details 	<b>Description:</b> return the current volume of the sample
//************************************/
int	SoundManager::GetCurrentSampleVolume(unsigned int theSampleID)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::GetCurrentSampleVolume: WARNING : Invalid sample ID.");
		return MIN_VOLUME;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;
		int aVolume = MIN_VOLUME;

		if (aSoundInstance != NULL)
		{
			aVolume = mSamplesInfo[theSampleID].mCurrentVolume;
		}
		else
		{
			K_LOG("Sexy::SoundManager::GetCurrentSampleVolume: WARNING : No sound instance for specified sample ID.");
		}

		return aVolume;
	}
}

/*!***********************************
// @return    	void
// @param     	theFirstSampleID unique identification of the first sample
// @param     	theNextSampleID unique identification of the next sample
// @param     	theFramesDelay time delay to elapse after the original sample 
//							   completes before triggering the next sample
//
// \brief		play a sample, when it is done, will play the next sample
// \details 	<b>Description:</b> play a sample, when it is done, will play the next sample
//************************************/
void SoundManager::PlaySamplesChained(unsigned int theFirstSampleID, unsigned int theNextSampleID, unsigned int theFramesDelay)
{
	if ((theFirstSampleID < 0) || (theFirstSampleID >= MAX_SOUND_SAMPLES) || (theNextSampleID < 0) || (theNextSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::PlaySamplesChained: WARNING : Invalid sample ID.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theFirstSampleID].mSoundInstance;

		if (aSoundInstance != NULL && mSamplesInfo[theNextSampleID].mSoundInstance != NULL)
		{
			aSoundInstance->playSample(); 
			// Update() takes care, that theNextSampleID will be played after theFirstSampleID is done playing
			mChainedSFX.push_back(ChainedSamples(theFirstSampleID, theNextSampleID, theFramesDelay, aSoundInstance));
		}
		else
		{
			K_LOG("Sexy::SoundManager::PlaySamplesChained: WARNING : No sound instance for specified sample ID.");
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
//
// \brief		stop a playing sample
// \details 	<b>Description:</b> stop a playing sample
//************************************/
void SoundManager::StopSample(unsigned int theSampleID)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::StopSample: WARNING : Invalid sample ID.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;

		if (aSoundInstance != NULL)
			aSoundInstance->stopSample();
		else
			K_LOG("Sexy::SoundManager::StopSample: WARNING : No sound instance for specified sample ID.");
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	thePause true to pause, false to resume
//
// \brief		pause or resume playing the sample
// \details 	<b>Description:</b> pause or resume playing the sample
//************************************/
void SoundManager::PauseSample(unsigned int theSampleID, bool thePause)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::PauseSample: WARNING : Invalid sample ID.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);
		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;

		if (aSoundInstance != NULL)
			aSoundInstance->pause(thePause);
		else
			K_LOG("Sexy::SoundManager::PauseSample: WARNING : No sound instance for specified sample ID.");
	}
}

/*!***********************************
// @return    	bool true if playing, false if not
// @param     	theSampleID unique sample identification
//
// \brief		return true if the sample is playing, false if not
// \details 	<b>Description:</b> return true if the sample is playing, false if not 
//************************************/
bool SoundManager::IsSamplePlaying(unsigned int theSampleID)
{
	if ((theSampleID < 0) || (theSampleID >= MAX_SOUND_SAMPLES))
	{
		K_LOG("Sexy::SoundManager::IsSamplePlaying: WARNING : Invalid sample ID.");
		return false;
	}
	
	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;

		if (aSoundInstance != NULL)
			return aSoundInstance->isPlaying();
		else
			K_LOG("Sexy::SoundManager::IsSamplePlaying: WARNING : No sound instance for specified sample ID.");
	}

	return false;
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theFadeInSpeed fade in speed (based on frames), 0.0f = no fade in
//
// \brief		loop the given sample with no delay for a continuous sound
// \details 	<b>Description:</b> loop the given sample with no delay for a continuous sound
//************************************/
void SoundManager::LoopSample(unsigned int theSampleID, float theFadeInSpeed)
{
	// don't allow "overlapping" loops of the same sample
	if (mLoopingSamples.find(theSampleID) != mLoopingSamples.end())
		return;

	// fade in speed must be > 0
	if (theFadeInSpeed < 0.0f)
	{
		K_LOG("Sexy::SoundManager::LoopSample: WARNING : Fade out speed is < 0.");
		return;
	}

	{
		// Make sure that the mSamplesInfo container is thread-safe.
		AutoCrit anAutoLock(mSoundManagerCritSect);

		KSound* aSoundInstance = mSamplesInfo[theSampleID].mSoundInstance;

		if (aSoundInstance != NULL)
		{
			if (theFadeInSpeed != 0.0f)
			{
				// fade in the looping sample, start with volume 0
				mSamplesInfo[theSampleID].mFadeVolume = static_cast<float>(MIN_VOLUME);
				// set the volume
				aSoundInstance->setVolume(MIN_VOLUME);
			}
			else
			{
				// set the current volume, perhaps the sample was faded out before
				aSoundInstance->setVolume(mSamplesInfo[theSampleID].mCurrentVolume);
			}

			aSoundInstance->playSample(); 

			// save the info for the looping sample in the map
			// a positive value for the fade in speed is passed, so update knows that 
			// the sample should fade in
			mLoopingSamples[theSampleID] = std::pair<KSound*, float>(aSoundInstance, theFadeInSpeed);
		}
		else
		{
			K_LOG("Sexy::SoundManager::LoopSample: WARNING : No sound instance for specified sample ID.");
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
//
// \brief		stop a looping sample
// \details 	<b>Description:</b> stop a looping sample, if the sample is not looping, the call is ignored.
//************************************/
void SoundManager::StopLoopingSample(unsigned int theSampleID)
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	LoopingSampleMap::iterator aIterator = mLoopingSamples.find(theSampleID);

	if (aIterator == mLoopingSamples.end())
	{
		return;
	}

	mLoopingSamples.erase(aIterator);
}

/*!***********************************
// @return    	void
// @param     	theMute true = mute looping samples, false = unmute looping samples
//
// \brief		mute/unmute looping samples
// \details 	<b>Description:</b> mute/unmute looping samples
//************************************/
void SoundManager::MuteLoopingSamples(bool theMute)
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	mMuteCount += theMute ? 1 : -1;
	
	if (mMuteCount < 0)
		mMuteCount = 0;

	if ((!theMute && (mMuteCount == 0)) || theMute)
	{
		for (LoopingSampleMap::iterator aIterator = mLoopingSamples.begin(); aIterator != mLoopingSamples.end(); ++aIterator)
		{
			// mute or set current volume
			aIterator->second.first->setVolume(theMute ? 0 : mSamplesInfo[aIterator->first].mCurrentVolume);
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theSampleID unique sample identification
// @param     	theFadeOutSpeed fade out speed (based on frames)
//
// \brief		fade out a looping sample : runs at 100Hz
// \details 	<b>Description:</b> fade out a looping sample : runs at 100Hz
//************************************/
void SoundManager::FadeOutLoopingSample(unsigned int theSampleID, float theFadeOutSpeed)
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	LoopingSampleMap::iterator aIterator = mLoopingSamples.find(theSampleID);

	if (aIterator == mLoopingSamples.end())
	{
		K_LOG("Sexy::SoundManager::FadeOutLoopingSample: WARNING : Trying to fade out a not looping sample.");
		return;
	}

	// fade out speed must be > 0
	if (theFadeOutSpeed < 0.0f)
	{
		K_LOG("Sexy::SoundManager::FadeOutLoopingSample: WARNING : Fade out speed is < 0.");
		return;
	}

	// save the current volume for fade
	mSamplesInfo[theSampleID].mFadeVolume = static_cast<float>(mSamplesInfo[theSampleID].mCurrentVolume);

	// store a negative fade out speed in the map, so Update() knows 
	// that the sample should fade out
	aIterator->second.second = - theFadeOutSpeed;
}

/*!***********************************
// @return    	void
//
// \brief		update play delayed, chained and fade
// \details 	<b>Description:</b> check the delay list and start playing samples,
//				poll the chained samples and start playing the next sample
//				check/poll the looping samples, do fade in/out 
//************************************/
void SoundManager::Update()
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	// increment update count to measure frames
	mUpdateCnt++;

	// check the delayed list and play the sample if delay time is over
	for (unsigned int i = 0; i < mDelayedSFX.size(); i++)
	{
		if (mDelayedSFX[i].second-- <= 0)
		{
			if ((mDelayedSFX[i].first < 0) || (mDelayedSFX[i].first >= MAX_SOUND_SAMPLES))
				K_LOG("Sexy::SoundManager::Update: WARNING : Invalid sample ID.");
			else
			{
				if (mSamplesInfo[mDelayedSFX[i].first].mSoundInstance != NULL)
				{
					mSamplesInfo[mDelayedSFX[i].first].mSoundInstance->playSample();
					// remember the starting time
					mSampleTimes.insert(UIntUIntMap::value_type(mDelayedSFX[i].first, mUpdateCnt));
				}
				else
					K_LOG("Sexy::SoundManager::Update: WARNING : No sound instance for specified sample ID.");
			}
			mDelayedSFX.erase(mDelayedSFX.begin() + i);
			i--;
		}
	}

	// poll the samples and query whether or not they are done playing
	// when one is done, trigger the next after the desired delay has elapsed
	for (unsigned int i = 0; i < mChainedSFX.size(); i++)
	{			
		if (!mChainedSFX[i].mFirstSoundInstance->isPlaying())
		{
			if (--mChainedSFX[i].mFramesDelay <= 0)
			{
				if ((mChainedSFX[i].mNextSampleID < 0) || (mChainedSFX[i].mNextSampleID >= MAX_SOUND_SAMPLES))
					K_LOG("Sexy::SoundManager::Update: WARNING : Invalid sample ID.");
				else
				{
					if (mSamplesInfo[mChainedSFX[i].mNextSampleID].mSoundInstance != NULL)
					{
						mSamplesInfo[mChainedSFX[i].mNextSampleID].mSoundInstance->playSample();
						// remember the starting time
						mSampleTimes.insert(UIntUIntMap::value_type(mChainedSFX[i].mNextSampleID, mUpdateCnt));
					}
					else
						K_LOG("Sexy::SoundManager::Update: WARNING : No sound instance for specified sample ID.");
				}
				mChainedSFX.erase(mChainedSFX.begin() + i);
				--i;
			}
		}
	}

	// check/poll looping samples
	for (LoopingSampleMap::iterator aIterator = mLoopingSamples.begin(); aIterator != mLoopingSamples.end(); )
	{
		// get the fade speed
		float aFadeSpeed = aIterator->second.second;

		// do we need to fade the sample?
		if (aFadeSpeed != 0.0f)
		{
			// get the fade volume of the looping sample
			float aVolume = mSamplesInfo[aIterator->first].mFadeVolume;
			// this is a helper, just to prevent: if (aVolume == 0.0f)
			int aFadeEnded = 0;

			// always add the fade speed, fade out speed values are < 0
			aVolume += aFadeSpeed;

			// clamp
			if (aVolume < 0.0f)
			{
				aVolume = 0.0f;
				aFadeEnded = -1;
			}
			if (static_cast<int>(aVolume) > mSamplesInfo[aIterator->first].mCurrentVolume) 
			{
				aVolume = static_cast<float>(mSamplesInfo[aIterator->first].mCurrentVolume);
				aFadeEnded = 1;
			}
			
			// save the fade volume
			mSamplesInfo[aIterator->first].mFadeVolume = aVolume;

			// set the new volume
			aIterator->second.first->setVolume(static_cast<int>(aVolume));

			if (aFadeEnded == -1)
			{
				// fade out completed, volume is MIN_VOLUME, remove the looping sample from the map
				mLoopingSamples.erase(aIterator);
				//continue;
				break;
			}
			
			if (aFadeEnded == 1)
			{
				// fade in completed, max volume reached, clear fade speed
				aIterator->second.second = 0.0f;
				break;
			}
		}
		
		// start another loop of the sample ?
		if (!aIterator->second.first->isPlaying())
		{
			if ((aIterator->first < 0) || (aIterator->first >= MAX_SOUND_SAMPLES))
				K_LOG("Sexy::SoundManager::Update: WARNING : Invalid sample ID.");
			else
			{
				if (mSamplesInfo[aIterator->first].mSoundInstance != NULL)
				{
					mSamplesInfo[aIterator->first].mSoundInstance->playSample();
					// remember the starting time
					mSampleTimes.insert(UIntUIntMap::value_type(aIterator->first, mUpdateCnt));
				}
				else
					K_LOG("Sexy::SoundManager::Update: WARNING : No sound instance for specified sample ID.");
			}
		}

		++aIterator;
	}
}

/*!***********************************
// @return    	int -1 if failure, otherwise a slot ID between 0 and MAX_SOUND_SAMPLES
//
// \brief		Returns an empty sound sample slot ID.
// \details 	<b>Description:</b> Returns -1 if failed.
//************************************/
int Sexy::SoundManager::GetFreeSampleId()
{
	// Make sure that the mSamplesInfo container is thread-safe.
	AutoCrit anAutoLock(mSoundManagerCritSect);

	// We haven't already loaded the sample, so find an available ID and 
	// try to load it, then return the new ID. (JSullivan)
	for (int aSampleID = MAX_SOUND_SAMPLES - 1; aSampleID >= 0; aSampleID--)
	{		
		if (mSamplesInfo[aSampleID].mSoundInstance == NULL)
		{
			return aSampleID;
		}
	}

	return -1;
}

/*!***********************************
// @return    	void
//
// \brief		Stops all sound samples that are currently playing.
// \details 	<b>Description:</b> Stops all sound samples that are currently playing.
//************************************/
void Sexy::SoundManager::StopAllSamples()
{
	for (int aSampleID = MAX_SOUND_SAMPLES - 1; aSampleID >= 0; aSampleID--)
	{		
		if (mSamplesInfo[aSampleID].mSoundInstance != NULL)
		{
			StopSample(aSampleID);
		}
	}
}