// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __DIALOGBUTTON_H__
#define __DIALOGBUTTON_H__

#include "ButtonWidget.h"

namespace Sexy
{

class DialogButton : public ButtonWidget
{
public:	
	SexyImage*				mComponentImage;
	int						mTranslateX, mTranslateY;
	int						mTextOffsetX, mTextOffsetY;

public:
	DialogButton(SexyImage* theComponentImage, int theId, ButtonListener* theListener);

	virtual void			Draw(Graphics* g);
};

}

#endif //__DIALOGBUTTON_H__