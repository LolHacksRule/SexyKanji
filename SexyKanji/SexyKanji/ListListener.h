// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __LISTLISTENER_H__
#define __LISTLISTENER_H__

namespace Sexy 
{

class ListListener 
{
public:
	virtual void ListClicked(int theId, int theIdx, int theClickCount) {}
	virtual void ListClosed(int theId) {}
	virtual void ListHiliteChanged(int theId, int theOldIdx, int theNewIdx) {}
};

}

#endif //__LISTLISTENER_H__
