// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "KSysDefs.h"

#ifndef K_IPHONE
#include "SexyMovie.h"
#include "KVideo.h"

#include "KPTK.h"
#include "Common.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;


/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyMovie::SexyMovie()
{
	mMovie = new KVideo();
	bStopped = false;
}

/*!***********************************
// @return    	
// @param     	theMovie
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyMovie::SexyMovie( const SexyMovie& theMovie )
{
	*this = theMovie;
	this->mMovie = new KVideo(); // We want our own KVideo
	mKGraphicImage = KPTK::createKGraphic(); // don't delete the other guy's graphic.

	if (theMovie.mMovie != NULL && mFilePath != _S(""))
	{
		if (!mMovie->openVideo(SexyStringToStringFast(mFilePath).c_str()))
		{
			K_LOG("Sexy::SexyMovie(SexyMovie&): Error opening Movie File: %s", SexyStringToStringFast(mFilePath).c_str());
			mFilePath = _S("");
		}
		else
			bStopped = false;
	}
}
/*!***********************************
// @return    	
// @param     	theFileName
// @param     	bPreload
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyMovie::SexyMovie( std::wstring theFileName, bool bPreload )
{
	mMovie = new KVideo();
	bStopped = false;

	if(!mMovie->openVideo(WStringToString(theFileName).c_str(), bPreload))
		K_LOG("Sexy::SexyMovie: Error opening Movie File: %s", WStringToString(theFileName).c_str());
	else
		mFilePath = WStringToSexyStringFast(theFileName);
}

/*!***********************************
// @return    	
// @param     	theFileName
// @param     	bPreload
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyMovie::SexyMovie( std::string theFileName, bool bPreload /*= false*/ )
{
	mMovie = new KVideo();
	bStopped = false;

	if(!mMovie->openVideo((theFileName).c_str(), bPreload))
		K_LOG("Sexy::SexyMovie: Error opening Movie File: %s", (theFileName).c_str());
	else
		mFilePath = StringToSexyStringFast(theFileName);
}


/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyMovie::~SexyMovie()
{
	if(mMovie)
		mMovie->closeVideo();

	delete mMovie;
}

/*!***********************************
// @return    	KGraphic*
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
KGraphic* Sexy::SexyMovie::GetKanjiImage()
{
	if(mMovie && mMovie->isVideoAvailable())
		return mMovie->getKGraphic();

	return NULL;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyMovie::Update()
{
	if(bStopped) return;
		
	mMovie->readFrame();

	bStopped = mMovie->isEndReached();
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	bPreload
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyMovie::OpenFile( std::wstring theFileName, bool bPreload /*= false*/ )
{
	if(!mMovie->openVideo(WStringToString(theFileName).c_str(), bPreload))
	{
		K_LOG("Sexy::SexyMovie::OpenFile: Error opening Movie File: %s", WStringToString(theFileName).c_str());
		return false;
	}
	else 
		mFilePath = WStringToSexyStringFast(theFileName);

	return true;
}
/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	bPreload
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyMovie::OpenFile( std::string theFileName, bool bPreload /*= false*/ )
{
	if(!mMovie->openVideo((theFileName).c_str(), bPreload))
	{
		K_LOG("Sexy::SexyMovie::OpenFile: Error opening Movie File: %s", (theFileName).c_str());
		return false;
	}
	else 
		mFilePath = StringToSexyStringFast(theFileName);

	return true;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::SexyMovie::GetWidth() const
{
	if(mMovie)
		return (int)mMovie->getKGraphic()->getWidth();

	return mWidth;
}
/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::SexyMovie::GetHeight() const
{
	if(mMovie)
		return (int)mMovie->getKGraphic()->getHeight();

	return mHeight;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyMovie::Restart()
{
	if(mMovie)
		mMovie->rewind();
	bStopped = false;
}
#endif