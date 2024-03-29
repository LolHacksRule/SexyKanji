// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyMatrix.h"

#include <math.h>

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyMatrix3::SexyMatrix3()
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyMatrix3::ZeroMatrix()
{
	m00 = m01 = m02 =
	m10 = m11 = m12 = 
	m20 = m21 = m22 = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyMatrix3::LoadIdentity()
{
	m01	= m02 = m10 = m12 = m20 = m21 = 0;
	m00 = m11 = m22 = 1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SexyMatrix3 SexyMatrix3::operator*(const SexyMatrix3 &theMat) const
{
	SexyMatrix3 aResult;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			float x = 0;
			for(int k=0; k<3; k++)				
				x += m[i][k]*theMat.m[k][j];

			aResult.m[i][j] = x;
		}
	}

	return aResult;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SexyVector2 SexyMatrix3::operator*(const SexyVector2 &theVec) const
{
	return SexyVector2(
		m00*theVec.x + m01*theVec.y + m02,
		m10*theVec.x + m11*theVec.y + m12);
}
	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
SexyVector3 SexyMatrix3::operator*(const SexyVector3 &theVec) const
{
	return SexyVector3(
		m00*theVec.x + m01*theVec.y + m02*theVec.z,
		m10*theVec.x + m11*theVec.y + m12*theVec.z,
		m20*theVec.x + m21*theVec.y + m22*theVec.z);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const SexyMatrix3& SexyMatrix3::operator*=(const SexyMatrix3 &theMat)
{
	return operator=(operator*(theMat));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyTransform2D::SexyTransform2D()
{
	LoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	
// @param     	loadIdentity
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyTransform2D::SexyTransform2D(bool loadIdentity)
{
	if (loadIdentity)
		LoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	
// @param     	theMatrix
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyTransform2D::SexyTransform2D(const SexyMatrix3& theMatrix) : SexyMatrix3(theMatrix)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const SexyTransform2D& SexyTransform2D::operator=(const SexyMatrix3 &theMat)
{
	SexyMatrix3::operator=(theMat);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	tx
// @param     	ty
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyTransform2D::Translate(float tx, float ty)
{
	SexyMatrix3 aMat;
	aMat.LoadIdentity();
	aMat.m02 = tx;
	aMat.m12 = ty;
	aMat.m22 = 1;

	*this = aMat * (*this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	rot
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyTransform2D::RotateRad(float rot)
{
	SexyMatrix3 aMat;
	aMat.LoadIdentity();

	float sinRot = -sinf(rot);
	float cosRot = cosf(rot);

	aMat.m00 = cosRot;
	aMat.m01 = -sinRot;
	aMat.m10 = sinRot;
	aMat.m11 = cosRot;

	*this = aMat * (*this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	rot
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyTransform2D::RotateDeg(float rot)
{
	RotateRad(3.1415926535897932384626433832795028841971f * rot / 180.0f);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	sx
// @param     	sy
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyTransform2D::Scale(float sx, float sy)
{
	SexyMatrix3 aMat;
	aMat.LoadIdentity();
	aMat.m00 = sx;
	aMat.m11 = sy;

	*this = aMat * (*this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Transform::Transform() : mMatrix(false)
{
	Reset();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::Reset()
{
	mNeedCalcMatrix = true;
	mComplex = false;
	mTransX1 = mTransY1 = 0;
	mTransX2 = mTransY2 = 0;
	mScaleX = mScaleY = 1;
	mRot = 0;
	mHaveRot = false;
	mHaveScale = false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	tx
// @param     	ty
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::Translate(float tx, float ty)
{
	if (!mComplex)
	{
		mNeedCalcMatrix = true;
		if (mHaveRot || mHaveScale)
		{
			mTransX2 += tx;
			mTransY2 += ty;
		}
		else
		{
			mTransX1 += tx;
			mTransY1 += ty;
		}
	}
	else
		mMatrix.Translate(tx,ty);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	rot
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::RotateRad(float rot)
{
	if (!mComplex)
	{
		if (mHaveScale)
		{
			MakeComplex();
			mMatrix.RotateRad(rot);
		}
		else 
		{
			mNeedCalcMatrix = true;
			mHaveRot = true;
			mRot += rot;
		}
	}
	else
		mMatrix.RotateRad(rot);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	rot
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::RotateDeg(float rot)
{
	RotateRad(3.1415926535897932384626433832795028841971f * rot / 180.0f);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
// @param     	sx
// @param     	sy
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::Scale(float sx, float sy)
{
	if (!mComplex)
	{
		if (mHaveRot || mTransX1!=0 || mTransY1!=0 || (sx<0 && mScaleX*sx!=-1) || sy<0)
		{
			MakeComplex();
			mMatrix.Scale(sx,sy);
		}
		else
		{
			mNeedCalcMatrix = true;
			mHaveScale = true;
			mScaleX *= sx;
			mScaleY *= sy;
		}
	}
	else
		mMatrix.Scale(sx,sy);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::MakeComplex()
{
	if (!mComplex)
	{
		mComplex = true;
		CalcMatrix();
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Transform::CalcMatrix() const
{
	if (mNeedCalcMatrix)
	{
		mNeedCalcMatrix = false;

		mMatrix.LoadIdentity();
		mMatrix.m02 = mTransX1;
		mMatrix.m12 = mTransY1;
		mMatrix.m22 = 1;

		if (mHaveScale)
		{
			mMatrix.m00 = mScaleX;
			mMatrix.m11 = mScaleY;
		}
		else if (mHaveRot)
			mMatrix.RotateRad(mRot);

		if (mTransX2!=0 || mTransY2!=0)
			mMatrix.Translate(mTransX2,mTransY2);
	}

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!***********************************
// @return    	const SexyTransform2D&
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
const SexyTransform2D& Transform::GetMatrix() const
{
	CalcMatrix();
	return mMatrix;
}

/*!***********************************
// @return    	double
// @param     	theRowHeightY
// @param     	theColumnWidthX
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
double Sexy::SexyTransform2D::determinantOfMinor( int theRowHeightY, int theColumnWidthX) const
{
	int x1 = theColumnWidthX == 0 ? 1 : 0;  /* always either 0 or 1 */
	int x2 = theColumnWidthX == 2 ? 1 : 2;  /* always either 1 or 2 */
	int y1 = theRowHeightY   == 0 ? 1 : 0;  /* always either 0 or 1 */
	int y2 = theRowHeightY   == 2 ? 1 : 2;  /* always either 1 or 2 */

	return ( m [y1] [x1]  *  m [y2] [x2] )
		-  ( m [y1] [x2]  *  m [y2] [x1] );
}

/*!***********************************
// @return    	double
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
double Sexy::SexyTransform2D::GetDeterminant() const
{
	return ( m [0] [0]  *  determinantOfMinor( 0, 0) )
		-  ( m [0] [1]  *  determinantOfMinor( 0, 1) )
		+  ( m [0] [2]  *  determinantOfMinor( 0, 2) );
}

/*!***********************************
// @return    	Sexy::SexyTransform2D
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyTransform2D Sexy::SexyTransform2D::Inverse() const
{
	SexyTransform2D aRetTrans(true);
	double det = GetDeterminant();

	if ( fabs(det) < 1e-2 )
		return aRetTrans;
	
	double oneOverDeterminant = 1.0 / det;

	for (   int y = 0;  y < 3;  y ++ )
	{
		for ( int x = 0;  x < 3;  x ++   )
		{
			/* Rule is Inverse = 1/det * minor of the TRANSPOSE matrix.  *
			* Note (y,x) becomes (x,y) INTENTIONALLY here!              */
			aRetTrans.m[y][x] = (float)(determinantOfMinor( x, y ) * oneOverDeterminant);

			/* (y0,x1)  (y1,x0)  (y1,x2)  and (y2,x1)  all need to be negated. */
			if( 1 == ((x + y) % 2) )
				aRetTrans.m[y][x] = - aRetTrans.m[y][x];
		}
	}

	return aRetTrans;
}

/*!***********************************
// @return    	void
// @param     	tx
// @param     	ty
//
// \brief		Translates by (tx,ty)
// \details 	<b>Description:</b> Translates by (tx,ty)
//************************************/
void Sexy::SexyKanjiTransform::Translate( float tx, float ty )
{
	if (!mComplex)
	{
		mNeedCalcMatrix = true;
		if (mHaveRot || mHaveScale)
		{
			mTransX2 += tx;
			mTransY2 += ty;
		}
		else
		{
			mTransX1 += tx;
			mTransY1 += ty;
		}
	}
	else
	{
		KMatrix aMat;
		aMat.translation(-tx,ty,0);
		mKMatrix = aMat * mKMatrix;
	}
}

/*!***********************************
// @return    	void
// @param     	rot
//
// \brief		Rotates the Transform by rot Radians.
// \details 	<b>Description:</b> Rotates the Transform by rot Radians.
//************************************/
void Sexy::SexyKanjiTransform::RotateRad( float rot )
{
	if (!mComplex)
	{
		if (mHaveScale)
		{
			MakeComplex();

			KMatrix aMat;
			aMat.rotationZ(rot);
			mKMatrix = aMat * mKMatrix;
		}
		else 
		{
			mNeedCalcMatrix = true;
			mHaveRot = true;
			mRot += rot;
		}
	}
	else
	{
		KMatrix aMat;
		aMat.rotationZ(rot);
		mKMatrix = aMat * mKMatrix;
	}
}

/*!***********************************
// @return    	void
// @param     	sx
// @param     	sy
//
// \brief		Scales the transform.  Use Negative values to mirror.
// \details 	<b>Description:</b> Scales the transform.  Use Negative values to mirror
				about the X or Y Axis.
//************************************/
void Sexy::SexyKanjiTransform::Scale( float sx, float sy )
{
	if (!mComplex)
	{
		if (mHaveRot || mTransX1!=0 || mTransY1!=0 || (sx<0 && mScaleX*sx!=-1) || sy<0)
		{
			MakeComplex();
			
			KMatrix aMat;
			aMat.scaling(sx,sy,1.0f);
			mKMatrix = aMat * mKMatrix;
		}
		else
		{
			mNeedCalcMatrix = true;
			mHaveScale = true;
			mScaleX *= sx;
			mScaleY *= sy;
		}
	}
	else
	{
		KMatrix aMat;
		aMat.scaling(sx,sy,1.0f);
		mKMatrix = aMat * mKMatrix;
	}
}

/*!***********************************
// @return    	const KMatrix&
//
// \brief		Calculates and then returns the KMatrix
// \details 	<b>Description:</b> Calculates and then returns the KMatrix
//************************************/
const KMatrix& Sexy::SexyKanjiTransform::GetKMatrix() const
{
	CalcMatrix();
	return mKMatrix;
}

/*!***********************************
// @return    	void
//
// \brief		Forces the Transform into 'MATRIX' mode.
// \details 	<b>Description:</b> 
//************************************/
void Sexy::SexyKanjiTransform::MakeComplex()
{
	if (!mComplex)
	{
		mComplex = true;
		CalcMatrix();
	}
}

/*!***********************************
// @return    	void
//
// \brief		Calculates the KMatrix
// \details 	<b>Description:</b> Calculates the KMatrix
//************************************/
void Sexy::SexyKanjiTransform::CalcMatrix() const
{
	if (mNeedCalcMatrix)
	{
		mNeedCalcMatrix = false;

		// translation resets the matrix and sets it as a translation
		mKMatrix.translation(-mTransX1, mTransY1, 0.0f);

		if (mHaveScale)
		{
			KMatrix aScaleMat;
			aScaleMat.scaling(mScaleX, mScaleY, 1.0f);
			mKMatrix = aScaleMat * mKMatrix;
		}
		else if (mHaveRot)
		{
			KMatrix aRotMat;
			aRotMat.rotationZ(mRot);
			mKMatrix = aRotMat * mKMatrix;
		}

		if (mTransX2!=0 || mTransY2!=0)
		{
			KMatrix aTransMat;
			aTransMat.translation(-mTransX2, mTransY2, 0.0);
			mKMatrix = aTransMat * mKMatrix;
		}
	}
}