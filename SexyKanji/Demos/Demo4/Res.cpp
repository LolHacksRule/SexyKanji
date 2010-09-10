/********************************************************************
	Generated by Sexy Kanji Resource Generator (JPoag/HFischer)
	-----------------------------------------------------------
	Created:	26.04.2010 10:14:30
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

SexyImage* Sexy::IMAGE_CHECKBOX;
SexyImage* Sexy::IMAGE_ROBOTROBOT;

bool Sexy::ExtractGameResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		IMAGE_CHECKBOX = aMgr.GetImageThrow(_S("IMAGE_CHECKBOX"));
		IMAGE_ROBOTROBOT = aMgr.GetImageThrow(_S("IMAGE_ROBOTROBOT"));
	}
	catch (ResourceManagerException&)
	{
		return false;
	}
	return true;
}

// Init Resources

SexyFont* Sexy::FONT_DEFAULT;
SexyImage* Sexy::IMAGE_CUSTOM_DRAGGING;
SexyImage* Sexy::IMAGE_CUSTOM_HAND;
SexyImage* Sexy::IMAGE_CUSTOM_POINTER;
SexyImage* Sexy::IMAGE_CUSTOM_TEXT;
SexyImage* Sexy::IMAGE_KANJI_LOGO;
int Sexy::MUSIC_GROOVE;

bool Sexy::ExtractInitResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		FONT_DEFAULT = aMgr.GetFontThrow(_S("FONT_DEFAULT"));
		IMAGE_CUSTOM_DRAGGING = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_DRAGGING"));
		IMAGE_CUSTOM_HAND = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_HAND"));
		IMAGE_CUSTOM_POINTER = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_POINTER"));
		IMAGE_CUSTOM_TEXT = aMgr.GetImageThrow(_S("IMAGE_CUSTOM_TEXT"));
		IMAGE_KANJI_LOGO = aMgr.GetImageThrow(_S("IMAGE_KANJI_LOGO"));
		MUSIC_GROOVE = aMgr.GetMusicThrow(_S("MUSIC_GROOVE"));
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

bool Sexy::ExtractTitleScreenResources(ResourceManager* theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;

	try
	{
		IMAGE_LOADER_BAR = aMgr.GetImageThrow(_S("IMAGE_LOADER_BAR"));
		IMAGE_LOADER_LOADINGTXT = aMgr.GetImageThrow(_S("IMAGE_LOADER_LOADINGTXT"));
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
	&IMAGE_CUSTOM_POINTER,
	&IMAGE_CUSTOM_HAND,
	&IMAGE_CUSTOM_DRAGGING,
	&IMAGE_CUSTOM_TEXT,
	&IMAGE_KANJI_LOGO,
	&MUSIC_GROOVE,
	&IMAGE_LOADER_BAR,
	&IMAGE_LOADER_LOADINGTXT,
	&IMAGE_ROBOTROBOT,
	&IMAGE_CHECKBOX,
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
		case IMAGE_CUSTOM_POINTER_ID: return _S("IMAGE_CUSTOM_POINTER");
		case IMAGE_CUSTOM_HAND_ID: return _S("IMAGE_CUSTOM_HAND");
		case IMAGE_CUSTOM_DRAGGING_ID: return _S("IMAGE_CUSTOM_DRAGGING");
		case IMAGE_CUSTOM_TEXT_ID: return _S("IMAGE_CUSTOM_TEXT");
		case IMAGE_KANJI_LOGO_ID: return _S("IMAGE_KANJI_LOGO");
		case MUSIC_GROOVE_ID: return _S("MUSIC_GROOVE");
		case IMAGE_LOADER_BAR_ID: return _S("IMAGE_LOADER_BAR");
		case IMAGE_LOADER_LOADINGTXT_ID: return _S("IMAGE_LOADER_LOADINGTXT");
		case IMAGE_ROBOTROBOT_ID: return _S("IMAGE_ROBOTROBOT");
		case IMAGE_CHECKBOX_ID: return _S("IMAGE_CHECKBOX");
		default: return _S("");
	}
}

