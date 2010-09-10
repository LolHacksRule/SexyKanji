// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __HYPERLINKWIDGET_H__
#define __HYPERLINKWIDGET_H__

#include "ButtonWidget.h"

namespace Sexy
{

class HyperlinkWidget : public ButtonWidget
{
public:
	SexyColor				mColor;
	SexyColor				mOverColor;
	int						mUnderlineSize;
	int						mUnderlineOffset;

public:
	HyperlinkWidget(int theId, ButtonListener* theButtonListener);

	void					Draw(Graphics* g);
	void					MouseEnter();
	void					MouseLeave();	
};

}

#endif //__HYPERLINKWIDGET_H__