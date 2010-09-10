// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SCROLLLISTENER_H__
#define __SCROLLLISTENER_H__

namespace Sexy 
{

class ScrollListener 
{
public:
	/*!***********************************
	// @return    	void
	// @param     	theId
	// @param     	thePosition
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void ScrollPosition(int theId, double thePosition) {};
};

}

#endif // __SCROLLLISTENER_H__
