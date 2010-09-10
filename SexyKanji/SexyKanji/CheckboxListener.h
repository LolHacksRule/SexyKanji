// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __CHECKBOX_LISTENER__
#define __CHECKBOX_LISTENER__

namespace Sexy
{

class CheckboxListener
{
public:
	/*!***********************************
	// @return    	void
	// @param     	theId
	// @param     	checked
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			CheckboxChecked(int theId, bool checked) {}
};

}

#endif //__CHECKBOX_LISTENER__