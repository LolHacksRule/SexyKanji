// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyColor.h"

#include "KMath2d.h" // K_MIN & K_MAX

using namespace Sexy;

SexyColor SexyColor::Black(0, 0, 0);
SexyColor SexyColor::White(255, 255, 255);

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor() :
	mRed(0),
	mGreen(0),
	mBlue(0),
	mAlpha(255)
{
}

/*!***********************************
// @return    	
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(int theColor) :
	mAlpha((theColor >> 24) & 0xFF),
	mRed((theColor   >> 16) & 0xFF),
	mGreen((theColor >> 8 ) & 0xFF),
	mBlue((theColor       ) & 0xFF)
{
	if(mAlpha==0)
		mAlpha = 0xff;
}

/*!***********************************
// @return    	
// @param     	theColor
// @param     	theAlpha
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(int theColor, int theAlpha) :
	mRed((theColor   >> 16) & 0xFF),
	mGreen((theColor >> 8 ) & 0xFF),
	mBlue((theColor       ) & 0xFF),
	mAlpha(theAlpha)
{
}

/*!***********************************
// @return    	
// @param     	theRed
// @param     	theGreen
// @param     	theBlue
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(int theRed, int theGreen, int theBlue) :
	mRed(theRed),
	mGreen(theGreen),
	mBlue(theBlue),
	mAlpha(0xFF)
{
}

/*!***********************************
// @return    	
// @param     	theRed
// @param     	theGreen
// @param     	theBlue
// @param     	theAlpha
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(int theRed, int theGreen, int theBlue, int theAlpha) :
	mRed(theRed),
	mGreen(theGreen),
	mBlue(theBlue),
	mAlpha(theAlpha)
{
}

/*!***********************************
// @return    	
// @param     	theColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(const SexyRGBA &theColor) :
	mRed(theColor.r),
	mGreen(theColor.g),
	mBlue(theColor.b),
	mAlpha(theColor.a)
{
}

/*!***********************************
// @return    	
// @param     	theElements
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(const unsigned char* theElements) :
	mRed(theElements[0]),
	mGreen(theElements[1]),
	mBlue(theElements[2]),
	mAlpha(0xFF)
{
}

/*!***********************************
// @return    	
// @param     	theElements
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyColor::SexyColor(const int* theElements) :
	mRed(theElements[0]),
	mGreen(theElements[1]),
	mBlue(theElements[2]),
	mAlpha(0xFF)
{
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int	SexyColor::GetRed() const
{
	return mRed;	
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int SexyColor::GetGreen() const
{
	return mGreen;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int	SexyColor::GetBlue() const
{
	return mBlue;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int	SexyColor::GetAlpha() const
{
	return mAlpha;
}

int& SexyColor::operator[](int theIdx)
{
	static int aJunk = 0;

	switch (theIdx)
	{
	case 0:
		return mRed;
	case 1:
		return mGreen;
	case 2:
		return mBlue;
	case 3:
		return mAlpha;
	default:
		return aJunk;
	}
}

int SexyColor::operator[](int theIdx) const
{
	switch (theIdx)
	{
	case 0:
		return mRed;
	case 1:
		return mGreen;
	case 2:
		return mBlue;
	case 3:
		return mAlpha;
	default:
		return 0;
	}
}

unsigned long SexyColor::ToInt() const
{
	return (mAlpha << 24) | (mRed << 16) | (mGreen << 8) | (mBlue);
}

/*!***********************************
// @return    	Sexy::SexyRGBA
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyRGBA SexyColor::ToRGBA() const
{
	SexyRGBA anRGBA;
	anRGBA.r = mRed;
	anRGBA.g = mGreen;
	anRGBA.b = mBlue;
	anRGBA.a = mAlpha;

	return anRGBA;
}

bool Sexy::operator==(const SexyColor& theColor1, const SexyColor& theColor2)
{
	return 
		(theColor1.mRed == theColor2.mRed) &&
		(theColor1.mGreen == theColor2.mGreen) &&
		(theColor1.mBlue == theColor2.mBlue) && 
		(theColor1.mAlpha == theColor2.mAlpha);
}

bool Sexy::operator!=(const SexyColor& theColor1, const SexyColor& theColor2)
{
	return 
		(theColor1.mRed != theColor2.mRed) ||
		(theColor1.mGreen != theColor2.mGreen) ||
		(theColor1.mBlue != theColor2.mBlue) ||
		(theColor1.mAlpha != theColor2.mAlpha);
}

/*!***********************************
// @return    	unsigned long
// @param     	h
// @param     	s
// @param     	l
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
unsigned long SexyColor::HSLToRGB(int h, int s, int l)
{
	int r;
	int g;
	int b;

	double v= (l < 128) ? (l * (255+s))/255 :
			(l+s-l*s/255);
	
	int y = (int) (2*l-v);

	int aColorDiv = (6 * h) / 256;
	int x = (int)(y+(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
	if (x > 255)
		x = 255;

	int z = (int) (v-(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
	if (z < 0)
		z = 0;
	
	switch (aColorDiv)
	{
		case 0: r = (int) v; g = x; b = y; break;
		case 1: r = z; g= (int) v; b = y; break;
		case 2: r = y; g= (int) v; b = x; break;
		case 3: r = y; g = z; b = (int) v; break;
		case 4: r = x; g = y; b = (int) v; break;
		case 5: r = (int) v; g = y; b = z; break;
		default: r = (int) v; g = x; b = y; break;
	}

	return 0xFF000000 | (r << 16) | (g << 8) | (b);
}

/*!***********************************
// @return    	unsigned long
// @param     	r
// @param     	g
// @param     	b
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
unsigned long SexyColor::RGBToHSL(int r, int g, int b)
{					
	int maxval = K_MAX(r, K_MAX(g, b));
	int minval = K_MIN(r, K_MIN(g, b));
	int hue = 0;
	int saturation = 0;
	int luminosity = (minval+maxval)/2;
	int delta = maxval - minval;

	if (delta != 0)
	{			
		saturation = (delta * 256) / ((luminosity <= 128) ? (minval + maxval) : (512 - maxval - minval));
		
		if (r == maxval)
			hue = (g == minval ? 1280 + (((maxval-b) * 256) / delta) :  256 - (((maxval - g) * 256) / delta));
		else if (g == maxval)
			hue = (b == minval ?  256 + (((maxval-r) * 256) / delta) :  768 - (((maxval - b) * 256) / delta));
		else
			hue = (r == minval ?  768 + (((maxval-g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));
		
		hue /= 6;
	}

	return 0xFF000000 | (hue) | (saturation << 8) | (luminosity << 16);	 
}

/*!***********************************
// @return    	void
// @param     	theSource
// @param     	theDest
// @param     	theSize
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyColor::HSLToRGB(const unsigned long* theSource, unsigned long* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		unsigned long src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (HSLToRGB((src & 0xFF), (src >> 8) & 0xFF, (src >> 16) & 0xFF) & 0x00FFFFFF);
	}
}

/*!***********************************
// @return    	void
// @param     	theSource
// @param     	theDest
// @param     	theSize
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyColor::RGBToHSL(const unsigned long* theSource, unsigned long* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		unsigned long src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (RGBToHSL(((src >> 16) & 0xFF), (src >> 8) & 0xFF, (src & 0xFF)) & 0x00FFFFFF);
	}
}
