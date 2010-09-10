// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji MusicSoundBase Version 0.4
	-------------------------------------------------------------	
	Created:	2010/02/25
	Filename: 	SexyKanji\MusicSoundBase.cpp
	
	Purpose:	SexyAppFramework Wrapper interface for music and sound

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#include "SexyAppBase.h"

#include "MusicSoundBase.h"

#include "KPTK.h"

#include "SexyConfig.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

// init static member
int MusicSoundBase::mGlobalMusicVolume = DEFAULT_MUSIC_VOLUME;
int MusicSoundBase::mGlobalSoundVolume = DEFAULT_SOUND_VOLUME;
bool MusicSoundBase::mSuspended = false;

/*!***********************************
// @return    	
//
// \brief		Constructor, member initialization
// \details 	<b>Description:</b> Constructor, member initialization
//************************************/
MusicSoundBase::MusicSoundBase()
{
	// create the sound instance 
	mSoundInstance = new KSound();
}

/*!***********************************
// @return    	
//
// \brief		Destructor, delete member
// \details 	<b>Description:</b> Destructor, delete member
//************************************/
MusicSoundBase::~MusicSoundBase()
{
	delete mSoundInstance;
}

/*!***********************************
// @return    	void
// @param     	theGlobalMusicVolume value for the global music volume
//
// \brief		set the global music volume
// \details 	<b>Description:</b> set the global music volume
//************************************/
void MusicSoundBase::SetGlobalMusicVolume(int theGlobalMusicVolume)
{
	if (theGlobalMusicVolume >= MIN_VOLUME && theGlobalMusicVolume <= MAX_VOLUME)
	{
		mSoundInstance->setGlobalVolumes(theGlobalMusicVolume, mGlobalSoundVolume);
		mGlobalMusicVolume = theGlobalMusicVolume;
		gSexyAppBase->mMusicVolume = theGlobalMusicVolume;
	}
	else
	{
		K_LOG("Sexy::MusicSoundBase::SetGlobalMusicVolume: WARNING : Invalid music volume.");
	}
}

/*!***********************************
// @return    	void
// @param     	theGlobalSoundVolume value for the global sound volume
//
// \brief		set the global sound volume
// \details 	<b>Description:</b> set the global sound volume
//************************************/
void MusicSoundBase::SetGlobalSoundVolume(int theGlobalSoundVolume)
{
	if (theGlobalSoundVolume >= MIN_VOLUME && theGlobalSoundVolume <= MAX_VOLUME)
	{
		mSoundInstance->setGlobalVolumes(mGlobalMusicVolume, theGlobalSoundVolume);
		mGlobalSoundVolume = theGlobalSoundVolume;
		gSexyAppBase->mSfxVolume = theGlobalSoundVolume;
	}
	else
	{
		K_LOG("Sexy::MusicSoundBase::SetGlobalSoundVolume: WARNING : Invalid sound volume.");
	}
}

/*!***********************************
// @return    	void
//
// \brief		suspend all sound activity, for instance after losing focus
// \details 	<b>Description:</b> suspend all sound activity, for instance after losing focus
//************************************/
void MusicSoundBase::SuspendAll()
{
	mSuspended = true;
	mSoundInstance->suspend();
}

/*!***********************************
// @return    	void
//
// \brief		resume sound activity, for instance after regaining focus 
// \details 	<b>Description:</b> resume sound activity, for instance after regaining focus 
//************************************/
void MusicSoundBase::ResumeAll()
{
	mSuspended = false;
	mSoundInstance->resume();
}

/*!***********************************
// @return    	long
// @param     	theFileName to detect image format from
//
// \brief		detect sound effect format
// \details 	<b>Description:</b> detect sound effect format from the supplied 
//				filename's extension (.wav, .ogg) {SAMPLE_WAV, SAMPLE_OGG}
//************************************/
long Sexy::MusicSoundBase::DetectFormat(const SexyString& theFileName)
{
	return mSoundInstance->detectFormat(SexyStringToStringFast(theFileName).c_str());
}

/*!***********************************
// @return    	void
//
// \brief		free all lingering resources when not using audio anymore. 
// \details 	<b>Description:</b> Free all lingering resources when not using audio anymore.
//				This method may be called by the game just before exiting,
//				to check for memory leaks, otherwise it is not necessary.
//************************************/
void MusicSoundBase::CleanupAll()
{
	mSoundInstance->cleanup();
}
