// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji MusicManager Version 0.5
	-------------------------------------------------------------	
	Created:	2010/02/25
	Filename: 	SexyKanji\MusicManager.h
	
	Purpose:	SexyAppFramework Wrapper class for music streams
				play streams, fading

				Heiko Fischer - heiko@fischeronline.de
*********************************************************************/

#ifndef __MUSIC_MANAGER_H__
#define __MUSIC_MANAGER_H__

#include "MusicSoundBase.h"

#include <string>
#include <map>

#include "SexyConfig.h"
#include "AutoCrit.h"

namespace Sexy
{
	class MusicManager : public MusicSoundBase
	{
	public:
		MusicManager();
		~MusicManager();

		//----------------------------------------------------------------------------
		// load stream with a specified ID and filename
		// any previously loaded stream with this ID will be released! 
	    //----------------------------------------------------------------------------
		bool			LoadStream(unsigned int theStreamID, SexyString theFileName);
		
		//----------------------------------------------------------------------------
		// load a stream with the specified filename, the StreamID will be auto assigned
		// if theForceLoad == true -> force to load the stream even if it's already loaded
		//	(use this option carefully, streams can consume a lot of memory!)
	    //----------------------------------------------------------------------------
		int				AutoLoadStream(SexyString theFileName, bool theForceLoad = false);
		
		//----------------------------------------------------------------------------
		// release all previous loaded streams - all loaded streams are 
		// auto-released in the destructor		
	    //----------------------------------------------------------------------------
		void			ReleaseStreams();
		
		//----------------------------------------------------------------------------
		// release a loaded stream with the given StreamID
	    //----------------------------------------------------------------------------
		void			ReleaseStream(unsigned int theStreamID);

		//----------------------------------------------------------------------------
		// immediately play a stream with the given ID
	    //----------------------------------------------------------------------------
		void			PlayStream(unsigned int theStreamID);

		//----------------------------------------------------------------------------
		// immediately play a stream with the given ID optionally loop the stream
	    //----------------------------------------------------------------------------
		void			PlayStream(unsigned int theStreamID, bool bLoop);

		//----------------------------------------------------------------------------
		// immediately play a stream with the given ID
		// optionally fade the stream in and loop
	    //----------------------------------------------------------------------------
		void			PlayStream(unsigned int theStreamID, bool bLoop, float theFadeInSpeed = 0.0f);

		//----------------------------------------------------------------------------
		// set or increment the volume of the given stream: 0 = silent, 100 = max volume
	    //----------------------------------------------------------------------------
		virtual void	SetStreamVolume(unsigned int theStreamID, int theVolume);
		virtual void	IncStreamVolume(unsigned int theStreamID, int theVolumeInc);

	    //----------------------------------------------------------------------------
		// get the current volume
	    //----------------------------------------------------------------------------
		virtual int		GetCurrentStreamVolume(unsigned int theStreamID);

		//----------------------------------------------------------------------------
		// stops playing the given stream 
	    //----------------------------------------------------------------------------
		virtual void	StopStream(unsigned int theStreamID);

	    //----------------------------------------------------------------------------
		// pause or resume playing the stream with the given ID
		// thePause: true to pause, false to resume
	    //----------------------------------------------------------------------------
		virtual void	PauseStream(unsigned int theStreamID, bool thePause);

	    //----------------------------------------------------------------------------
		// check if this stream is currently playing, returns true if playing, false if not
	    //----------------------------------------------------------------------------
		virtual bool	IsStreamPlaying(unsigned int theStreamID);

		//----------------------------------------------------------------------------
		// fades out a playing stream
	    //----------------------------------------------------------------------------
		virtual void	FadeOutStream(unsigned int theStreamID, float theFadeOutSpeed = 0.004);

		//----------------------------------------------------------------------------
		// fades in a stream
	    //----------------------------------------------------------------------------
		virtual void	FadeInStream(int theSongId, int theOffset = -1, double theSpeed = 0.002, bool noLoop = false);

		//----------------------------------------------------------------------------
		// fades out all streams
		//----------------------------------------------------------------------------
		virtual void	FadeOutAll(bool stopSong = true, double theSpeed = 0.004);


		//----------------------------------------------------------------------------
		// Get a currently free stream ID
		//----------------------------------------------------------------------------
		virtual unsigned int GetFreeStreamID();

		//----------------------------------------------------------------------------
		// Restrieve the Sound Instance from the ID
		//----------------------------------------------------------------------------
		virtual KSound*	GetSoundInstance(int theStreamID);

		//----------------------------------------------------------------------------
		// Reserve a stream ID detected by GetFreeStreamID() 
		//----------------------------------------------------------------------------
		virtual void ReserveStreamID(unsigned int theStreamID);

		//----------------------------------------------------------------------------
		// Free a stream ID reserved by ReserveStreamID()
		//----------------------------------------------------------------------------
		virtual void FreeReservedStreamID(unsigned int theStreamID);

	private:
		// SexyAppBase is a friend, so the Update() method can kept private here
		friend class SexyAppBase;

		// called from SexyAppBase::DoUpdateFrames()
		virtual void Update();

		// alias name
		typedef std::map<unsigned int, std::pair<KSound*, float> > FadingStreamMap;  

		SampleStreamInfo			mStreamsInfo[MAX_MUSIC_STREAMS];// informations about all the loaded streams
		FadingStreamMap				mFadingStreams;					// a list of streams that are fading -> <SampleID, <KSound*, FadeSpeed>>
		std::set<unsigned int>		mReservedStreamIDs;				// set with reserved stream IDs

		KSysLock					mMusicManagerCritSect;
	};
}

#endif // __MUSIC_MANAGER_H__
