// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __EDITLISTENER_H__
#define __EDITLISTENER_H__

#include "Common.h"
#include "KKeyTable.h"

namespace Sexy
{
	class EditListener
	{
	public:
		/*!***********************************
		// @return    	void
		// @param     	theId
		// @param     	theString
		//
		// \brief		
		// \details 	<b>Description:</b> 
		//************************************/
		virtual void EditWidgetText(int theId, const SexyString& theString) {};

		/*!***********************************
		// @return    	bool
		// @param     	theId
		// @param     	theKey
		//
		// \brief		
		// \details 	<b>Description:</b> 
		//************************************/
		virtual bool AllowKey(int theId, EKeyboardLayout theKey) { return true; }
		/*!***********************************
		// @return    	bool
		// @param     	theId
		// @param     	theChar
		//
		// \brief		
		// \details 	<b>Description:</b> 
		//************************************/
		virtual bool AllowChar(int theId, SexyChar theChar) { return true; }
		/*!***********************************
		// @return    	bool
		// @param     	theId
		// @param     	theText
		//
		// \brief		
		// \details 	<b>Description:</b> 
		//************************************/
		virtual bool AllowText(int theId, const SexyString& theText) { return true; }
	};
}

#endif //__EDITLISTENER_H__
