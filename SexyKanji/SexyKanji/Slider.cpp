// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Slider.h"
#include "Graphics.h"
#include "SexyImage.h"
#include "SliderListener.h"
#include "WidgetManager.h"
#include "SexyAppBase.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
// @param     	theTrackImage
// @param     	theThumbImage
// @param     	theId
// @param     	theListener
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Slider::Slider(SexyImage* theTrackImage, SexyImage* theThumbImage, int theId, SliderListener* theListener) : 		
	mTrackImage(theTrackImage),
	mThumbImage(theThumbImage),
	mId(theId),
	mListener(theListener),
	mVal(0.0)
{
	mDragging = false;
	mHorizontal = true;
	mRelX = mRelY = 0;
}

/*!***********************************
// @return    	void
// @param     	theValue
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::SetValue(double theValue)
{	
	mVal = theValue;
	if (mVal < 0.0)
		mVal = 0.0;
	else if (mVal > 1.0)
		mVal = 1.0;

	MarkDirtyFull();
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Slider::HasTransparencies()
{
	return true;
}

/*!***********************************
// @return    	void
// @param     	g
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::Draw(Graphics* g)
{	
	if (mTrackImage != NULL)
	{
		int cw = /*mHorizontal ? mTrackImage->GetWidth()/3 :*/ mTrackImage->GetWidth();
		int ch = /*mHorizontal ?*/ mTrackImage->GetHeight() /*: mTrackImage->GetHeight()/3*/;

		if (mHorizontal)
		{
			int ty = (mHeight - ch) / 2;
			g->DrawImageBox(SexyRect(0,0,cw-1,ch-1), SexyRect(0, ty, mWidth, ch), mTrackImage);

// 			g->DrawImage(mTrackImage, 0, ty, SexyRect(0, 0, cw, ch));
// 
// 			Graphics aClipG(*g);
// 			aClipG.ClipRect(cw, ty, mWidth - cw*2, ch);
// 			for (int i = 0; i < (mWidth-cw*2+cw-1)/cw; i++)
// 				aClipG.DrawImage(mTrackImage, cw + i*cw, ty, SexyRect(cw, 0, cw, ch));
// 
// 			g->DrawImage(mTrackImage, mWidth-cw, ty, SexyRect(cw*2, 0, cw, ch));
		}
		else
		{
			g->DrawImage(mTrackImage, 0, 0, SexyRect(0, 0, cw, ch));

			g->PushState();
			g->ClipRect(0, ch, cw, mHeight - ch * 2);
			for (int i = 0; i < (mHeight-ch*2+ch-1)/ch; i++)
				g->DrawImage(mTrackImage, 0, ch + i*ch, SexyRect(0, ch, cw, ch));
			g->PopState();

			g->DrawImage(mTrackImage, 0, mHeight-ch, SexyRect(0, ch*2, cw, ch));
		}
	}

	if (mHorizontal && (mThumbImage != NULL))
		g->DrawImageF(mThumbImage, float(mVal * (mWidth - mThumbImage->GetWidth())), float(mHeight - mThumbImage->GetHeight()) / 2.0f);
	else if (!mHorizontal && (mThumbImage != NULL))
		g->DrawImageF(mThumbImage, float(mWidth - mThumbImage->GetWidth()) / 2.0f, float(mVal * (mHeight - mThumbImage->GetHeight())));

	//g->SetColor(Color(255, 255, 0));
	//g->FillRect(0, 0, mWidth, mHeight);	
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
// @param     	theClickCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::MouseDown(int x, int y, int theClickCount)
{
	if (mHorizontal)
	{
		int aThumbX = (int) (mVal * (mWidth - mThumbImage->GetWidth()));

		if ((x >= aThumbX) && (x < aThumbX + mThumbImage->GetWidth()))
		{
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_DRAGGING);
			mDragging = true;
			mRelX = x - aThumbX;
		}
		else
		{
			// clicked on the bar, set position to mouse click
			double pos = (double)x / mWidth;
			SetValue(pos);
		}
	}
	else
	{
		int aThumbY = (int) (mVal * (mHeight - mThumbImage->GetHeight()));

		if ((y >= aThumbY) && (y < aThumbY + mThumbImage->GetHeight()))
		{
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_DRAGGING);
			mDragging = true;
			mRelY = y - aThumbY;
		}
		else
		{
			// clicked on the bar, set position to mouse click
			double pos = (double)y / mHeight;
			SetValue(pos);
		}
	}
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::MouseMove(int x, int y)
{
	if (mHorizontal)
	{
		int aThumbX = (int) (mVal * (mWidth - mThumbImage->GetWidth()));

		if ((x >= aThumbX) && (x < aThumbX + mThumbImage->GetWidth()))
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_DRAGGING);
		else
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
	}
	else
	{
		int aThumbY = (int) (mVal * (mHeight - mThumbImage->GetHeight()));

		if ((y >= aThumbY) && (y < aThumbY + mThumbImage->GetHeight()))
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_DRAGGING);
		else
			mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
	}
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::MouseDrag(int x, int y)
{
	if (mDragging)
	{	
		double anOldVal = mVal;

		if (mHorizontal)
			mVal = (x - mRelX) / (double) (mWidth - mThumbImage->GetWidth());
		else
			mVal = (y - mRelY) / (double) (mHeight - mThumbImage->GetHeight());

		if (mVal < 0.0)
			mVal = 0.0;
		if (mVal > 1.0)
			mVal = 1.0;

		if (mVal != anOldVal)
		{
			mListener->SliderVal(mId, mVal);
			MarkDirtyFull();
		}
	}
}

/*!***********************************
// @return    	void
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::MouseUp(int x, int y)
{
	mDragging = false;
	mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
	mListener->SliderVal(mId, mVal);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Slider::MouseLeave()
{
	if (!mDragging)
		mWidgetManager->mApp->SetCursor(SEXY_CURSOR_POINTER);
}