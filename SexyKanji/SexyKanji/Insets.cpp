// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Insets.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Insets::Insets() :
	mLeft(0),
	mTop(0),
	mRight(0),
	mBottom(0)
{
		int a = 0;
}

/*!***********************************
// @return    	
// @param     	theLeft
// @param     	theTop
// @param     	theRight
// @param     	theBottom
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Insets::Insets(int theLeft, int theTop, int theRight, int theBottom) :
	mLeft(theLeft),
	mTop(theTop),
	mRight(theRight),
	mBottom(theBottom)
{
}

/*!***********************************
// @return    	
// @param     	theInsets
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Insets::Insets(const Insets& theInsets) :
	mLeft(theInsets.mLeft),
	mTop(theInsets.mTop),
	mRight(theInsets.mRight),
	mBottom(theInsets.mBottom)
{
}

