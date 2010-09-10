// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __POINT_H__
#define __POINT_H__

namespace Sexy
{

template<class _T> class TPoint
{
public:
	_T						mX;
	_T						mY;

public:
	TPoint(_T theX, _T theY) : 
		mX(theX),
		mY(theY)
	{
	}

	TPoint(const TPoint<_T>& theTPoint) :
		mX(theTPoint.mX),
		mY(theTPoint.mY)
	{
	}

	TPoint() :
		mX(0),
		mY(0)
	{
	}

	inline bool operator==(const TPoint& p)
	{
		return ((p.mX == mX) && (p.mY == mY));
	}

	inline bool operator!=(const TPoint& p)
	{
		return ((p.mX != mX) || (p.mY != mY));
	}

	TPoint operator+(const TPoint& p) const {return TPoint(mX+p.mX, mY+p.mY);}
	TPoint operator-(const TPoint& p) const {return TPoint(mX-p.mX, mY-p.mY);}
	TPoint operator*(const TPoint& p) const {return TPoint(mX*p.mX, mY*p.mY);}
	TPoint operator/(const TPoint& p) const {return TPoint(mX/p.mX, mY/p.mY);}
	TPoint& operator+=(const TPoint& p)  {mX+=p.mX; mY+=p.mY; return *this;}
	TPoint& operator-=(const TPoint& p)  {mX-=p.mX; mY-=p.mY; return *this;}
	TPoint& operator*=(const TPoint& p)  {mX*=p.mX; mY*=p.mY; return *this;}
	TPoint& operator/=(const TPoint& p)  {mX/=p.mX; mY/=p.mY; return *this;}
	TPoint operator*(_T s) const {return TPoint(mX*s, mY*s);} 
	TPoint operator/(_T s) const {return TPoint(mX/s, mY/s);} 
};

typedef TPoint<int> SexyPoint;
typedef TPoint<double> SexyFPoint;

};

#endif //__POINT_H__