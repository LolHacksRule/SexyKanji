// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __TRectANGLE_H__
#define __TRectANGLE_H__

#include "SexyPoint.h"
#include "KMath2d.h"

#include <list>

namespace Sexy
{

template<class _T> class TRect
{
public:
	_T mX;
	_T mY;
	_T mWidth;
	_T mHeight;

public:
	TRect(_T theX, _T theY, _T theWidth, _T theHeight) :
	  mX(theX), mY(theY), mWidth(theWidth), mHeight(theHeight)
	{
	}
	
	TRect(const TRect<_T>& theTRect) :
	  mX(theTRect.mX), mY(theTRect.mY), mWidth(theTRect.mWidth), mHeight(theTRect.mHeight)
	{
	}

	TRect() :
	  mX(0), mY(0), mWidth(0), mHeight(0)
	{
	}

	/*!***********************************
	// @return    	bool
	// @param     	theTRect
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	bool					Intersects(const TRect<_T>& theTRect) const
	{
		return !((theTRect.mX + theTRect.mWidth <= mX) ||
			(theTRect.mY + theTRect.mHeight <= mY) ||
			(theTRect.mX >= mX + mWidth) ||
			(theTRect.mY >= mY + mHeight));
	}

	/*!***********************************
	// @return    	TRect<_T>
	// @param     	theTRect
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	TRect<_T>				Intersection(const TRect<_T>& theTRect)	const
	{
		_T x1 = K_MAX(mX, theTRect.mX);
		_T x2 = K_MIN(mX + mWidth, theTRect.mX + theTRect.mWidth);
		_T y1 = K_MAX(mY, theTRect.mY);
		_T y2 = K_MIN(mY + mHeight, theTRect.mY + theTRect.mHeight);
		if (((x2 - x1) < 0) || ((y2 - y1) < 0))
			return TRect<_T>(0,0,0,0);
		else
			return TRect<_T>(x1, y1, x2 - x1, y2 - y1);
	}

	/*!***********************************
	// @return    	TRect<_T>
	// @param     	theTRect
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	TRect<_T>				Union(const TRect<_T>& theTRect)	
	{
		_T x1 = K_MIN(mX, theTRect.mX);
		_T x2 = K_MAX(mX + mWidth, theTRect.mX + theTRect.mWidth);
		_T y1 = K_MIN(mY, theTRect.mY);
		_T y2 = K_MAX(mY + mHeight, theTRect.mY + theTRect.mHeight);
			return TRect<_T>(x1, y1, x2 - x1, y2 - y1);
	}

	/*!***********************************
	// @return    	bool
	// @param     	theX
	// @param     	theY
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	bool					Contains(_T theX, _T theY) const
	{
		return ((theX >= mX) && (theX < mX + mWidth) &&
			(theY >= mY) && (theY < mY + mHeight));
	}

	/*!***********************************
	// @return    	bool
	// @param     	thePoint
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	bool					Contains(const TPoint<_T>& thePoint) const
	{
		return ((thePoint.mX >= mX) && (thePoint.mX < mX + mWidth) &&
			(thePoint.mY >= mY) && (thePoint.mY < mY + mHeight));
	}

	/*!***********************************
	// @return    	void
	// @param     	theX
	// @param     	theY
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	void					Offset(_T theX, _T theY)
	{
		mX += theX;
		mY += theY;
	}

	/*!***********************************
	// @return    	void
	// @param     	thePoint
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	void					Offset(const TPoint<_T>& thePoint)
	{
		mX += thePoint.mX;
		mY += thePoint.mY;
	}

	/*!***********************************
	// @return    	Sexy::TRect
	// @param     	theX
	// @param     	theY
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	TRect					Inflate(_T theX, _T theY)
	{
		mX -= theX;
		mWidth += theX*2;
		mY -= theY;
		mHeight += theY*2;

		return *this;
	}

	bool operator==(const TRect<_T>& theRect) const
	{
		return (mX == theRect.mX) && (mY == theRect.mY) && (mWidth == theRect.mWidth) && (mHeight == theRect.mHeight);
	}
};

typedef TRect<int> SexyRect;
typedef TRect<double> SexyFRect;



}

#endif //__TRectANGLE_H__