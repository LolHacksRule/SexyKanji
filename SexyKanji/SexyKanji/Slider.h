// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Widget.h"

namespace Sexy
{

class SliderListener;

class Slider : public Widget
{
public:		
	SliderListener*			mListener;
	double					mVal;
	int						mId;
	SexyImage*					mTrackImage;
	SexyImage*					mThumbImage;

	bool					mDragging;
	int						mRelX;
	int						mRelY;

	bool					mHorizontal;

public:
	Slider(SexyImage* theTrackImage, SexyImage* theThumbImage, int theId, SliderListener* theListener);

	virtual void			SetValue(double theValue);

	virtual bool			HasTransparencies();
	virtual void			Draw(Graphics* g);	

	virtual void			MouseMove(int x, int y);
	virtual void			MouseDown(int x, int y, int theClickCount);
	virtual void			MouseDrag(int x, int y);
	virtual void			MouseUp(int x, int y);
	virtual void			MouseLeave();
};

}

#endif //__SLIDER_H__