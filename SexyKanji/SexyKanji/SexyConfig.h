// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SEXY_CONFIG_H__
#define __SEXY_CONFIG_H__

#include "Common.h"

const SexyString	DEFAULT_COMPANY				= _S("SexyKanji");
const SexyString	DEFAULT_TITLE				= _S("SexyKanji App");
const SexyString	DEFAULT_VERSION				= _S("1.0"); 
const int			DEFAULT_WIDTH				= 800;			// [0, #INF)
const int			DEFAULT_HEIGHT				= 600;			// [0, #INF)
const int			DEFAULT_BITS				= 32;			// { -1, 16, 32}
const bool			DEFAULT_WINDOWED			= true;			// {true, false}
const bool			DEFAULT_VSYNC				= true;			// {true, false}

const int			DEFAULT_MUSIC_VOLUME		= 85;			// [0..100]
const int			DEFAULT_SOUND_VOLUME		= 85;			// [0..100]
const int			MAX_SOUND_SAMPLES			= 256;			// [0..0xFFFFFFFF]
const int			MAX_MUSIC_STREAMS			= 256;			// [0..0xFFFFFFFF]

const SexyString	DEFAULT_PROPERTIES_PATH		= _S("properties/default.xml");
const SexyString	DEFAULT_SETTINGS_PATH		= _S("properties/settings.xml");

const SexyString	DEFAULT_RESOURCE_PATH		= _S("properties/resources.xml");

#endif // __SEXY_CONFIG_H__
