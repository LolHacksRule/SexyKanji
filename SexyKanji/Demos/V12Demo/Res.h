/********************************************************************
	Generated by Sexy Kanji Resource Generator (JPoag/HFischer)
	-----------------------------------------------------------
	Created:	04.04.2010 14:15:50
	Filename:	Res.h

	Purpose:	Functions and globals, based on the resource.xml file,
				to easily use the resources in your program.
*********************************************************************/

#ifndef __RES_H__
#define __RES_H__

#include "Common.h"

namespace Sexy
{
	// forward declarations
	class ResourceManager;
	class SexyImage;
	class SexyFont;
	class SexyMovie;

	SexyImage* LoadImageById(ResourceManager* theManager, int theId);
	void ReplaceImageById(ResourceManager* theManager, int theId, SexyImage* theImage);
	bool ExtractResourcesByName(ResourceManager* theManager, SexyString theName);

	// Game Resources

	bool ExtractGameResources(ResourceManager* theMgr);

	extern int SOUND_MUTATOR;
	extern int SOUND_TIMER;
	extern SexyImage* IMAGE_BG0;
	extern SexyImage* IMAGE_BG1;
	extern SexyImage* IMAGE_BG2;
	extern SexyImage* IMAGE_BUTTON_DOWN;
	extern SexyImage* IMAGE_BUTTON_NORMAL;
	extern SexyImage* IMAGE_BUTTON_OVER;
	extern SexyImage* IMAGE_CHECKBOX;
	extern SexyImage* IMAGE_DIALOG_BOX;
	extern SexyImage* IMAGE_DIALOG_BUTTON;
	extern SexyImage* IMAGE_ROBOTROBOT;
	extern SexyImage* IMAGE_SLIDER_THUMB;
	extern SexyImage* IMAGE_SLIDER_TRACK;

	// Hungarr Resources

	bool ExtractHungarrResources(ResourceManager* theMgr);

	extern int SOUND_BEAM_HIT;
	extern int SOUND_BEAM_MOVING;
	extern int SOUND_BUTTON;
	extern int SOUND_EXPLOSION;
	extern int SOUND_GAME_OVER_CLICK;
	extern int SOUND_GAME_OVER_RESTART;
	extern int SOUND_GAME_OVER_STATS;
	extern int SOUND_GAME_OVER_TEXT;
	extern int SOUND_LEVEL_UP1;
	extern int SOUND_LEVEL_UP2;
	extern int SOUND_LEVEL_UP3;
	extern int SOUND_LEVEL_UP4;
	extern int SOUND_MAGZAP;
	extern int SOUND_PLANET;
	extern int SOUND_PLANET_HIT;
	extern int SOUND_REGION_FILLED;
	extern SexyImage* IMAGE_ATOMIC_EXPLOSION;
	extern SexyImage* IMAGE_BOMB_RADIAL_DEATH;
	extern SexyImage* IMAGE_HUNGARR_BEAM_DOWN;
	extern SexyImage* IMAGE_HUNGARR_BEAM_LEFT;
	extern SexyImage* IMAGE_HUNGARR_BEAM_RIGHT;
	extern SexyImage* IMAGE_HUNGARR_BEAM_UP;
	extern SexyImage* IMAGE_HUNGARR_HORIZ;
	extern SexyImage* IMAGE_HUNGARR_SMALL;
	extern SexyImage* IMAGE_HUNGARR_VERT;
	extern SexyImage* IMAGE_PARTICLE_LIGHTNING;
	extern SexyImage* IMAGE_PLANETS;
	extern SexyImage* IMAGE_SPARK;

	// Init Resources

	bool ExtractInitResources(ResourceManager* theMgr);

	extern SexyFont* FONT_DEFAULT;
	extern SexyFont* FONT_HUNGARR;
	extern SexyImage* IMAGE_CUSTOM_DRAGGING;
	extern SexyImage* IMAGE_CUSTOM_HAND;
	extern SexyImage* IMAGE_CUSTOM_POINTER;
	extern SexyImage* IMAGE_CUSTOM_TEXT;
	extern SexyImage* IMAGE_HUNGARR_LOGO;
	extern SexyImage* IMAGE_KANJI_LOGO;

	// TitleScreen Resources

	bool ExtractTitleScreenResources(ResourceManager* theMgr);

	extern int SOUND_CONTINUE;
	extern SexyImage* IMAGE_LOADER_BAR;
	extern SexyImage* IMAGE_LOADER_LOADINGTXT;

	// Resource IDs

	enum ResourceId
	{
		FONT_DEFAULT_ID,
		FONT_HUNGARR_ID,
		IMAGE_CUSTOM_POINTER_ID,
		IMAGE_CUSTOM_HAND_ID,
		IMAGE_CUSTOM_DRAGGING_ID,
		IMAGE_CUSTOM_TEXT_ID,
		IMAGE_HUNGARR_LOGO_ID,
		IMAGE_KANJI_LOGO_ID,
		IMAGE_LOADER_BAR_ID,
		IMAGE_LOADER_LOADINGTXT_ID,
		SOUND_CONTINUE_ID,
		SOUND_MUTATOR_ID,
		SOUND_TIMER_ID,
		IMAGE_ROBOTROBOT_ID,
		IMAGE_CHECKBOX_ID,
		IMAGE_BG0_ID,
		IMAGE_BG1_ID,
		IMAGE_BG2_ID,
		IMAGE_BUTTON_DOWN_ID,
		IMAGE_BUTTON_OVER_ID,
		IMAGE_BUTTON_NORMAL_ID,
		IMAGE_DIALOG_BOX_ID,
		IMAGE_DIALOG_BUTTON_ID,
		IMAGE_SLIDER_TRACK_ID,
		IMAGE_SLIDER_THUMB_ID,
		IMAGE_HUNGARR_SMALL_ID,
		IMAGE_HUNGARR_BEAM_UP_ID,
		IMAGE_HUNGARR_BEAM_DOWN_ID,
		IMAGE_HUNGARR_BEAM_LEFT_ID,
		IMAGE_HUNGARR_BEAM_RIGHT_ID,
		IMAGE_HUNGARR_HORIZ_ID,
		IMAGE_HUNGARR_VERT_ID,
		IMAGE_ATOMIC_EXPLOSION_ID,
		IMAGE_BOMB_RADIAL_DEATH_ID,
		IMAGE_PLANETS_ID,
		IMAGE_SPARK_ID,
		IMAGE_PARTICLE_LIGHTNING_ID,
		SOUND_MAGZAP_ID,
		SOUND_BUTTON_ID,
		SOUND_PLANET_ID,
		SOUND_LEVEL_UP1_ID,
		SOUND_LEVEL_UP2_ID,
		SOUND_EXPLOSION_ID,
		SOUND_BEAM_HIT_ID,
		SOUND_PLANET_HIT_ID,
		SOUND_BEAM_MOVING_ID,
		SOUND_LEVEL_UP4_ID,
		SOUND_LEVEL_UP3_ID,
		SOUND_GAME_OVER_CLICK_ID,
		SOUND_GAME_OVER_STATS_ID,
		SOUND_GAME_OVER_RESTART_ID,
		SOUND_GAME_OVER_TEXT_ID,
		SOUND_REGION_FILLED_ID,
		RESOURCE_ID_MAX
	};

	// Getter functions

	SexyImage* GetImageById(int theId);
	SexyFont* GetFontById(int theId);
	int GetSoundById(int theId);
	int GetMusicById(int theId);
	SexyMovie* GetMovieById(int theId);

	SexyImage*& GetImageRefById(int theId);
	SexyFont*& GetFontRefById(int theId);
	int& GetSoundRefById(int theId);
	int& GetMusicRefById(int theId);
	SexyMovie*& GetMovieRefById(int theId);

	ResourceId GetIdByImage(SexyImage* theImage);
	ResourceId GetIdByFont(SexyFont* theFont);
	ResourceId GetIdBySound(int theSound);
	ResourceId GetIdByMusic(int theMusic);
	ResourceId GetIdByMovie(SexyMovie* theMovie);

	SexyString GetStringIdById(int theId);
	ResourceId GetIdByStringId(SexyString theStringId);

} // namespace Sexy

#endif // __RES_H__
