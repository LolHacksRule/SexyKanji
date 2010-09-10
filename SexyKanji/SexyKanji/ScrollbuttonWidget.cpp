// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "ScrollbuttonWidget.h"
#include "ButtonListener.h"
#include "Graphics.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theId
// @param     	theButtonListener
// @param     	theType
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ScrollbuttonWidget::ScrollbuttonWidget(int theId, ButtonListener *theButtonListener, int theType) : ButtonWidget(theId, theButtonListener) 
{
	mHorizontal = false;
	mType = theType;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
ScrollbuttonWidget::~ScrollbuttonWidget() 
{
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void ScrollbuttonWidget::Draw(Graphics *g) 
{
	int anArrowOffset = 0;
		
	g->SetColor(SexyColor(212, 212, 212));
	g->FillRect(0, 0, mWidth, mHeight);
		
	if (mIsDown && mIsOver && !mDisabled) 
	{
		anArrowOffset = 1;
		g->SetColor(SexyColor(132, 132, 132));
		g->DrawRect(0, 0, mWidth-1, mHeight-1);
	}
	else 
	{
		g->SetColor(SexyColor(255, 255, 255));
		g->FillRect(1, 1, mWidth-2, 1);
		g->FillRect(1, 1, 1, mHeight-2);
		
		g->SetColor(SexyColor::Black);
		g->FillRect(0, mHeight - 1, mWidth, 1);
		g->FillRect(mWidth - 1, 0, 1, mHeight);						
	
		g->SetColor(SexyColor(132, 132, 132));
		g->FillRect(1, mHeight - 2, mWidth - 2, 1);
		g->FillRect(mWidth - 2, 1, 1, mHeight - 2);
	}
		
	if (!mDisabled) 
		g->SetColor(SexyColor::Black);	
	else 
		g->SetColor(SexyColor(132, 132, 132));

	if (mHorizontal || (mType==3 || mType==4))
	{
		for (int i = 0; i < 4; i++)	
		{
			if (mId == 0 || mType==3) 
				g->FillRect(i + (mWidth-4)/2 + anArrowOffset, mHeight/2 - i - 1 + anArrowOffset, 1, 1 + i*2);
			else 
				g->FillRect((3 - i) + (mWidth-4)/2 + anArrowOffset, mHeight/2 - i - 1 + anArrowOffset, 1, 1 + i*2);
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)	
		{
			if (mId == 0 || mType==1) 
				g->FillRect(mWidth/2 - i - 1 + anArrowOffset, i + (mHeight-4)/2 + anArrowOffset, 1 + i*2, 1);
			else 
				g->FillRect(mWidth/2 - i - 1 + anArrowOffset, (3 - i) + (mHeight-4)/2 + anArrowOffset, 1 + i*2, 1);
		}
	}
}
