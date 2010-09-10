/********************************************************************
	Generated by Sexy Kanji Resource Generator (JPoag/HFischer)
	-----------------------------------------------------------
	Created:	04.04.2010 14:15:50
	Filename:	Res.cpp

	Purpose:	Functions and globals, based on the resource.xml file,
				to easily use the resources in your program.
*********************************************************************/

#include "Res.h"
#include "ResourceManager.h"

using namespace Sexy;

#pragma warning(disable:4311 4312)

static bool gNeedRecalcVariableToIdMap = false;

bool Sexy::ExtractResourcesByName(ResourceManager* theManager, SexyString theName)
{
	if (sexystrcmp(theName.c_str(), _S("Game")) == 0) return ExtractGameResources(theManager);
	if (sexystrcmp(theName.c_str(), _S("Hungarr")) == 0) return ExtractHungarrResources(theManager);
	if (sexystrcmp(theName.c_str(), _S("Init")) == 0) return ExtractInitResources(theManager);
	if (sexystrcmp(theName.c_str(), _S("TitleScreen")) == 0) return ExtractTitleScreenResources(theManager);

	return false;
}

Sexy::ResourceId Sexy::GetIdByStringId(SexyString theStringId)
{
	typedef std::map<SexyString, int> MyMap;

	static MyMap aMap;

	if (aMap.empty())
	{
		for (int i=0; i < RESOURCE_ID_MAX; i++)
			aMap[GetStringIdById(i)] = i;
	}

	MyMap::iterator anItr = aMap.find(theStringId);
	if (anItr == aMap.end())
		return RESOURCE_ID_MAX;
	else
		return (ResourceId) anItr->second;
}

// Game Resources

SexyImage* Sexy::IMAGE_BG0;
SexyImage* Sexy::IMAGE_BG1;
SexyImage* Sexy::IMAGE_BG2;
SexyImage* Sexy::IMAGE_BUTTON_DOWN;
SexyImage* Sexy::IMAGE_BUTTON_NORMAL;
SexyImage* Sexy::IMAGE_BUTTON_OVER;
SexyImage* Sexy::IMAGE_CHECKBOX;
SexyImage* Sexy::IMAGE_DIALOG_BOX;
SexyImage* Sexy::IMAGE_DIALOG_BUTTON;
SexyImage* Sexy::IMAGE_ROBOTROBOT;
SexyImage* Sexy::IMAGE_SLIDER_THUMB;
SexyImage* Sexy::IMAGE_SLIDER_TRACK;
int Sexy::SOUND_MUTATOR;
int Sexy::SOUND_TIMER;

bool Sexy::ExtractGameResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		IMAGE_BG0 = aMgr.GetImageThrow(_S("IMAGE_BG0"));
		IMAGE_BG1 = aMgr.GetImageThrow(_S("IMAGE_BG1"));
		IMAGE_BG2 = aMgr.GetImageThrow(_S("IMAGE_BG2"));
		IMAGE_BUTTON_DOWN = aMgr.GetImageThrow(_S("IMAGE_BUTTON_DOWN"));
		IMAGE_BUTTON_NORMAL = aMgr.GetImageThrow(_S("IMAGE_BUTTON_NORMAL"));
		IMAGE_BUTTON_OVER = aMgr.GetImageThrow(_S("IMAGE_BUTTON_OVER"));
		IMAGE_CHECKBOX = aMgr.GetImageThrow(_S("IMAGE_CHECKBOX"));
		IMAGE_DIALOG_BOX = aMgr.GetImageThrow(_S("IMAGE_DIALOG_BOX"));
		IMAGE_DIALOG_BUTTON = aMgr.GetImageThrow(_S("IMAGE_DIALOG_BUTTON"));
		IMAGE_ROBOTROBOT = aMgr.GetImageThrow(_S("IMAGE_ROBOTROBOT"));
		IMAGE_SLIDER_THUMB = aMgr.GetImageThrow(_S("IMAGE_SLIDER_THUMB"));
		IMAGE_SLIDER_TRACK = aMgr.GetImageThrow(_S("IMAGE_SLIDER_TRACK"));
		SOUND_MUTATOR = aMgr.GetSoundThrow(_S("SOUND_MUTATOR"));
		SOUND_TIMER = aMgr.GetSoundThrow(_S("SOUND_TIMER"));
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// Hungarr Resources

SexyImage* Sexy::IMAGE_ATOMIC_EXPLOSION;
SexyImage* Sexy::IMAGE_BOMB_RADIAL_DEATH;
SexyImage* Sexy::IMAGE_HUNGARR_BEAM_DOWN;
SexyImage* Sexy::IMAGE_HUNGARR_BEAM_LEFT;
SexyImage* Sexy::IMAGE_HUNGARR_BEAM_RIGHT;
SexyImage* Sexy::IMAGE_HUNGARR_BEAM_UP;
SexyImage* Sexy::IMAGE_HUNGARR_HORIZ;
SexyImage* Sexy::IMAGE_HUNGARR_SMALL;
SexyImage* Sexy::IMAGE_HUNGARR_VERT;
SexyImage* Sexy::IMAGE_PARTICLE_LIGHTNING;
SexyImage* Sexy::IMAGE_PLANETS;
SexyImage* Sexy::IMAGE_SPARK;
int Sexy::SOUND_BEAM_HIT;
int Sexy::SOUND_BEAM_MOVING;
int Sexy::SOUND_BUTTON;
int Sexy::SOUND_EXPLOSION;
int Sexy::SOUND_GAME_OVER_CLICK;
int Sexy::SOUND_GAME_OVER_RESTART;
int Sexy::SOUND_GAME_OVER_STATS;
int Sexy::SOUND_GAME_OVER_TEXT;
int Sexy::SOUND_LEVEL_UP1;
int Sexy::SOUND_LEVEL_UP2;
int Sexy::SOUND_LEVEL_UP3;
int Sexy::SOUND_LEVEL_UP4;
int Sexy::SOUND_MAGZAP;
int Sexy::SOUND_PLANET;
int Sexy::SOUND_PLANET_HIT;
int Sexy::SOUND_REGION_FILLED;

bool Sexy::ExtractHungarrResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		IMAGE_ATOMIC_EXPLOSION = aMgr.GetImageThrow(_S("IMAGE_ATOMIC_EXPLOSION"));
		IMAGE_BOMB_RADIAL_DEATH = aMgr.GetImageThrow(_S("IMAGE_BOMB_RADIAL_DEATH"));
		IMAGE_HUNGARR_BEAM_DOWN = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_BEAM_DOWN"));
		IMAGE_HUNGARR_BEAM_LEFT = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_BEAM_LEFT"));
		IMAGE_HUNGARR_BEAM_RIGHT = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_BEAM_RIGHT"));
		IMAGE_HUNGARR_BEAM_UP = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_BEAM_UP"));
		IMAGE_HUNGARR_HORIZ = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_HORIZ"));
		IMAGE_HUNGARR_SMALL = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_SMALL"));
		IMAGE_HUNGARR_VERT = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_VERT"));
		IMAGE_PARTICLE_LIGHTNING = aMgr.GetImageThrow(_S("IMAGE_PARTICLE_LIGHTNING"));
		IMAGE_PLANETS = aMgr.GetImageThrow(_S("IMAGE_PLANETS"));
		IMAGE_SPARK = aMgr.GetImageThrow(_S("IMAGE_SPARK"));
		SOUND_BEAM_HIT = aMgr.GetSoundThrow(_S("SOUND_BEAM_HIT"));
		SOUND_BEAM_MOVING = aMgr.GetSoundThrow(_S("SOUND_BEAM_MOVING"));
		SOUND_BUTTON = aMgr.GetSoundThrow(_S("SOUND_BUTTON"));
		SOUND_EXPLOSION = aMgr.GetSoundThrow(_S("SOUND_EXPLOSION"));
		SOUND_GAME_OVER_CLICK = aMgr.GetSoundThrow(_S("SOUND_GAME_OVER_CLICK"));
		SOUND_GAME_OVER_RESTART = aMgr.GetSoundThrow(_S("SOUND_GAME_OVER_RESTART"));
		SOUND_GAME_OVER_STATS = aMgr.GetSoundThrow(_S("SOUND_GAME_OVER_STATS"));
		SOUND_GAME_OVER_TEXT = aMgr.GetSoundThrow(_S("SOUND_GAME_OVER_TEXT"));
		SOUND_LEVEL_UP1 = aMgr.GetSoundThrow(_S("SOUND_LEVEL_UP1"));
		SOUND_LEVEL_UP2 = aMgr.GetSoundThrow(_S("SOUND_LEVEL_UP2"));
		SOUND_LEVEL_UP3 = aMgr.GetSoundThrow(_S("SOUND_LEVEL_UP3"));
		SOUND_LEVEL_UP4 = aMgr.GetSoundThrow(_S("SOUND_LEVEL_UP4"));
		SOUND_MAGZAP = aMgr.GetSoundThrow(_S("SOUND_MAGZAP"));
		SOUND_PLANET = aMgr.GetSoundThrow(_S("SOUND_PLANET"));
		SOUND_PLANET_HIT = aMgr.GetSoundThrow(_S("SOUND_PLANET_HIT"));
		SOUND_REGION_FILLED = aMgr.GetSoundThrow(_S("SOUND_REGION_FILLED"));
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// Init Resources

SexyFont* Sexy::FONT_DEFAULT;
SexyFont* Sexy::FONT_HUNGARR;
SexyImage* Sexy::IMAGE_CUSTOM_DRAGGING;
SexyImage* Sexy::IMAGE_CUSTOM_HAND;
SexyImage* Sexy::IMAGE_CUSTOM_POINTER;
SexyImage* Sexy::IMAGE_CUSTOM_TEXT;
SexyImage* Sexy::IMAGE_HUNGARR_LOGO;
SexyImage* Sexy::IMAGE_KANJI_LOGO;

bool Sexy::ExtractInitResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		FONT_DEFAULT = aMgr.GetFontThrow(_S("FONT_DEFAULT"));
		FONT_HUNGARR = aMgr.GetFontThrow(_S("FONT_HUNGARR"));
		IMAGE_CUSTOM_DRAGGING = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_DRAGGING"));
		IMAGE_CUSTOM_HAND = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_HAND"));
		IMAGE_CUSTOM_POINTER = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_POINTER"));
		IMAGE_CUSTOM_TEXT = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_TEXT"));
		IMAGE_HUNGARR_LOGO = aMgr.GetImageThrow(_S("IMAGE_HUNGARR_LOGO"));
		IMAGE_KANJI_LOGO = aMgr.GetImageThrow(_S("IMAGE_KANJI_LOGO"));
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// TitleScreen Resources

SexyImage* Sexy::IMAGE_LOADER_BAR;
SexyImage* Sexy::IMAGE_LOADER_LOADINGTXT;
int Sexy::SOUND_CONTINUE;

bool Sexy::ExtractTitleScreenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		IMAGE_LOADER_BAR = aMgr.GetImageThrow(_S("IMAGE_LOADER_BAR"));
		IMAGE_LOADER_LOADINGTXT = aMgr.GetImageThrow(_S("IMAGE_LOADER_LOADINGTXT"));
		SOUND_CONTINUE = aMgr.GetSoundThrow(_S("SOUND_CONTINUE"));
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

static void* gResources[] =
{
	&FONT_DEFAULT,
	&FONT_HUNGARR,
	&IMAGE_CUSTOM_POINTER,
	&IMAGE_CUSTOM_HAND,
	&IMAGE_CUSTOM_DRAGGING,
	&IMAGE_CUSTOM_TEXT,
	&IMAGE_HUNGARR_LOGO,
	&IMAGE_KANJI_LOGO,
	&IMAGE_LOADER_BAR,
	&IMAGE_LOADER_LOADINGTXT,
	&SOUND_CONTINUE,
	&SOUND_MUTATOR,
	&SOUND_TIMER,
	&IMAGE_ROBOTROBOT,
	&IMAGE_CHECKBOX,
	&IMAGE_BG0,
	&IMAGE_BG1,
	&IMAGE_BG2,
	&IMAGE_BUTTON_DOWN,
	&IMAGE_BUTTON_OVER,
	&IMAGE_BUTTON_NORMAL,
	&IMAGE_DIALOG_BOX,
	&IMAGE_DIALOG_BUTTON,
	&IMAGE_SLIDER_TRACK,
	&IMAGE_SLIDER_THUMB,
	&IMAGE_HUNGARR_SMALL,
	&IMAGE_HUNGARR_BEAM_UP,
	&IMAGE_HUNGARR_BEAM_DOWN,
	&IMAGE_HUNGARR_BEAM_LEFT,
	&IMAGE_HUNGARR_BEAM_RIGHT,
	&IMAGE_HUNGARR_HORIZ,
	&IMAGE_HUNGARR_VERT,
	&IMAGE_ATOMIC_EXPLOSION,
	&IMAGE_BOMB_RADIAL_DEATH,
	&IMAGE_PLANETS,
	&IMAGE_SPARK,
	&IMAGE_PARTICLE_LIGHTNING,
	&SOUND_MAGZAP,
	&SOUND_BUTTON,
	&SOUND_PLANET,
	&SOUND_LEVEL_UP1,
	&SOUND_LEVEL_UP2,
	&SOUND_EXPLOSION,
	&SOUND_BEAM_HIT,
	&SOUND_PLANET_HIT,
	&SOUND_BEAM_MOVING,
	&SOUND_LEVEL_UP4,
	&SOUND_LEVEL_UP3,
	&SOUND_GAME_OVER_CLICK,
	&SOUND_GAME_OVER_STATS,
	&SOUND_GAME_OVER_RESTART,
	&SOUND_GAME_OVER_TEXT,
	&SOUND_REGION_FILLED,
	NULL
};

SexyImage* Sexy::LoadImageById(ResourceManager* theManager, int theId)
{
	return (*((SexyImage**)gResources[theId]) = theManager->LoadImage(GetStringIdById(theId)));
}

void Sexy::ReplaceImageById(ResourceManager* theManager, int theId, SexyImage* theImage)
{
	theManager->ReplaceImage(GetStringIdById(theId), theImage);
	*(SexyImage**)gResources[theId] = theImage;
}

SexyImage* Sexy::GetImageById(int theId)
{
	return *(SexyImage**)gResources[theId];
}

SexyFont* Sexy::GetFontById(int theId)
{
	return *(SexyFont**)gResources[theId];
}

int Sexy::GetSoundById(int theId)
{
	return *(int*)gResources[theId];
}

int Sexy::GetMusicById(int theId)
{
	return *(int*)gResources[theId];
}

SexyMovie* Sexy::GetMovieById(int theId)
{
	return *(SexyMovie**)gResources[theId];
}

SexyImage*& Sexy::GetImageRefById(int theId)
{
	return *(SexyImage**)gResources[theId];
}

SexyFont*& Sexy::GetFontRefById(int theId)
{
	return *(SexyFont**)gResources[theId];
}

int& Sexy::GetSoundRefById(int theId)
{
	return *(int*)gResources[theId];
}

int& Sexy::GetMusicRefById(int theId)
{
	return *(int*)gResources[theId];
}

SexyMovie*& Sexy::GetMovieRefById(int theId)
{
	return *(SexyMovie**)gResources[theId];
}

static Sexy::ResourceId GetIdByVariable(const void* theVariable)
{
	typedef std::map<int, int> MyMap;

	static MyMap aMap;

	if (gNeedRecalcVariableToIdMap)
	{
		gNeedRecalcVariableToIdMap = false;
		aMap.clear();
		for (int i=0; i < RESOURCE_ID_MAX; i++)
			aMap[*(int*)gResources[i]] = i;
	}

	MyMap::iterator anItr = aMap.find((int)theVariable);
	if (anItr == aMap.end())
		return RESOURCE_ID_MAX;
	else
		return (ResourceId)anItr->second;
}

Sexy::ResourceId Sexy::GetIdByImage(SexyImage* theImage)
{
	return GetIdByVariable(theImage);
}

Sexy::ResourceId Sexy::GetIdByFont(SexyFont* theFont)
{
	return GetIdByVariable(theFont);
}

Sexy::ResourceId Sexy::GetIdBySound(int theSound)
{
	return GetIdByVariable((void*)theSound);
}

Sexy::ResourceId Sexy::GetIdByMusic(int theMusic)
{
	return GetIdByVariable((void*)theMusic);
}

Sexy::ResourceId Sexy::GetIdByMovie(SexyMovie* theMovie)
{
	return GetIdByVariable(theMovie);
}

SexyString Sexy::GetStringIdById(int theId)
{
	switch (theId)
	{
		case FONT_DEFAULT_ID: return _S("FONT_DEFAULT");
		case FONT_HUNGARR_ID: return _S("FONT_HUNGARR");
		case IMAGE_CUSTOM_POINTER_ID: return _S("IMAGE_CUSTOM_POINTER");
		case IMAGE_CUSTOM_HAND_ID: return _S("IMAGE_CUSTOM_HAND");
		case IMAGE_CUSTOM_DRAGGING_ID: return _S("IMAGE_CUSTOM_DRAGGING");
		case IMAGE_CUSTOM_TEXT_ID: return _S("IMAGE_CUSTOM_TEXT");
		case IMAGE_HUNGARR_LOGO_ID: return _S("IMAGE_HUNGARR_LOGO");
		case IMAGE_KANJI_LOGO_ID: return _S("IMAGE_KANJI_LOGO");
		case IMAGE_LOADER_BAR_ID: return _S("IMAGE_LOADER_BAR");
		case IMAGE_LOADER_LOADINGTXT_ID: return _S("IMAGE_LOADER_LOADINGTXT");
		case SOUND_CONTINUE_ID: return _S("SOUND_CONTINUE");
		case SOUND_MUTATOR_ID: return _S("SOUND_MUTATOR");
		case SOUND_TIMER_ID: return _S("SOUND_TIMER");
		case IMAGE_ROBOTROBOT_ID: return _S("IMAGE_ROBOTROBOT");
		case IMAGE_CHECKBOX_ID: return _S("IMAGE_CHECKBOX");
		case IMAGE_BG0_ID: return _S("IMAGE_BG0");
		case IMAGE_BG1_ID: return _S("IMAGE_BG1");
		case IMAGE_BG2_ID: return _S("IMAGE_BG2");
		case IMAGE_BUTTON_DOWN_ID: return _S("IMAGE_BUTTON_DOWN");
		case IMAGE_BUTTON_OVER_ID: return _S("IMAGE_BUTTON_OVER");
		case IMAGE_BUTTON_NORMAL_ID: return _S("IMAGE_BUTTON_NORMAL");
		case IMAGE_DIALOG_BOX_ID: return _S("IMAGE_DIALOG_BOX");
		case IMAGE_DIALOG_BUTTON_ID: return _S("IMAGE_DIALOG_BUTTON");
		case IMAGE_SLIDER_TRACK_ID: return _S("IMAGE_SLIDER_TRACK");
		case IMAGE_SLIDER_THUMB_ID: return _S("IMAGE_SLIDER_THUMB");
		case IMAGE_HUNGARR_SMALL_ID: return _S("IMAGE_HUNGARR_SMALL");
		case IMAGE_HUNGARR_BEAM_UP_ID: return _S("IMAGE_HUNGARR_BEAM_UP");
		case IMAGE_HUNGARR_BEAM_DOWN_ID: return _S("IMAGE_HUNGARR_BEAM_DOWN");
		case IMAGE_HUNGARR_BEAM_LEFT_ID: return _S("IMAGE_HUNGARR_BEAM_LEFT");
		case IMAGE_HUNGARR_BEAM_RIGHT_ID: return _S("IMAGE_HUNGARR_BEAM_RIGHT");
		case IMAGE_HUNGARR_HORIZ_ID: return _S("IMAGE_HUNGARR_HORIZ");
		case IMAGE_HUNGARR_VERT_ID: return _S("IMAGE_HUNGARR_VERT");
		case IMAGE_ATOMIC_EXPLOSION_ID: return _S("IMAGE_ATOMIC_EXPLOSION");
		case IMAGE_BOMB_RADIAL_DEATH_ID: return _S("IMAGE_BOMB_RADIAL_DEATH");
		case IMAGE_PLANETS_ID: return _S("IMAGE_PLANETS");
		case IMAGE_SPARK_ID: return _S("IMAGE_SPARK");
		case IMAGE_PARTICLE_LIGHTNING_ID: return _S("IMAGE_PARTICLE_LIGHTNING");
		case SOUND_MAGZAP_ID: return _S("SOUND_MAGZAP");
		case SOUND_BUTTON_ID: return _S("SOUND_BUTTON");
		case SOUND_PLANET_ID: return _S("SOUND_PLANET");
		case SOUND_LEVEL_UP1_ID: return _S("SOUND_LEVEL_UP1");
		case SOUND_LEVEL_UP2_ID: return _S("SOUND_LEVEL_UP2");
		case SOUND_EXPLOSION_ID: return _S("SOUND_EXPLOSION");
		case SOUND_BEAM_HIT_ID: return _S("SOUND_BEAM_HIT");
		case SOUND_PLANET_HIT_ID: return _S("SOUND_PLANET_HIT");
		case SOUND_BEAM_MOVING_ID: return _S("SOUND_BEAM_MOVING");
		case SOUND_LEVEL_UP4_ID: return _S("SOUND_LEVEL_UP4");
		case SOUND_LEVEL_UP3_ID: return _S("SOUND_LEVEL_UP3");
		case SOUND_GAME_OVER_CLICK_ID: return _S("SOUND_GAME_OVER_CLICK");
		case SOUND_GAME_OVER_STATS_ID: return _S("SOUND_GAME_OVER_STATS");
		case SOUND_GAME_OVER_RESTART_ID: return _S("SOUND_GAME_OVER_RESTART");
		case SOUND_GAME_OVER_TEXT_ID: return _S("SOUND_GAME_OVER_TEXT");
		case SOUND_REGION_FILLED_ID: return _S("SOUND_REGION_FILLED");
		default: return _S("");
	}
}

