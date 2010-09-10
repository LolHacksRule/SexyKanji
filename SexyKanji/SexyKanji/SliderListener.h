// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __SLIDERLISTENER_H__
#define __SLIDERLISTENER_H__

namespace Sexy
{

class SliderListener
{
public:
	/*!***********************************
	// @return    	void
	// @param     	theId
	// @param     	theVal
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			SliderVal(int theId, double theVal) {};
};

}

#endif //__SLIDERLISTENER_H__
