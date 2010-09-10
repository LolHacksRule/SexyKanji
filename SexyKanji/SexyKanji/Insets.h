// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __INSETS_H__
#define __INSETS_H__

namespace Sexy
{

class Insets
{
public:
	int						mLeft;
	int						mTop;
	int						mRight;
	int						mBottom;

public:
	Insets();
	Insets(int theLeft, int theTop, int theRight, int theBottom);
	Insets(const Insets& theInsets);			
};

}

#endif //__INSETS_H__