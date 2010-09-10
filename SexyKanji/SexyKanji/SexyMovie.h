// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SEXYMOVIE_H__
#define __SEXYMOVIE_H__

#include "SexyImage.h"

class KVideo;

namespace Sexy
{
	class SexyMovie : public SexyImage
	{
	protected:
		KVideo*							mMovie;
		bool							bStopped;
	public:
		SexyMovie();
		virtual ~SexyMovie();
		SexyMovie(const SexyMovie& theMovie);
		SexyMovie(std::wstring theFileName, bool bPreload = false);
		SexyMovie(std::string theFileName, bool bPreload = false);


		//////////////////////////////////////////////////////////////////////////
		// Sexy Image Overrides
		// This is so the Movie can be treated like a SexyImage!
		//////////////////////////////////////////////////////////////////////////
		virtual KGraphic*				GetKanjiImage();
		virtual bool					OpenFile(std::wstring theFileName, bool bPreload = false);
		virtual bool					OpenFile(std::string theFileName, bool bPreload = false);

		virtual int						GetWidth() const;
		virtual int						GetHeight() const;
		virtual int						GetRows()const				{return 1;}
		virtual int						GetColumns() const			{return 1;}

		virtual unsigned long*			GetBits(){return NULL;}
		virtual void					SetBits(unsigned long* theBits, int theWidth, int theHeight){}
		virtual void					PurgeBits(bool bPurgeBits = true){}
		virtual void					CommitBits() {}

		virtual void					SetRows(int theNumRows)		{mNumRows = 1;}
		virtual void					SetColumns(int theNumCols)	{mNumCols = 1;}

		//////////////////////////////////////////////////////////////////////////
		// Sexy Movie Specific Methods
		//////////////////////////////////////////////////////////////////////////

		virtual void					Update();
		virtual bool					IsStopped() const { return bStopped; }
		virtual void					Stop() { bStopped = true; }
		virtual void					Restart();
	};
}

#endif // __SEXYMOVIE_H__