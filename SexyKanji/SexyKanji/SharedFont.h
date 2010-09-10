// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	created:	2010/04/24
	created:	24:4:2010   8:20
	filename: 	SexyKanji\SharedFont.h
	author:		Andrei Lazutin
	
	purpose:	Provide the reference counting for SexyFont to be used
				in the FontManager.
*********************************************************************/
#ifndef __SHARED_FONT_H__
#define __SHARED_FONT_H__
#pragma once

#include "SexyFont.h"
#include "SharedItem.h"

namespace Sexy
{
    typedef SharedItem<Sexy::SexyFont> SharedFont;
    typedef SharedItemRef<Sexy::SexyFont> SharedFontRef;
    typedef std::map<std::pair<SexyString, SexyString>, SharedFont> SharedFontMap;
};

#endif