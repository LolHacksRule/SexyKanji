// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "HyperlinkWidget.h"
#include "Graphics.h"
#include "SexyFont.h"
#include "WidgetManager.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theId
// @param     	theButtonListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
HyperlinkWidget::HyperlinkWidget(int theId, ButtonListener* theButtonListener) :
	ButtonWidget(theId, theButtonListener),
	mColor(255, 255, 255),
	mOverColor(255, 255, 255)
{	
	mDoFinger = true;
	mUnderlineOffset = 3;
	mUnderlineSize = 1;
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void HyperlinkWidget::Draw(Graphics* g)
{
	//if (mFont == NULL)
	//	mFont = new SysFont(mWidgetManager->mApp, "Arial Unicode MS", 10); //baz changed

	int aFontX = (mWidth - mFont->StringWidth(mLabel)) / 2;
	int aFontY = (mHeight / 2) - (mFont->GetAscent() / 2);

	if (mIsOver)
		g->SetColor(mOverColor);
	else
		g->SetColor(mColor);

	g->SetFont(mFont);	
	g->DrawString(mLabel, aFontX, aFontY);

	for (int i = 0; i < mUnderlineSize; i++)
		g->FillRect(aFontX, aFontY + mFont->GetAscent() + i, mFont->StringWidth(mLabel), 1);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void HyperlinkWidget::MouseEnter()
{
	ButtonWidget::MouseEnter();

	MarkDirtyFull();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void HyperlinkWidget::MouseLeave()
{
	ButtonWidget::MouseLeave();

	MarkDirtyFull();
}