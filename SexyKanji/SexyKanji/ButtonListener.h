// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __BUTTONLISTENER_H__
#define __BUTTONLISTENER_H__

namespace Sexy
{

class ButtonListener
{	
public:
	/*!***********************************
	// @return    	void
	// @param     	theId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonPress(int theId) {}
	/*!***********************************
	// @return    	void
	// @param     	theId
	// @param     	theClickCount
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonPress(int theId, int theClickCount) { ButtonPress(theId); }
	/*!***********************************
	// @return    	void
	// @param     	theId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonDepress(int theId) {}
	/*!***********************************
	// @return    	void
	// @param     	theId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonDownTick(int theId) {}
	/*!***********************************
	// @return    	void
	// @param     	theId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonMouseEnter(int theId) {}
	/*!***********************************
	// @return    	void
	// @param     	theId
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonMouseLeave(int theId) {}
	/*!***********************************
	// @return    	void
	// @param     	theId
	// @param     	theX
	// @param     	theY
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	virtual void			ButtonMouseMove(int theId, int theX, int theY) {}
};

}

#endif //__BUTTONLISTENER_H__