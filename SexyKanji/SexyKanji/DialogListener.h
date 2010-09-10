// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __DIALOGLISTENER_H__
#define __DIALOGLISTENER_H__

namespace Sexy
{

class DialogListener
{
public:
	/*!***********************************
	// @return    	void
	// @param     	theDialogId
	// @param     	theButtonId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			DialogButtonPress(int theDialogId, int theButtonId) {}
	/*!***********************************
	// @return    	void
	// @param     	theDialogId
	// @param     	theButtonId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			DialogButtonDepress(int theDialogId, int theButtonId) {}
};

}

#endif // __DIALOGLISTENER_H__