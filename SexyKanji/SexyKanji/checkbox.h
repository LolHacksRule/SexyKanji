// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "Widget.h"

namespace Sexy
{

class CheckboxListener;
class SexyImage;

class Checkbox : public Widget
{	
protected:
	CheckboxListener*		mListener;

public:
	int						mId;

	bool					mChecked;

	SexyImage*				mUncheckedImage;
	SexyImage*				mCheckedImage;

	SexyRect				mCheckedRect;
	SexyRect				mUncheckedRect;

	SexyColor				mOutlineColor;	// These are only used if no image is specified
	SexyColor				mBkgColor;
	SexyColor				mCheckColor;

public:
	virtual void			SetChecked(bool checked, bool tellListener = true);
	virtual bool			IsChecked();

	virtual void			MouseDown(int x, int y, int theClickCount) { Widget::MouseDown(x, y, theClickCount); }
	virtual void			MouseDown(int x, int y, int theBtnNum, int theClickCount);
	virtual void			Draw(Graphics* g);

public:
	Checkbox(SexyImage* theUncheckedImage, SexyImage* theCheckedImage, int theId, CheckboxListener* theCheckboxListener);
};

}

#endif //__CHECKBOX_H__