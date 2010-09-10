// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Checkbox.h"
#include "CheckboxListener.h"
#include "Graphics.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theUncheckedImage
// @param     	theCheckedImage
// @param     	theId
// @param     	theCheckboxListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Checkbox::Checkbox(SexyImage* theUncheckedImage, SexyImage* theCheckedImage, int theId, CheckboxListener* theCheckboxListener) :	
	mUncheckedImage(theUncheckedImage),
	mCheckedImage(theCheckedImage),
	mId(theId),
	mListener(theCheckboxListener),
	mChecked(false)	,
	mOutlineColor(SexyColor::White),
	mBkgColor(SexyColor(80, 80, 80)),
	mCheckColor(SexyColor(255, 255, 0))
{
	mDoFinger = true;
}

/*!***********************************
// @return    	void
// @param     	checked
// @param     	tellListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Checkbox::SetChecked(bool checked, bool tellListener)
{
	mChecked = checked;
	if (tellListener && mListener)
		mListener->CheckboxChecked(mId, mChecked);
	MarkDirty();
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Checkbox::IsChecked()
{
	return mChecked;
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Checkbox::Draw(Graphics* g)
{
	Widget::Draw(g);

	if ((mCheckedRect.mWidth == 0) && (mCheckedImage != NULL) && (mUncheckedImage != NULL))
	{
		if (mChecked)
			g->DrawImage(mCheckedImage, 0, 0);
		else
			g->DrawImage(mUncheckedImage, 0, 0);
	}
	else if ((mCheckedRect.mWidth != 0) && (mUncheckedImage != NULL))
	{
		if (mChecked)
			g->DrawImage(mUncheckedImage, 0, 0, mCheckedRect);
		else
			g->DrawImage(mUncheckedImage, 0, 0, mUncheckedRect);
	}
	else if ((mUncheckedImage == NULL) && (mCheckedImage == NULL))
	{
		// No image, default draw method
		g->SetColor(mOutlineColor);
		g->FillRect(0, 0, mWidth, mHeight);
		g->SetColor(mBkgColor);
		g->FillRect(1, 1, mWidth - 2, mHeight - 2);

		if (mChecked)
		{
			g->SetColor(mCheckColor);
			g->DrawLine(1, 1, mWidth - 2, mHeight - 2);
			g->DrawLine(mWidth - 1, 1, 1, mHeight - 2);
		}
	}
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theBtnNum
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Checkbox::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseDown(x, y, theBtnNum, theClickCount);

	// Double click.
	if ((theClickCount == 2) || (theClickCount == -2))
	{
		return;
	}

	mChecked = !mChecked;
	if (mListener)
		mListener->CheckboxChecked(mId, mChecked);
	MarkDirty();
}
