// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji MusicManager Version 0.5
	-------------------------------------------------------------	
	Created:	2010/02/25
	Filename: 	SexyKanji\MusicManager.cpp
	
	Purpose:	SexyAppFramework Wrapper class for music streams
				play streams, looping, fading

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#include "MusicManager.h"

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
MusicManager::MusicManager()
{
	// init stream information
	for (unsigned int i = 0; i < MAX_MUSIC_STREAMS; i++)
	{
		mStreamsInfo[i].mSoundInstance	= NULL;
		mStreamsInfo[i].mFileName		= _S(""); 
		mStreamsInfo[i].mInitialVolume	= MIN_VOLUME;
		mStreamsInfo[i].mFadeVolume		= MIN_VOLUME;
		mStreamsInfo[i].mCurrentVolume	= MIN_VOLUME;
	}

	mReservedStreamIDs.clear();
}

/*!***********************************
// @return    	
//
// \brief		 Destructor, releases all streams
// \details 	<b>Description:</b>  Destructor, releases all streams
//************************************/
MusicManager::~MusicManager()
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	mReservedStreamIDs.clear();

	ReleaseStreams();
}

/*!***********************************
// @return    	bool
// @param     	theStreamID unique stream identification
// @param     	theFileName filename of the stream
//
// \brief		load stream with a specified ID
// \details 	<b>Description:</b> load stream with a specified ID, any previously 
//				loaded stream with this ID will be released! 
//************************************/
bool MusicManager::LoadStream(unsigned int theStreamID, SexyString theFileName)
{
	// must pass in a valid StreamID
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::LoadStream: WARNING : Invalid stream ID.");
		return false;
	}

	// make sure any stream already using this ID is released first
	ReleaseStream(theStreamID);

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		// get a new sound instance and load the stream
		mStreamsInfo[theStreamID].mSoundInstance = new KSound();
		KSound::resume();
		bool aSuccess = mStreamsInfo[theStreamID].mSoundInstance->loadStream(SexyStringToStringFast(theFileName).c_str());
		if(!gSexyAppBase->HasFocus()) KSound::suspend();

		if (aSuccess == true)
		{
			// save filename and volume for this stream
			mStreamsInfo[theStreamID].mFileName = theFileName;
			mStreamsInfo[theStreamID].mInitialVolume = DEFAULT_MUSIC_VOLUME;
			mStreamsInfo[theStreamID].mCurrentVolume = DEFAULT_MUSIC_VOLUME;
		}
		else
		{
			// loading fails, delete the SoundInstance and clear the place
			delete mStreamsInfo[theStreamID].mSoundInstance;
			mStreamsInfo[theStreamID].mSoundInstance = NULL;
		}

		return aSuccess;
	}
}

/*!***********************************
// @return    	int
// @param     	theFileName filename of the stream
// @param     	theForceLoad  default = false, true -> force to load stream, even if it's already loaded
//
// \brief		load a stream, the StreamID will be auto assigned
// \details 	<b>Description:</b> load a stream, the StreamID will be auto assigned.
//				returns a new unique StreamID or -1 for failure
//************************************/
int MusicManager::AutoLoadStream(SexyString theFileName, bool theForceLoad /* = false */)
{
	unsigned int aStreamID;

	// if we've already loaded the stream, and it's not a 
	// variant (theForceLoad == false), just return its ID
	if (theForceLoad == false)
	{
		for (aStreamID = 0; aStreamID < MAX_MUSIC_STREAMS; aStreamID++)
		{
			if (mStreamsInfo[aStreamID].mFileName == theFileName)
			{
				return aStreamID;
			}
		}
	}

	// we haven't already loaded the stream or it's a variant, so find 
	// an available ID and try to load it, then return the new ID
	aStreamID = GetFreeStreamID();

	if (aStreamID > MAX_MUSIC_STREAMS)
	{
		K_LOG("Sexy::MusicManager::AutoLoadStream: WARNING : No valid stream ID left.");
	}
	else if (LoadStream(aStreamID, theFileName) == true)
	{
		return aStreamID;
	}

	return -1;
}

/*!***********************************
// @return    	void
//
// \brief		release all previous loaded streams
// \details 	<b>Description:</b> release all previous loaded streams
//************************************/
void MusicManager::ReleaseStreams()
{
	for (unsigned int i = 0; i < MAX_MUSIC_STREAMS; i++)
	{
		ReleaseStream(i);
	}
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
//
// \brief		release a loaded stream with the given StreamID
// \details 	<b>Description:</b> release a loaded stream with the given StreamID
//************************************/
void MusicManager::ReleaseStream(unsigned int theStreamID)
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	// free and delete the SoundInstance and clear the place
	if (mStreamsInfo[theStreamID].mSoundInstance != NULL)
	{
		mStreamsInfo[theStreamID].mSoundInstance->stopStream();
		mStreamsInfo[theStreamID].mSoundInstance->freeSound();

		delete mStreamsInfo[theStreamID].mSoundInstance;
		mStreamsInfo[theStreamID].mSoundInstance = NULL;
	}

	mStreamsInfo[theStreamID].mFileName			= _S(""); 
	mStreamsInfo[theStreamID].mInitialVolume	= MIN_VOLUME;
	mStreamsInfo[theStreamID].mFadeVolume		= MIN_VOLUME;
	mStreamsInfo[theStreamID].mCurrentVolume	= MIN_VOLUME;
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
//
// \brief		immediately play a stream with the given ID
// \details 	<b>Description:</b> immediately play a stream with the given ID
//************************************/
void MusicManager::PlayStream(unsigned int theStreamID)
{
	PlayStream(theStreamID, true, 0.0f);
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	theLoop true to loop stream indefinitely when playing, false to only play once and then stop
//
// \brief		immediately play a stream with the given ID
// \details 	<b>Description:</b> 
//				optionally loop the stream
//************************************/
void MusicManager::PlayStream(unsigned int theStreamID, bool theLoop)
{
	PlayStream(theStreamID, theLoop, 0.0f);
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	bLoop true to loop stream indefinitely when playing, false to only play once and then stop
// @param     	theFadeInSpeed fade in speed (based on frames), 0.0f = no fade in
//
// \brief		immediately play a stream with the given ID
// \details 	<b>Description:</b> immediately play a stream with the given ID
//				optionally fade the stream in and loop	
//************************************/
void MusicManager::PlayStream(unsigned int theStreamID, bool bLoop, float theFadeInSpeed)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::PlayStream: WARNING : Invalid stream ID.");
		return;
	}

	// fade in speed must be > 0
	if (theFadeInSpeed < 0.0f)
	{
		K_LOG("Sexy::MusicManager::PlayStream: WARNING : Invalid fade speed (must be > 0).");
		return;
	}

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;

		if (aSoundInstance != NULL)
		{
			if (theFadeInSpeed != 0.0f)
			{
				// fade in the stream, start with volume 0
				mStreamsInfo[theStreamID].mFadeVolume = static_cast<float>(MIN_VOLUME);
				// set the volume
				aSoundInstance->setVolume(MIN_VOLUME);

				// save the info for the fading stream in the map
				// a positive value for the fade in speed is passed, so update knows that 
				// the stream should fade in
				mFadingStreams[theStreamID] = std::pair<KSound*, float>(aSoundInstance, theFadeInSpeed);
			}

			mStreamsInfo[theStreamID].mSoundInstance->playStream(bLoop);
		}
		else
		{
			K_LOG("Sexy::MusicManager::PlayStream: WARNING : No sound instance for specified stream ID.");
		}
	}
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	theVolume new stream volume
//
// \brief		set the volume of a stream
// \details 	<b>Description:</b> set the volume of a stream
//************************************/
void MusicManager::SetStreamVolume(unsigned int theStreamID, int theVolume)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::SetStreamVolume: WARNING : Invalid stream ID.");
		return;
	}
	
	// clamp the Volume
	if (theVolume < MIN_VOLUME) theVolume = MIN_VOLUME;
	if (theVolume > MAX_VOLUME) theVolume = MAX_VOLUME;
	
	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		// set the new volume
		if (mStreamsInfo[theStreamID].mSoundInstance != NULL)
		{
			mStreamsInfo[theStreamID].mCurrentVolume = theVolume;
			mStreamsInfo[theStreamID].mSoundInstance->setVolume(theVolume);
		}
		else
		{
			K_LOG("Sexy::MusicManager::SetStreamVolume: WARNING : No sound instance for specified stream ID.");
		}	
	}
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	theVolumeInc volume increment
//
// \brief		Increments (or decrements if negative) the volume of the stream
// \details 	<b>Description:</b> Increments (or decrements if negative) the volume of the stream
//************************************/
void MusicManager::IncStreamVolume(unsigned int theStreamID, int theVolumeInc)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::IncStreamVolume: WARNING : Invalid stream ID.");
		return;
	}

	// Inner scope
	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;

		if (aSoundInstance != NULL)
		{
			// get, increment and clamp the volume
			int aVolume = mStreamsInfo[theStreamID].mCurrentVolume + theVolumeInc;
			if (aVolume < MIN_VOLUME) aVolume = MIN_VOLUME;
			if (aVolume > MAX_VOLUME) aVolume = MAX_VOLUME;
			
			// set the volume
			mStreamsInfo[theStreamID].mCurrentVolume = aVolume;
			aSoundInstance->setVolume(aVolume);
		}
		else
		{
			K_LOG("Sexy::MusicManager::IncStreamVolume: WARNING : No sound instance for specified stream ID.");
		}
	}
}

/*!***********************************
// @return    	int
// @param     	theStreamID unique stream identification
//
// \brief		returns the current volume of the stream
// \details 	<b>Description:</b> returns the current volume of the stream
//************************************/
int MusicManager::GetCurrentStreamVolume(unsigned int theStreamID)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::GetCurrentStreamVolume: WARNING : Invalid stream ID.");
		return MIN_VOLUME;
	}

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;
		int aVolume = MIN_VOLUME;

		if (aSoundInstance != NULL)
		{
			FadingStreamMap::iterator aIterator = mFadingStreams.find(theStreamID);

			// is stream currently fading ?
			if (aIterator == mFadingStreams.end())
			{
				// no fade, return current volume
				aVolume = mStreamsInfo[theStreamID].mCurrentVolume;
			}
			else
			{
				// stream is fading, return fade volume
				aVolume = static_cast<int>(mStreamsInfo[theStreamID].mFadeVolume);
			}
		}
		else
		{
			K_LOG("Sexy::MusicManager::GetCurrentStreamVolume: WARNING : No sound instance for specified stream ID.");
		}

		return aVolume;
	}
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
//
// \brief		stop a playing stream
// \details 	<b>Description:</b>  stop a playing stream
//************************************/
void MusicManager::StopStream(unsigned int theStreamID)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::StopStream: WARNING : Invalid stream ID.");
		return;
	}

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;
		if (aSoundInstance != NULL)
			aSoundInstance->stopStream();
		else
			K_LOG("Sexy::MusicManager::StopStream: WARNING : No sound instance for specified stream ID.");
	}
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	thePause true to pause, false to resume
//
// \brief		 pause or resume playing the stream
// \details 	<b>Description:</b>  pause or resume playing the stream
//************************************/
void MusicManager::PauseStream(unsigned int theStreamID, bool thePause)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::PauseStream: WARNING : Invalid stream ID.");
		return;
	}

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;

		if (aSoundInstance != NULL)
			aSoundInstance->pause(thePause);
		else
			K_LOG("Sexy::MusicManager::PauseStream: WARNING : No sound instance for specified stream ID.");
	}

}

/*!***********************************
// @return    	bool
// @param     	theStreamID unique stream identification
//
// \brief		returns true if the stream is playing, false if not
// \details 	<b>Description:</b> returns true if the stream is playing, false if not
//************************************/
bool MusicManager::IsStreamPlaying(unsigned int theStreamID)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::IsStreamPlaying: WARNING : Invalid stream ID.");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Contributed by SergeyL:
	FadingStreamMap::iterator itFading = mFadingStreams.find(theStreamID);
	if (itFading != mFadingStreams.end() && itFading->first != NULL)
		return true;

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);
		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;

		if (aSoundInstance != NULL)
			return aSoundInstance->isPlaying();
		else
			K_LOG("Sexy::MusicManager::IsStreamPlaying: WARNING : No sound instance for specified stream ID.");
	}
	
	return false;
}

/*!***********************************
// @return    	void
// @param     	theStreamID unique stream identification
// @param     	theFadeOutSpeed fade out speed (based on frames)
//
// \brief		fades out a playing stream
// \details 	<b>Description:</b> fades out a playing stream
//************************************/
void MusicManager::FadeOutStream(unsigned int theStreamID, float theFadeOutSpeed)
{
	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::FadeOutStream: WARNING : Invalid stream ID.");
		return;
	}

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		KSound* aSoundInstance = mStreamsInfo[theStreamID].mSoundInstance;

		if (aSoundInstance == NULL)
		{
			K_LOG("Sexy::MusicManager::FadeOutStream: WARNING : No sound instance for specified stream ID.");
			return;
		}

		if (!aSoundInstance->isPlaying())
		{
			K_LOG("Sexy::MusicManager::FadeOutStream: WARNING : Trying to fade out a not playing stream.");
			return;
		}

		// fade out speed must be > 0
		if (theFadeOutSpeed < 0.0f)
		{
			K_LOG("Sexy::MusicManager::FadeOutStream: WARNING : Fade out speed is < 0.");
			return;
		}

//	Let's test before we add this line: (JPOAG)
// 	if (mFadingStreams.find(theStreamID) != mFadingStreams.end()) 
// 		return; // already fading out: avoid the same stream (JPOAG)

		// save the current volume for fade
		mStreamsInfo[theStreamID].mFadeVolume = static_cast<float>(mStreamsInfo[theStreamID].mCurrentVolume);

		// save the info for the fading stream in the map
		// a negative value for the fade out speed is passed, so update knows that 
		// the stream should fade out
		mFadingStreams[theStreamID] = std::pair<KSound*, float>(aSoundInstance, -theFadeOutSpeed);
	}
}

/*!***********************************
// @return    	unsigned int
//
// \brief		get a free stream ID
// \details 	<b>Description:</b> get a free stream ID
//************************************/
unsigned int MusicManager::GetFreeStreamID()
{
	unsigned int aStreamID;

	{
		// Make sure that the mStreamsInfo container is thread-safe.
		AutoCrit anAutoLock(mMusicManagerCritSect);

		for (aStreamID = MAX_MUSIC_STREAMS - 1; aStreamID >= 0; aStreamID--)
		{		
			if (mStreamsInfo[aStreamID].mSoundInstance == NULL && 
				mReservedStreamIDs.find(aStreamID) == mReservedStreamIDs.end())
			{
				return aStreamID;
			}
		}
	}

	return MAX_MUSIC_STREAMS + 1;
}

/*!***********************************
// @return    	void
// @param     	theStreamID
//
// \brief		reserve a free stream ID
// \details 	<b>Description:</b> reserve a free stream ID, call GetFreeStreamID() to get a free ID
//************************************/
void MusicManager::ReserveStreamID(unsigned int theStreamID)
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	if (mReservedStreamIDs.find(theStreamID) == mReservedStreamIDs.end())
		mReservedStreamIDs.insert(theStreamID);
}

/*!***********************************
// @return    	void
// @param     	theStreamID
//
// \brief		free a reserved stream ID
// \details 	<b>Description:</b> free a reserved stream ID (reserved by ReserveStreamID())
//************************************/
void MusicManager::FreeReservedStreamID(unsigned int theStreamID)
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	if (mReservedStreamIDs.find(theStreamID) != mReservedStreamIDs.end())
		mReservedStreamIDs.erase(theStreamID);
}

/*!***********************************
// @return    	void
//
// \brief		do fade in/out; called at 100Hz (JPOAG)
// \details 	<b>Description:</b> do fade in/out; called at 100Hz (JPOAG)
//************************************/
void MusicManager::Update()
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	// check fading streams
	for (FadingStreamMap::iterator aIterator = mFadingStreams.begin(); aIterator != mFadingStreams.end(); )
	{
		// get the fade speed
		float aFadeSpeed = aIterator->second.second;

		// do we need to fade the stream?
		if (aFadeSpeed != 0.0f)
		{
			// get the fade volume of the fading stream
			float aVolume = mStreamsInfo[aIterator->first].mFadeVolume;
			// this is a helper, just to prevent: if (aVolume == 0.0f)
			int aFadeEnded = 0;

			// always add the fade speed, fade out speed values are < 0, fade in speed values are > 0
			aVolume += aFadeSpeed;

			// clamp
			if (aVolume < 0.0f)
			{
				aVolume = 0.0f;
				aFadeEnded = -1;
			}
			if (static_cast<int>(aVolume) > mStreamsInfo[aIterator->first].mCurrentVolume) 
			{
				aVolume = static_cast<float>(mStreamsInfo[aIterator->first].mCurrentVolume);
				aFadeEnded = 1;
			}
			
			// save the fade volume
			mStreamsInfo[aIterator->first].mFadeVolume = aVolume;

			// set the new volume
			if (aIterator->second.first)
				aIterator->second.first->setVolume(static_cast<int>(aVolume));

			if (aFadeEnded == -1)
			{
				// fade out completed, volume is MIN_VOLUME, clear fade speed
				aIterator->second.second = 0.0f;
				mStreamsInfo[aIterator->first].mSoundInstance->stopStream();	// dmbreaker: fixes bug with "ghost-tracks" in fade
				// remove the fading stream from the map
				mFadingStreams.erase(aIterator);
				break;
			}
			
			if (aFadeEnded == 1)
			{
				// fade in completed, max volume reached, clear fade speed
				aIterator->second.second = 0.0f;
				// remove the fading stream from the map
				mFadingStreams.erase(aIterator); 
				break;
			}
		}

		// avoid incrementing an end() iterator (JPOAG)
		++aIterator;
	}
}

/*!***********************************
// @return    	void
// @param     	theSongId
// @param     	theOffset No longer supported
// @param     	theSpeed
// @param     	noLoop
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::MusicManager::FadeInStream( int theSongId, int theOffset /*= -1*/, double theSpeed /*= 0.002*/, bool noLoop /*= false*/ )
{
	PlayStream(theSongId, !noLoop, (float)theSpeed);
}

/*!***********************************
// @return    	void
// @param     	stopSong
// @param     	theSpeed
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::MusicManager::FadeOutAll( bool stopSong /*= true*/, double theSpeed /*= 0.004*/ )
{
	for(int i = 0; i < MAX_MUSIC_STREAMS; i++)
	{
		if(mStreamsInfo[i].mSoundInstance != NULL && mStreamsInfo[i].mSoundInstance->isPlaying())
		{
			FadeOutStream(i, (float)theSpeed);
		}
	}
}

/*!***********************************
// @return    	KSound*
// @param     	theStreamID
//
// \brief		REturns the KSound Instance associated with the Stream Id.
// \details 	<b>Description:</b> REturns the KSound Instance associated with the Stream Id.
//************************************/
KSound* Sexy::MusicManager::GetSoundInstance( int theStreamID )
{
	// Make sure that the mStreamsInfo container is thread-safe.
	AutoCrit anAutoLock(mMusicManagerCritSect);

	if ((theStreamID < 0) || (theStreamID >= MAX_MUSIC_STREAMS))
	{
		K_LOG("Sexy::MusicManager::GetSoundInstance: WARNING : Invalid stream ID.");
		return NULL;
	}

	if (mStreamsInfo[theStreamID].mSoundInstance != NULL)
		return mStreamsInfo[theStreamID].mSoundInstance;

	return NULL;
}