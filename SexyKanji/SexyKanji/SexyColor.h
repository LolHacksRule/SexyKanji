// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __COLOR_H__
#define __COLOR_H__

namespace Sexy
{

#pragma pack(push,1)
struct SexyRGBA {unsigned char b,g,r,a;};
#pragma pack(pop)

class SexyColor
{
public:
	int mRed;
	int mGreen;
	int mBlue;
	int mAlpha;

	static SexyColor Black;
	static SexyColor White;

public:
	SexyColor();
	SexyColor(int theColor);
	SexyColor(int theColor, int theAlpha);
	SexyColor(int theRed, int theGreen, int theBlue);
	SexyColor(int theRed, int theGreen, int theBlue, int theAlpha);
	SexyColor(const SexyRGBA &theColor);
	SexyColor(const unsigned char* theElements);	
	SexyColor(const int* theElements);

	int						GetRed() const;
	int						GetGreen() const;
	int						GetBlue() const;
	int						GetAlpha() const;
	unsigned long			ToInt() const;
	SexyRGBA				ToRGBA() const;

	int&					operator[](int theIdx);
	int						operator[](int theIdx) const;	

	static unsigned long		HSLToRGB(int h, int s, int l);
	static unsigned long		RGBToHSL(int r, int g, int b);
	static void					HSLToRGB(const unsigned long* theSource, unsigned long* theDest, int theSize);
	static void					RGBToHSL(const unsigned long* theSource, unsigned long* theDest, int theSize);
};

bool operator==(const SexyColor& theColor1, const SexyColor& theColor2);
bool operator!=(const SexyColor& theColor1, const SexyColor& theColor2);

}

#endif //__COLOR_H__