// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#pragma once

#include "Common.h"
#include "SharedFont.h"
#include "AutoCrit.h"

namespace Sexy
{
	/*!***********************************
	// \class		FontManager
	//
	// \brief		A utility for loading Fonts and for managing Shared Fonts.
	// \details 	<b>Description:</b> Increase program speed and reduce memory consumption by allowing the 
	//				program to share Fonts instead of duplicating them.
	//************************************/
	class FontManager
	{
	private:
		SharedFontMap							mSharedFontMap;
		KSysLock								mFontManagerCritSect;
	public:
		FontManager(void);
		~FontManager(void);

		SexyFont*								GetFont(const std::wstring& theFileName);
		SexyFont*								GetFont(const std::string& theFileName);
		SharedFontRef							GetSharedFont(const std::wstring& theFileName, const std::wstring& theVariant = L"", bool* isNew = NULL);
		SharedFontRef							GetSharedFont(const std::string& theFileName, const std::string& theVariant = "", bool* isNew = NULL);
		void									CleanSharedFonts(bool forceDeleteAll = false);

	};
};


