// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
created:	2010/04/24
created:	24:4:2010   8:20
filename: 	SexyKanji\SharedFont.h
author:		Andrei Lazutin

purpose:	Provide the reference counting for SexyImage to be used
in the ImageManager.
*********************************************************************/
#ifndef __SHARED_IMAGE_H__
#define __SHARED_IMAGE_H__
#pragma once

#include "SexyImage.h"
#include "SharedItem.h"

namespace Sexy
{
    typedef SharedItem<SexyImage> SharedImage;
    typedef SharedItemRef<SexyImage> SharedImageRef;
    typedef std::map<std::pair<SexyString, SexyString>, SharedImage> SharedImageMap;
};


#endif //__SHARED_IMAGE_H__
