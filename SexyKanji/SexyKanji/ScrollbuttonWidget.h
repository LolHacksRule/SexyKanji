// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SCROLLBUTTONWIDGET_H__
#define __SCROLLBUTTONWIDGET_H__

#include "ButtonWidget.h"

namespace Sexy 
{
	
class ButtonListener;

class ScrollbuttonWidget : public ButtonWidget 
{
public:
	bool				mHorizontal;

	// mType is a new way of doing things (1 = up, 2 = down, 3 = left, 4 = right).  
	// This overrides mHorizontal and mId.
	int					mType; 

public:
	ScrollbuttonWidget(int theId, ButtonListener *theButtonListener, int theType = 0);
	virtual ~ScrollbuttonWidget();

	void				Draw(Graphics *g);
};

}

#endif //__SCROLLBUTTONWIDGET_H__
