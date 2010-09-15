// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	created:	2010/02/22
	created:	22:2:2010
	filename: 	SexyKanji\Graphics.h
	file base:	Graphics
	file ext:	h
	author:		Jeremy Sullivan / James Poag
	
	purpose:	SexyAppFramework Wrapper for Kanji Graphic drawing routines.
*********************************************************************/
#include "Graphics.h"

#include "Common.h"
#include "SexyFont.h"
#include "SexyImage.h"
#include "SexyTriVertex.h"
#include "SexyAppBase.h"
#include "Debug.h"

#include "KPTK.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

const Sexy::SexyPoint* Graphics::mPFPoints = NULL;
SexyImage* gCurrentRenderTarget = NULL;

/*!***********************************
// @return    	void
// @param     	theState
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void GraphicsState::CopyStateFrom(const GraphicsState* theState)
{
	mRenderTarget = theState->mRenderTarget;
	mLineGraphic = theState->mLineGraphic;
	mTransX = theState->mTransX;
	mTransY = theState->mTransY;
	mClipRect = theState->mClipRect;
	mFont = theState->mFont;
	mColor = theState->mColor;
	mDrawMode = theState->mDrawMode;
	mColorizeImages = theState->mColorizeImages;
	mLinearBlend = theState->mLinearBlend;
	mWriteColoredString = theState->mWriteColoredString;
	mLinearBlend = theState->mLinearBlend;
	mAccumTransStack = theState->mAccumTransStack;
}

/*!***********************************
// @return    	
// @param     	theGraphics
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Graphics::Graphics(const Graphics& theGraphics)
{
	CopyStateFrom(&theGraphics);

	// Create and allocate the batch vertex buffer.
	mBatch = KPTK::createKBatch();
	mBatch->allocateVertexBuffer(BATCH_BUFFER_SIZE, K_BATCH_DYNTL);
	mIsImageOwner = false;
}

/*!***********************************
// @return    	
// @param     	theRenderTarget
//
// \brief		Default CTor for Graphics.
// \details 	<b>Description:</b> 
//************************************/
Graphics::Graphics(SexyImage* theRenderTarget)
{
	mTransX = mTransY = 0.0f;
	mFont = NULL;
	mDrawMode = DRAWMODE_NORMAL;
	mIsImageOwner = mColorizeImages = mWriteColoredString = mLinearBlend = false;
	mLinearBlend = true;

	if (theRenderTarget != NULL && theRenderTarget->GetKanjiImage() != NULL)
	{
		mRenderTarget = theRenderTarget;
		mLineGraphic = mRenderTarget;
		mIsImageOwner = false;

		if(mRenderTarget->GetKanjiImage()->isRenderTargetLost())
		{
			int aWidth = mRenderTarget->GetWidth();
			int aHeight = mRenderTarget->GetHeight();
			mRenderTarget->GetKanjiImage()->makeRenderTarget(aWidth,aHeight, true);
			mRenderTarget->GetKanjiImage()->clearRenderTarget();
		}
	}
	else
	{
		// We need a KGraphic for line drawing and poly fills.
		mRenderTarget = NULL;
		mLineGraphic = new SexyImage();
		mIsImageOwner = true;
	}

	// Create and allocate the batch vertex buffer.
	mBatch = KPTK::createKBatch();
	mBatch->allocateVertexBuffer(BATCH_BUFFER_SIZE, K_BATCH_DYNTL);

	mClipRect = SexyRect(0, 0, Sexy::gSexyAppBase->GetWidth(), Sexy::gSexyAppBase->GetHeight());
}

/*!***********************************
// @return    	
//
// \brief		Destructor for Graphics
// \details 	<b>Description:</b> 
//************************************/
Graphics::~Graphics()
{
	if (mBatch)
	{
		mBatch->freeBuffer();
		delete mBatch;
		mBatch = NULL;
	}

	if (mIsImageOwner)
	{
		delete mLineGraphic;
		mIsImageOwner = false;
	}

	mLineGraphic = NULL;

	// we will never Own the render target
	mRenderTarget = NULL;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::PushState()
{
	mStateStack.push_back(GraphicsState());
	mStateStack.back().CopyStateFrom(this);
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::PopState()
{
	if (mStateStack.size() > 0)
	{
		CopyStateFrom(&mStateStack.back());
		mStateStack.pop_back();
	}
}

/*!***********************************
// @return    	void
// @param     	theFont
//
// \brief		Set the Font to use to draw text.
// \details 	<b>Description:</b> Set the Font to use to draw text.
//************************************/
void Graphics::SetFont(Sexy::SexyFont* theFont)
{
	mFont = theFont;
}

/*!***********************************
// @return    	Sexy::SexyFont*
//
// \brief		Get a pointer to the current Font being used to draw text.
// \details 	<b>Description:</b>Get a pointer to the current Font being used to draw text. 
//************************************/
Sexy::SexyFont* Graphics::GetFont()
{
	return mFont;
}

/*!***********************************
// @return    	void
// @param     	theColor
//
// \brief		Set the SexyColor to use when drawing text or colorized images.
// \details 	<b>Description:</b>Set the SexyColor to use when drawing text or colorized images. 
//************************************/
void Graphics::SetColor(const SexyColor& theColor)
{
	mColor = theColor;
}

/*!***********************************
// @return    	const SexyColor&
//
// \brief		Get a const reference to the current SexyColor being used 
// to draw text or colorized images.
// \details 	<b>Description:</b> 
//************************************/
const SexyColor& Graphics::GetColor()
{
	return mColor;
}

/*!***********************************
// @return    	void
// @param     	theDrawMode
//
// \brief		Set the Draw Mode (DRAWMODE_NORMAL or DRAWMODE_ADDITIVE).
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetDrawMode(int theDrawMode)
{
	mDrawMode = theDrawMode;
}

/*!***********************************
// @return    	int
//
// \brief		Get the Draw Mode (DRAWMODE_NORMAL or DRAWMODE_ADDITIVE).
// \details 	<b>Description:</b> 
//************************************/
int Graphics::GetDrawMode()
{
	return mDrawMode;
}

/*!***********************************
// @return    	void
// @param     	colorizeImages
//
// \brief		Set: should images be colorized when drawn?
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetColorizeImages(bool colorizeImages)
{
	mColorizeImages = colorizeImages;
}

/*!***********************************
// @return    	bool
//
// \brief		Get: will images be colorized when drawn?
// \details 	<b>Description:</b> 
//************************************/
bool Graphics::GetColorizeImages()
{
	return mColorizeImages;
}

/*!***********************************
// @return    	void
// @param     	writeColoredStrings
//
// \brief		Set: will text be colorized when drawn with the WriteString call?
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetWriteColoredStrings(bool writeColoredStrings)
{
	mWriteColoredString = writeColoredStrings;
}

/*!***********************************
// @return    	bool
//
// \brief		Get: will text be colorized when drawn with the WriteString call?
// \details 	<b>Description:</b> 
//************************************/
bool Graphics::GetWriteColoredStrings()
{
	return mWriteColoredString;
}

/*!***********************************
// @return    	void
// @param     	bTurnOnLinearBlending
//
// \brief		Set: Draw fast instead of high quality (filtering).
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetLinearBlend(bool bTurnOnLinearBlending)
{
	mLinearBlend = bTurnOnLinearBlending;
}

/*!***********************************
// @return    	bool
//
// \brief		Get: Draw fast instead of high quality (filtering).
// \details 	<b>Description:</b> 
//************************************/
bool Graphics::GetLinearBlend()
{
	return mLinearBlend;
}

/*!***********************************
// @return    	void
// @param     	theTransform
//
// \brief		Sets the Absolute Transform for the World
// \details 	<b>Description:</b> This method overrides previously
				set transfroms with theTransfrom.  To combine
				transforms, look into Push/Pop Transform.
//************************************/
void Graphics::SetTransform(const SexyKanjiTransform& theTransform)
{
	ClearTransforms();

	mAccumTransStack.push(theTransform.GetKMatrix());
}
/*!***********************************
// @return    	void
// @param     	theTransX
// @param     	theTransY
//
// \brief		Set the image translation offset.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::Translate(int theTransX, int theTransY)
{
	mTransX += static_cast<float>(theTransX);
	mTransY += static_cast<float>(theTransY);
}

/*!***********************************
// @return    	void
// @param     	theTransX
// @param     	theTransY
//
// \brief		Set the image translation offset (float version).
// \details 	<b>Description:</b> 
//************************************/
void Graphics::TranslateF(float theTransX, float theTransY)
{
	mTransX += theTransX;
	mTransY += theTransY;
}

/*!***********************************
// @return    	void
//
// \brief		Clear current clip rect. Resets to default clip rect.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::ClearClipRect()
{
	if (mRenderTarget)
		mClipRect = SexyRect(0, 0, static_cast<int>(mRenderTarget->GetKanjiImage()->getWidth()), static_cast<int>(mRenderTarget->GetKanjiImage()->getHeight()));
	else
		mClipRect = SexyRect(0,0, gSexyAppBase->GetWidth(), gSexyAppBase->GetHeight());
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		Set the clip rect.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetClipRect(int theX, int theY, int theWidth, int theHeight)
{
	if (mRenderTarget)
	{
		mClipRect = SexyRect(0, 0, static_cast<int>(mRenderTarget->GetKanjiImage()->getWidth()), 
			static_cast<int>(mRenderTarget->GetKanjiImage()->getHeight())).Intersection(
			SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY), theWidth, theHeight));
	}
	else
	mClipRect = SexyRect(0, 0, gSexyAppBase->GetWidth(),gSexyAppBase->GetHeight()).Intersection(
				SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY), theWidth, theHeight));
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		Set the clip rect.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::SetClipRect(const SexyRect& theRect)
{
	SetClipRect(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		Clip the clip rect.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::ClipRect(int theX, int theY, int theWidth, int theHeight)
{
	mClipRect = mClipRect.Intersection(SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY), theWidth, theHeight));
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		Clip the clip rect.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::ClipRect(const SexyRect& theRect)
{
	ClipRect(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theStartX
// @param     	theStartY
// @param     	theEndX
// @param     	theEndY
//
// \brief		Draw a line using the passed coords with the current color.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawLine(int theStartX, int theStartY, int theEndX, int theEndY)
{
	if (mLineGraphic == NULL || mColor.mAlpha <= 0) return;

	PreDraw();

	float aStartX = theStartX + mTransX;
	float aStartY = theStartY + mTransY;
	float aEndX = theEndX + mTransX;
	float aEndY = theEndY + mTransY;

	if (!DrawLineClipHelper(&aStartX, &aStartY, &aEndX, &aEndY))
		return;

	mLineGraphic->GetKanjiImage()->setTextureQuality(false);
	mLineGraphic->GetKanjiImage()->drawLine(aStartX, aStartY, aEndX, aEndY, 
		static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f,
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		Draw an outline rect using the passed rect with the current color.
// \details 	<b>Description:</b> Draw an outline rect using the passed rect with the current color.
//************************************/
void Graphics::DrawRect(int theX, int theY, int theWidth, int theHeight)
{
	if (mLineGraphic == NULL || mColor.mAlpha <= 0) return;

	PreDraw();

	// Kanji doesn't have an 'outline' rect, so we'll just draw lines.
	DrawLine(theX, theY, theX + theWidth, theY);
	DrawLine(theX + theWidth, theY, theX + theWidth, theY + theHeight);
	DrawLine(theX + theWidth, theY + theHeight, theX, theY + theHeight);
	DrawLine(theX, theY + theHeight, theX, theY);
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		Draw an outline rect using the passed rect with the current color.
// \details 	<b>Description:</b> Draw an outline rect using the passed rect with the current color.
//************************************/
void Graphics::DrawRect(const SexyRect& theRect)
{
	DrawRect(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		Fill the passed rect with the current color.
// \details 	<b>Description:</b> Fill the passed rect with the current color.
//************************************/
void Graphics::FillRect(int theX, int theY, int theWidth, int theHeight)
{
	if (mLineGraphic == NULL || mColor.mAlpha <= 0) return;

	PreDraw();

	SexyRect aDestRect = SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY),
		theWidth, theHeight).Intersection(mClipRect);

	mLineGraphic->GetKanjiImage()->drawRect(
		static_cast<float>(aDestRect.mX), 
		static_cast<float>(aDestRect.mY),
		static_cast<float>(aDestRect.mX + aDestRect.mWidth),
		static_cast<float>(aDestRect.mY + aDestRect.mHeight),
		static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f,
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f
		);
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		Fill the passed rect with the current color.
// \details 	<b>Description:</b> Fill the passed rect with the current color. <b>SLOW!!!</b>
//************************************/
void Graphics::FillRect(const SexyRect& theRect)
{
	FillRect(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theHeight
//
// \brief		
// \details 	<b>Description:</b> Clear the passed rect. For us this means
// fill the rect with Black.
//************************************/
void Graphics::ClearRect(int theX, int theY, int theWidth, int theHeight)
{
	// Save the current color.
	Sexy::SexyColor aTempColor = mColor;

	// Fill rect with Black.
	mColor = Sexy::SexyColor(0, 0, 0, 255);
	FillRect(theX, theY, theWidth, theHeight);

	// Restore the original color.
	mColor = aTempColor;
}

/*!***********************************
// @return    	void
// @param     	theRect
//
// \brief		Clear the passed rect. For us this means
// fill the rect with Black.
// \details 	<b>Description:</b> Clear the passed rect. For us this means
// fill the rect with Black.
//************************************/
void Graphics::ClearRect(const SexyRect& theRect)
{
	ClearRect(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

/*!***********************************
// @return    	void
// @param     	theVertexList
// @param     	theNumVertices
//
// \brief		Draw a filled polygon given the points.
// \details 	<b>Description:</b> Slow!
//************************************/
void Graphics::PolyFill(const SexyPoint *theVertexList, int theNumVertices)
{
	if (mLineGraphic == NULL) return;

	PreDraw();

	int i;
	Span aSpans[MAX_TEMP_SPANS];
	int aSpanPos = 0;

	static uchar aCoverageBuffer[256*256];
	int aCoverWidth = 256, aCoverHeight = 256; 
	int aCoverLeft, aCoverRight, aCoverTop, aCoverBottom;

	for (i = 0; i < theNumVertices; ++i)
	{
		const SexyPoint* aPt = &theVertexList[i];
		if (i == 0)
		{
			aCoverLeft = aCoverRight = aPt->mX;
			aCoverTop = aCoverBottom = aPt->mY;
		}
		else
		{
			aCoverLeft = K_MIN(aCoverLeft, aPt->mX);
			aCoverRight = K_MAX(aCoverRight, aPt->mX);
			aCoverTop = K_MIN(aCoverTop, aPt->mY);
			aCoverBottom = K_MAX(aCoverBottom, aPt->mY);
		}
	}
	uchar* coverPtr = aCoverageBuffer;
	if ((aCoverRight-aCoverLeft+1) > aCoverWidth || (aCoverBottom-aCoverTop+1) > aCoverHeight)
	{
		aCoverWidth = aCoverRight-aCoverLeft+1;
		aCoverHeight = aCoverBottom-aCoverTop+1;
		coverPtr = new uchar[aCoverWidth*aCoverHeight];
	}
	memset(coverPtr, 0, aCoverWidth*aCoverHeight);

    int k, y0, y1, y, j, xl, xr;
    int *ind;		/* list of vertex indices, sorted by mPFPoints[ind[j]].y */		

	int aMinX = mClipRect.mX;
	int aMaxX = mClipRect.mX + mClipRect.mWidth - 1;
	int aMinY = mClipRect.mY;
	int aMaxY = mClipRect.mY + mClipRect.mHeight - 1;

    mPFNumVertices = theNumVertices;
    mPFPoints = theVertexList;
    
	if (mPFNumVertices<=0) return;

    ind = new int[mPFNumVertices];
    mPFActiveEdgeList = new Edge[mPFNumVertices];

    /* create y-sorted array of indices ind[k] into vertex list */
    for (k=0; k<mPFNumVertices; k++)
		ind[k] = k;
	std::qsort(ind, mPFNumVertices, sizeof ind[0], PFCompareInd);	/* sort ind by mPFPoints[ind[k]].y */

    mPFNumActiveEdges = 0;				/* start with empty active list */
    k = 0;				/* ind[k] is next vertex to process */
    y0 = (int) K_MAX(aMinY, ceil(mPFPoints[ind[0]].mY-0.5 + mTransY));		/* ymin of polygon */
    y1 = (int) K_MIN(aMaxY, floor(mPFPoints[ind[mPFNumVertices-1]].mY-0.5 + mTransY));	/* ymax of polygon */

    for (y=y0; y<=y1; y++) 
	{
		// step through scanlines 
		// scanline y is at y+.5 in continuous coordinates 

		// check vertices between previous scanline and current one, if any 
		for (; (k < mPFNumVertices) && (mPFPoints[ind[k]].mY + mTransY <= y + 0.5); k++) 
		{
			// to simplify, if mPFPoints.mY=y+.5, pretend it's above 
			// invariant: y-.5 < mPFPoints[i].mY <= y+.5 
			i = ind[k];				
			// insert or delete edges before and after vertex i (i-1 to i,
			// and i to i+1) from active list if they cross scanline y			 

			j = i>0 ? i-1 : mPFNumVertices-1;	// vertex previous to i 
			if (mPFPoints[j].mY + mTransY <= y-0.5)	// old edge, remove from active list 
				PFDelete(j);
			else if (mPFPoints[j].mY + mTransY > y+0.5)	// new edge, add to active list 
				PFInsert(j, y);

			j = i<mPFNumVertices-1 ? i+1 : 0;	// vertex next after i 
			if (mPFPoints[j].mY + mTransY <= y-0.5)	// old edge, remove from active list 
				PFDelete(i);
			else if (mPFPoints[j].mY + mTransY > y+0.5)	// new edge, add to active list 
				PFInsert(i, y);
		}

		// sort active edge list by active[j].mX 
		qsort(mPFActiveEdgeList, mPFNumActiveEdges, sizeof mPFActiveEdgeList[0], PFCompareActive);

		// draw horizontal segments for scanline y 
		for (j = 0; j < mPFNumActiveEdges; j += 2) 
		{	// draw horizontal segments 
			// span 'tween j & j+1 is inside, span tween j+1 & j+2 is outside 
			xl = (int) ceil(mPFActiveEdgeList[j].mX-0.5);		// left end of span 
			int lErr = int((fabs((mPFActiveEdgeList[j].mX-0.5) - xl)) * 255);
			if (xl<aMinX)
			{
				xl = aMinX;
				lErr = 255;
			}
			xr = (int) floor(mPFActiveEdgeList[j+1].mX-0.5);	// right end of span 
			int rErr = int((fabs((mPFActiveEdgeList[j+1].mX-0.5) - xr)) * 255);
			if (xr>aMaxX) 
			{
				xr = aMaxX;
				rErr = 255;
			}
			
			if ((xl <= xr) && (aSpanPos < MAX_TEMP_SPANS))
			{
				Span* aSpan = &aSpans[aSpanPos++];
				aSpan->mY = y;
				aSpan->mX = xl;
				aSpan->mWidth = xr - xl + 1;

				uchar* coverRow = coverPtr + (y - aCoverTop) * aCoverWidth;
				if (xr == xl)
				{
					coverRow[xl-aCoverLeft] = K_MIN(255, coverRow[xl-aCoverLeft] + ((lErr*rErr)>>8));
				}
				else
				{
					if (fabs(mPFActiveEdgeList[j].mDX) > 1.0f) // mostly horizontal on the left edge
					{
						double m = 1.0 / mPFActiveEdgeList[j].mDX, 
								b = mPFActiveEdgeList[j].b, 
								c = fabs(mPFActiveEdgeList[j].mDX);
						do
						{
							double _y =	m * xl + b;
							lErr = K_MIN(255, int(fabs((_y) - y - .5) * 255));
							coverRow[xl-aCoverLeft] = K_MIN(255, coverRow[xl-aCoverLeft] + lErr);
							xl++;
							c -= 1.0;
						} while (xl <= xr && c > 0);
					}
					else
					{
						coverRow[xl-aCoverLeft] = K_MIN(255, coverRow[xl-aCoverLeft] + lErr);
						xl++;
					}

					if (fabs(mPFActiveEdgeList[j+1].mDX) > 1.0f) // mostly horizontal on the right edge
					{
						double m = 1.0 / mPFActiveEdgeList[j+1].mDX, 
								b = mPFActiveEdgeList[j+1].b, 
								c = fabs(mPFActiveEdgeList[j+1].mDX);
						do
						{
							double _y =	m * xr + b;
							rErr = K_MIN(255, int(fabs((_y) - y - .5) * 255));
							coverRow[xr-aCoverLeft] = K_MIN(255, coverRow[xr-aCoverLeft] + rErr);
							xr--;
							c -= 1.0;
						} while (xr >= xl && c > 0);
					}
					else
					{
						coverRow[xr-aCoverLeft] = K_MIN(255, coverRow[xr-aCoverLeft] + rErr);
						xr--;
					}

					if (xl <= xr)
						memset(&coverRow[xl-aCoverLeft], 255, xr-xl+1);
				}
			}			
			
			mPFActiveEdgeList[j].mX += mPFActiveEdgeList[j].mDX;	// increment edge coords 
			mPFActiveEdgeList[j+1].mX += mPFActiveEdgeList[j+1].mDX;
		}
	}

	if(mColorizeImages)
		mLineGraphic->GetKanjiImage()->setBlitColor(static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f, 
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);
	else
		mLineGraphic->GetKanjiImage()->setBlitColor(1, 1, 1, 1);
	
	mLineGraphic->GetKanjiImage()->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);
	mLineGraphic->GetKanjiImage()->setTextureQuality(mLinearBlend);
	
	for (int i = 0; i < aSpanPos; ++i)
	{
		Span* aSpan = &aSpans[i];
		DrawLine(aSpan->mX, aSpan->mY, aSpan->mX + aSpan->mWidth, aSpan->mY);
	}

	if (coverPtr != aCoverageBuffer) delete[] coverPtr;
	delete[] ind;
	delete[] mPFActiveEdgeList;
}

/*!***********************************
// @return    	void
// @param     	theString
// @param     	theX
// @param     	theY
//
// \brief		Draw the passed string at the specified x and y screen coords.
// \details 	<b>Description:</b> Draw the passed string at the specified x and y screen coords.
//************************************/
void Graphics::DrawString(const SexyString& theString, int theX, int theY)
{
	if (mFont == NULL || theString.empty() || mFont->GetKTextFace() == NULL) return;
	PreDraw();

	mFont->GetKTextFace()->setBlitColor(static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f, 
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);

	mFont->GetKTextFace()->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);

	SexyRect aDestRect = SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY),
		mFont->StringWidth(theString), mFont->GetHeight());

	if ((aDestRect.mWidth > 0) && (aDestRect.mHeight > 0))
	{
		mFont->GetKTextFace()->drawStringFromLeftF(reinterpret_cast<const char*>(theString.c_str()), 
			(float)theX + (mTransX),(float)theY + (mTransY), 0, 1, SexyStrEncoding);
	}
}
/*!***********************************
// @return    	int
// @param     	theLine
// @param     	theX
// @param     	theY
// @param     	theOldColor
//
// \brief		Draw the passed string at the specified x and y screen coords.
// \details 	<b>Description:</b>Draw the passed string at the specified x and y screen coords. 
//************************************/
int	Graphics::DrawStringColor(const SexyString& theLine, int theX, int theY, int theOldColor)
{
	if (mFont == NULL) return 0;
	PreDraw();

	// Save the current mWriteColoredString state and set it to true.
	bool current = mWriteColoredString;
	mWriteColoredString = true;

	int anXOffset = WriteString(theLine, theX, theY, -1, -1, true, 0, -1, theOldColor);

	// Restore the mWriteColoredString state.
	mWriteColoredString = current;

	// return the x offset.
	return anXOffset;
}

/*!***********************************
// @return    	void
// @param     	theString
// @param     	theX
// @param     	theY
//
// \brief		Draw the passed string at the specified x and y screen coords.
// \details 	<b>Description:</b> Draw the passed string at the specified x and y screen coords.
//************************************/
void Graphics::DrawStringClipped(const SexyString& theString, int theX, int theY)
{
	if (mFont == NULL || theString.empty() || mFont->GetKTextFace() == NULL) return;
	PreDraw();

	mFont->GetKTextFace()->setBlitColor(static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f, 
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);

	mFont->GetKTextFace()->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);

	SexyRect aDestRect = SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY),
		mFont->StringWidth(theString), mFont->GetAscent());

	mFont->GetKTextFace()->drawStringClippedF(reinterpret_cast<const char*>(theString.c_str()), 
		(float)theX + (mTransX),(float)theY + (mTransY),
		static_cast<float>(aDestRect.mX + aDestRect.mWidth), static_cast<float>(aDestRect.mY + aDestRect.mHeight), 0, 0, 
		0, -1, 0, 0, SexyStrEncoding);
}

/*!***********************************
// @return    	int
// @param     	theString
// @param     	theX
// @param     	theY
// @param     	theWidth
// @param     	theJustification
// @param     	drawString
// @param     	theOffset
// @param     	theLength
// @param     	theOldColor
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Graphics::WriteString(const SexyString& theString, int theX, int theY, int theWidth, int theJustification, bool drawString, int theOffset, int theLength, int theOldColor)
{
	if (mFont == NULL) return 0;
	PreDraw();

	if (theOldColor == -1)
		theOldColor = mColor.ToInt();
	
	if (drawString)
	{
		switch (theJustification)
		{	
		case CENTER_JUSTIFIED :
			theX += (theWidth - WriteString(theString, theX, theY, theWidth, -1, false, theOffset, theLength, theOldColor))/2;
			break;
		case RIGHT_JUSTIFIED:
			theX += theWidth - WriteString(theString, theX, theY, theWidth, -1, false, theOffset, theLength, theOldColor);
			break;
		}
	}

	if(theLength < 0 || theOffset + theLength > (int)theString.length())
		theLength = (int)theString.length();
	else
		theLength = theOffset + theLength;


	SexyString aString;
	int aXOffset = 0;

	for (int i = theOffset; i < theLength; i++)
	{
		if ((theString[i] == '^') && mWriteColoredString)
		{
			if (i+1<theLength && theString[i+1] == '^') // literal '^'
			{
				aString += (L'^');
				i++;
			}
			else if (i>theLength-8) // badly formatted color specification
				break;
			else // change color instruction
			{
				DWORD aColor = 0;
				if (theString[i+1]== (L'o'))
				{
					if (std::sexystrncmp(theString.c_str()+i+1, (_S("oldclr")), 6) == 0)
						aColor = theOldColor;
				}
				else
				{
					for (int aDigitNum = 0; aDigitNum < 6; aDigitNum++)
					{
						wchar_t aChar = theString[i+aDigitNum+1];
						int aVal = 0;

						if ((aChar >= (L'0')) && (aChar <= (L'9')))
							aVal = aChar - (L'0');
						else if ((aChar >= (L'A')) && (aChar <= (L'F')))
							aVal = (aChar - (L'A')) + 10;
						else if ((aChar >= (L'a')) && (aChar <= (L'f')))
							aVal = (aChar - (L'a')) + 10;

						aColor += (aVal << ((5 - aDigitNum) * 4));
					}				
				}

				if (drawString)
				{
					DrawString(aString, theX + aXOffset, theY);
					SetColor(SexyColor((aColor >> 16) & 0xFF, (aColor >> 8) & 0xFF, (aColor) & 0xFF, GetColor().mAlpha));					
				}

				i += 7;

				aXOffset += mFont->StringWidth(aString);

				aString = _S("");
			}
		}
		else
			aString += theString[i];
	}

	if (drawString)
	{
		DrawString(aString, theX + aXOffset, theY);
	}

	aXOffset += mFont->StringWidth(aString);

	return aXOffset;
}

/*!***********************************
// @return    	int
// @param     	theString
// @param     	theRect
// @param     	theLineSpacing
// @param     	theJustification
//
// \brief		
// \details 	<b>Description:</b> Get the height of the passed string if drawn within the specified rect.
// Does NOT actually draw anything to screen!
// Valid values for Justification are:
//		LEFT_JUSTIFIED = -1,
//		CENTER_JUSTIFIED = 0,
//		RIGHT_JUSTIFIED = 1
// Returns height the text would be if it was written word wrapped.
//************************************/
int Graphics::GetWordWrappedHeight(const SexyString& theString, const SexyRect& theRect, int theLineSpacing, int theJustification)
{
	if (mFont == NULL) return 0;

	return WriteWordWrapped(theString, theRect.mX, theRect.mY, theRect.mWidth, theLineSpacing, theJustification, false);
}

/*!***********************************
// @return    	int
// @param     	theString
// @param     	theWrapWidth
// @param     	theLineSpacing
// @param     	theJustification
//
// \brief		Get the height of the passed string if drawn at the specified 
// screen coords with a width of theWrapWidth.
// \details 	<b>Description:</b> Get the height of the passed string if drawn at the specified 
// screen coords with a width of theWrapWidth.
// Does NOT actually draw anything to screen!
// Valid values for Justification are:
//		LEFT_JUSTIFIED = -1,
//		CENTER_JUSTIFIED = 0,
//		RIGHT_JUSTIFIED = 1
// Returns height the text would be if it was written word wrapped.
//************************************/
int Graphics::GetWordWrappedHeight(const SexyString& theString, int theWrapWidth, int theLineSpacing, int theJustification)
{
	if (mFont == NULL) return 0;

	return WriteWordWrapped(theString, 0, 0, theWrapWidth, theLineSpacing, theJustification, false);
}

static int WriteWordWrappedHelper(Graphics *g, const SexyString& theString, int theX, int theY, int theWidth, int theJustification, bool drawString, int theOffset, int theLength, int theOldColor, int theMaxChars)
{
	if (theOffset+theLength>theMaxChars)
	{
		theLength = theMaxChars-theOffset;
		if (theLength<=0)
			return -1;
	}

	return g->WriteString(theString,theX,theY,theWidth,theJustification,drawString,theOffset,theLength,theOldColor);
}

/*!***********************************
// @return    	int
// @param     	theRect
// @param     	theString
// @param     	theLineSpacing
// @param     	theJustification
// @param     	doBlit
//
// \brief		Draw the passed string within the specified rect.
// \details 	<b>Description:</b> Draw the passed string within the specified rect.
// Valid values for Justification are:
//		LEFT_JUSTIFIED = -1,
//		CENTER_JUSTIFIED = 0,
//		RIGHT_JUSTIFIED = 1
// Returns height of the written text.
//************************************/
int Graphics::WriteWordWrapped(const SexyRect& theRect, const SexyString& theLine, int theLineSpacing, int theJustification, bool doBlit)
{
	SexyColor anOrigColor = GetColor();
	int anOrigColorInt = anOrigColor.ToInt();
	if ((anOrigColorInt&0xFF000000)==0xFF000000)
		anOrigColorInt &= ~0xFF000000;

	int theMaxChars = (int)theLine.length();

	SexyFont* aFont = GetFont();

	int aYOffset = aFont->GetAscent() - aFont->GetAscentPadding();

	if (theLineSpacing == -1)
		theLineSpacing = aFont->GetLineSpacing();

	SexyString aCurString;
	ulong aCurPos = 0;
	int aLineStartPos = 0;
	int aCurWidth = 0;
	SexyChar aCurChar = 0;
	SexyChar aPrevChar = 0;
	int aSpacePos = -1;
	int aMaxWidth = 0;
	int anIndentX = 0;

	while (aCurPos < theLine.length())
	{
		aCurChar = theLine[aCurPos];
		if(aCurChar==_S('^') && mWriteColoredString) // Handle special color modifier
		{
			if(aCurPos+1<theLine.length())
			{
				if(theLine[aCurPos+1]==_S('^'))
					aCurPos++; // literal '^' -> just skip the extra '^'
				else
				{
					aCurPos+=8;
					continue; // skip the color specifier when calculating the width
				}
			}
		}
		else if(aCurChar==_S(' '))
			aSpacePos = aCurPos;
		else if(aCurChar==_S('\n'))
		{
			aCurWidth = theRect.mWidth+1; // force word wrap
			aSpacePos = aCurPos;
			aCurPos++; // skip enter on next go round
		}

		aCurWidth += aFont->CharWidth(aCurChar);
		aPrevChar = aCurChar;

		if(aCurWidth > theRect.mWidth) // need to wrap
		{
			int aWrittenWidth;
			if(aSpacePos!=-1)
			{
				int aPhysPos = theRect.mY + aYOffset + (int)mTransY;
				if ((aPhysPos >= mClipRect.mY) && (aPhysPos < mClipRect.mY + mClipRect.mHeight + theLineSpacing))
				{
					WriteWordWrappedHelper(this, theLine, theRect.mX + anIndentX, theRect.mY + aYOffset, theRect.mWidth,
						theJustification, doBlit, aLineStartPos, aSpacePos-aLineStartPos, anOrigColorInt, theMaxChars);
				}

				aWrittenWidth = aCurWidth + anIndentX;

				if (aWrittenWidth<0)
					break;

				aCurPos = aSpacePos+1;
				if (aCurChar != _S('\n'))
				{
					while (aCurPos<theLine.length() && theLine[aCurPos]==_S(' '))
						aCurPos++;
				}
				aLineStartPos = aCurPos;
			}
			else
			{
				if((int)aCurPos<aLineStartPos+1)
					aCurPos++; // ensure at least one character gets written

				aWrittenWidth = WriteWordWrappedHelper(this, theLine, theRect.mX + anIndentX, theRect.mY + aYOffset, theRect.mWidth,
					theJustification, doBlit, aLineStartPos, aCurPos-aLineStartPos, anOrigColorInt, theMaxChars);

				if (aWrittenWidth<0)
					break;
			}

			if (aWrittenWidth > aMaxWidth)
				aMaxWidth = aWrittenWidth;

			aLineStartPos = aCurPos;
			aSpacePos = -1;
			aCurWidth = 0;
			aPrevChar = 0;
			anIndentX = 0;
			aYOffset += theLineSpacing;
		}
		else
			aCurPos++;
	}

	if(aLineStartPos<(int)theLine.length()) // write the last piece
	{
		int aWrittenWidth = WriteWordWrappedHelper(this, theLine, theRect.mX + anIndentX, theRect.mY + aYOffset, theRect.mWidth,
			theJustification, doBlit, aLineStartPos, theLine.length()-aLineStartPos, anOrigColorInt, theMaxChars);

		if (aWrittenWidth>=0)
		{
			if (aWrittenWidth > aMaxWidth)
				aMaxWidth = aWrittenWidth;

			aYOffset += theLineSpacing;
		}
	}
	else if (aCurChar == '\n')
	{
		aYOffset += theLineSpacing;
	}

	SetColor(anOrigColor);

	return aYOffset + aFont->GetDescent() - theLineSpacing;
}
/*!***********************************
// @return    	int
// @param     	theString
// @param     	theX
// @param     	theY
// @param     	theWrapWidth
// @param     	theLineSpacing
// @param     	theJustification
// @param     	doBlit
//
// \brief		Draw the passed string at the specified screen coords with a width of theWrapWidth.
// \details 	<b>Description:</b> Draw the passed string at the specified screen coords with a width of theWrapWidth.
// Valid values for Justification are:
//		LEFT_JUSTIFIED = -1,
//		CENTER_JUSTIFIED = 0,
//		RIGHT_JUSTIFIED = 1
// Returns height of the written text.
//************************************/
int Graphics::WriteWordWrapped(const SexyString& theString, int theX, int theY, int theWrapWidth, int theLineSpacing, int theJustification, bool doBlit)
{
	if (mFont == NULL) return 0;
	PreDraw();

	KTextFace* aKTextFace = mFont->GetKTextFace();
	if (aKTextFace == NULL) return 0;

	// Save the original setting.
	float aTempSpacing = aKTextFace->getYSpacing();
	if (theLineSpacing != -1)
	{
		aKTextFace->setYSpacing(static_cast<float>(theLineSpacing));
	}
	else
	{
		aKTextFace->setYSpacing(static_cast<float>(mFont->GetAscent()));
	}

	aKTextFace->setBlitColor(static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f, 
		static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);

	aKTextFace->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);

	switch (theJustification)
	{
	case LEFT_JUSTIFIED :
		theJustification = TEXTSTYLE_LEFT ;
		break;
	case CENTER_JUSTIFIED :
		theJustification = TEXTSTYLE_CENTER ;
		break;
	case RIGHT_JUSTIFIED :
		theJustification = TEXTSTYLE_RIGHT ;
		break;
	default :
		theJustification = TEXTSTYLE_LEFT ;
	}

	long aHeight = aKTextFace->getMultilineHeight(reinterpret_cast<const char*>(SexyStringToWStringFast(theString).c_str()), theX, theX + theWrapWidth, theY, theJustification);

	if (doBlit)
	{
		SexyRect aClipRect = SexyRect(theX + static_cast<int>(mTransX), theY + static_cast<int>(mTransY),
			theWrapWidth, aHeight);

		aKTextFace->drawMultiline(reinterpret_cast<const char*>(SexyStringToWStringFast(theString).c_str()), aClipRect.mX, aClipRect.mX + theWrapWidth, aClipRect.mY, theJustification);
	}

	// Reset the original setting.
	aKTextFace->setYSpacing(aTempSpacing);

	return aHeight + mFont->GetAscent();
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
//
// \brief		Draw the passed Image at the specified x and y screen coords.
// \details 	<b>Description:</b> Draw the passed Image at the specified x and y screen coords.
//************************************/
void Graphics::DrawImage(SexyImage* theImage, int theX, int theY)
{
	if (theImage == NULL) return;

	// We only draw with floats, so just pass it to the float version (JSULLIVAN)
	DrawImageF(theImage, static_cast<float>(theX), static_cast<float>(theY));
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theSrcRect
//
// \brief		
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// at the specified x and y screen coords.
//************************************/
void Graphics::DrawImage(SexyImage* theImage, int theX, int theY, const SexyRect& theSrcRect)
{
	if (theImage == NULL) return;

	// We only draw with floats, so just pass it to the float version (JSULLIVAN)
	DrawImageF(theImage, static_cast<float>(theX), static_cast<float>(theY), theSrcRect);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theDestRect
// @param     	theSrcRect
//
// \brief		
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// in the specified theDestRect, stretching to fit if necessary.
//************************************/
void Graphics::DrawImage(SexyImage* theImage, const SexyRect& theDestRect, const SexyRect& theSrcRect)
{
	if (theImage == NULL) return;
	PreDraw();

	Sexy::Transform aTransform;

	if ((theDestRect.mWidth != theSrcRect.mWidth) || (theDestRect.mHeight != theSrcRect.mHeight))
	{
		float anXScale = static_cast<float>(theDestRect.mWidth) / theSrcRect.mWidth;
		float aYScale = static_cast<float>(theDestRect.mHeight) / theSrcRect.mHeight;
		aTransform.Scale(anXScale, aYScale);
	}

	aTransform.Translate(static_cast<float>(theDestRect.mWidth / 2), static_cast<float>(theDestRect.mHeight / 2));
	DrawImageMatrix(theImage, aTransform.GetMatrix(), theSrcRect, theDestRect, 
		static_cast<float>(theDestRect.mX), static_cast<float>(theDestRect.mY));
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theStretchedWidth
// @param     	theStretchedHeight
//
// \brief		
// \details 	<b>Description:</b> Draw the passed Image at the specified x and y screen coords
// stretching to the passed theStretchedWidth and theStretchedHeight.
//************************************/
void Graphics::DrawImage(SexyImage* theImage, int theX, int theY, int theStretchedWidth, int theStretchedHeight)
{
	if (theImage == NULL) return;

	SexyRect aSrcRect = SexyRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
	SexyRect aDestRect = SexyRect(theX, theY, theStretchedWidth, theStretchedHeight);

	DrawImage(theImage, aDestRect, aSrcRect);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
//
// \brief		Draw the passed Image at the specified x and y screen coords (float version).
// \details 	<b>Description:</b> 	Draw the passed Image at the specified x and y screen coords (float version).
//************************************/
void Graphics::DrawImageF(SexyImage* theImage, float theX, float theY)
{
	if (theImage == NULL) return;

	SexyRect aSrcRect = SexyRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
	DrawImageF(theImage, theX, theY, aSrcRect);
}


/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theSrcRect
//
// \brief		
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// at the specified x and y screen coords (float version).
//************************************/
void Graphics::DrawImageF(SexyImage* theImage, float theX, float theY, const SexyRect& theSrcRect)
{
	if (theImage == NULL) return;

	Sexy::Transform aTransform;
	aTransform.Translate(static_cast<float>(theSrcRect.mWidth / 2), static_cast<float>(theSrcRect.mHeight / 2));
	DrawImageMatrix(theImage, aTransform.GetMatrix(), theSrcRect, theX, theY);
}

/*!***********************************
// @return    	void
// @param     	theDest
// @param     	theComponentImage
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageBox(const SexyRect& theDest, SexyImage* theComponentImage)
{
	DrawImageBox(SexyRect(0, 0, theComponentImage->GetWidth(), theComponentImage->GetHeight()), theDest, theComponentImage);
}

/*!***********************************
// @return    	void
// @param     	theSrc
// @param     	theDest
// @param     	theComponentImage
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageBox(const SexyRect& theSrc, const SexyRect& theDest, SexyImage* theComponentImage)
{
	if ((theSrc.mWidth <= 0) || (theSrc.mHeight <= 0) || (theComponentImage == NULL))
		return;
	PreDraw();

	float cw = (float)theSrc.mWidth / 3.0f;
	float ch = (float)theSrc.mHeight / 3.0f;

	float dx = (float)theDest.mX;
	float dy = (float)theDest.mY;
	float dw = (float)theDest.mWidth;
	float dh = (float)theDest.mHeight;

	SexyVector2 aDVA[] = //aDestVertArray
	{
		SexyVector2(dx,dy),			SexyVector2(dx+cw,dy),			SexyVector2(dx+dw-cw,dy),		SexyVector2(dx+dw,dy),
		SexyVector2(dx,dy+ch),		SexyVector2(dx+cw,dy+ch),		SexyVector2(dx+dw-cw,dy+ch),	SexyVector2(dx+dw,dy+ch),
		SexyVector2(dx,dy+dh-ch),	SexyVector2(dx+cw,dy+dh-ch),	SexyVector2(dx+dw-cw,dy+dh-ch),	SexyVector2(dx+dw,dy+dh-ch),
		SexyVector2(dx,dy+dh),		SexyVector2(dx+cw,dy+dh),		SexyVector2(dx+dw-cw,dy+dh),	SexyVector2(dx+dw,dy+dh)
	};

	float sx = (float)theSrc.mX;
	float sy = (float)theSrc.mY;
	float sw = (float)theSrc.mWidth;
	float sh = (float)theSrc.mHeight;

	SexyVector2 aSUVA[] = // aSrcUVArray
	{
		SexyVector2(sx,sy),			SexyVector2(sx+cw,sy),			SexyVector2(sx+sw-cw,sy),		SexyVector2(sx+sw,sy),
		SexyVector2(sx,sy+ch),		SexyVector2(sx+cw,sy+ch),		SexyVector2(sx+sw-cw,sy+ch),	SexyVector2(sx+sw,sy+ch),
		SexyVector2(sx,sy+sh-ch),	SexyVector2(sx+cw,sy+sh-ch),	SexyVector2(sx+sw-cw,sy+sh-ch),	SexyVector2(sx+sw,sy+sh-ch),
		SexyVector2(sx,sy+sh),		SexyVector2(sx+cw,sy+sh),		SexyVector2(sx+sw-cw,sy+sh),	SexyVector2(sx+sw,sy+sh),
	};

	// Convert to UV [0.0 .. 1.0f]
	for (int i = 0; i < 16; i++)
	{
		aSUVA[i].x /= theComponentImage->GetWidth();
		aSUVA[i].y /= theComponentImage->GetHeight();
	}

	// Build Triangles
	Sexy::SexyTriVertex theVertices[][3]=
	{
//Top Row/////////////////////////////////////////////////////////////////////////////
/*0 1 2*/{SexyTriVertex(aDVA[0].x, aDVA[0].y, aSUVA[0].x, aSUVA[0].y), SexyTriVertex(aDVA[1].x, aDVA[1].y, aSUVA[1].x, aSUVA[1].y), SexyTriVertex(aDVA[4].x, aDVA[4].y, aSUVA[4].x, aSUVA[4].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[4].x, aDVA[4].y, aSUVA[4].x, aSUVA[4].y), SexyTriVertex(aDVA[1].x, aDVA[1].y, aSUVA[1].x, aSUVA[1].y), SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[1].x, aDVA[1].y, aSUVA[1].x, aSUVA[1].y), SexyTriVertex(aDVA[2].x, aDVA[2].y, aSUVA[2].x, aSUVA[2].y), SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y), SexyTriVertex(aDVA[2].x, aDVA[2].y, aSUVA[2].x, aSUVA[2].y), SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[2].x, aDVA[2].y, aSUVA[2].x, aSUVA[2].y), SexyTriVertex(aDVA[3].x, aDVA[3].y, aSUVA[3].x, aSUVA[3].y), SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y), SexyTriVertex(aDVA[3].x, aDVA[3].y, aSUVA[3].x, aSUVA[3].y), SexyTriVertex(aDVA[7].x, aDVA[7].y, aSUVA[7].x, aSUVA[7].y)}, 

//Middle Row//////////////////////////////////////////////////////////////////////////
/*0 1 2*/{SexyTriVertex(aDVA[4].x, aDVA[4].y, aSUVA[4].x, aSUVA[4].y), SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y), SexyTriVertex(aDVA[8].x, aDVA[8].y, aSUVA[8].x, aSUVA[8].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[8].x, aDVA[8].y, aSUVA[8].x, aSUVA[8].y), SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y), SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[5].x, aDVA[5].y, aSUVA[5].x, aSUVA[5].y), SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y), SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y), SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y), SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[6].x, aDVA[6].y, aSUVA[6].x, aSUVA[6].y), SexyTriVertex(aDVA[7].x, aDVA[7].y, aSUVA[7].x, aSUVA[7].y), SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y), SexyTriVertex(aDVA[7].x, aDVA[7].y, aSUVA[7].x, aSUVA[7].y), SexyTriVertex(aDVA[11].x, aDVA[11].y, aSUVA[11].x, aSUVA[11].y)}, 

//Bottom Row//////////////////////////////////////////////////////////////////////////
/*0 1 2*/{SexyTriVertex(aDVA[8].x, aDVA[8].y, aSUVA[8].x, aSUVA[8].y), SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y), SexyTriVertex(aDVA[12].x, aDVA[12].y, aSUVA[12].x, aSUVA[12].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[12].x, aDVA[12].y, aSUVA[12].x, aSUVA[12].y), SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y), SexyTriVertex(aDVA[13].x, aDVA[13].y, aSUVA[13].x, aSUVA[13].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[9].x, aDVA[9].y, aSUVA[9].x, aSUVA[9].y), SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y), SexyTriVertex(aDVA[13].x, aDVA[13].y, aSUVA[13].x, aSUVA[13].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[13].x, aDVA[13].y, aSUVA[13].x, aSUVA[13].y), SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y), SexyTriVertex(aDVA[14].x, aDVA[14].y, aSUVA[14].x, aSUVA[14].y)}, 

/*0 1 2*/{SexyTriVertex(aDVA[10].x, aDVA[10].y, aSUVA[10].x, aSUVA[10].y), SexyTriVertex(aDVA[11].x, aDVA[11].y, aSUVA[11].x, aSUVA[11].y), SexyTriVertex(aDVA[14].x, aDVA[14].y, aSUVA[14].x, aSUVA[14].y)}, 
/*2 1 3*/{SexyTriVertex(aDVA[14].x, aDVA[14].y, aSUVA[14].x, aSUVA[14].y), SexyTriVertex(aDVA[11].x, aDVA[11].y, aSUVA[11].x, aSUVA[11].y), SexyTriVertex(aDVA[15].x, aDVA[15].y, aSUVA[15].x, aSUVA[15].y)}, 
	};

	// Re Color
	for (int i = 0; i < 18; i++)
	{
		theVertices[i][0].color = 0xFFFFFFFF;
		theVertices[i][1].color = 0xFFFFFFFF;
		theVertices[i][2].color = 0xFFFFFFFF;
	}

	DrawTrianglesTex(theComponentImage, theVertices, 18);
	return;
}

/*!***********************************
// @return    	void
// @param     	theImageStrip
// @param     	theX
// @param     	theY
// @param     	theCelCol
// @param     	theCelRow
//
// \brief		
// \details 	<b>Description:</b> Draw the specified cel of the passed theImageStrip 
// at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageCel(SexyImage* theImageStrip, int theX, int theY, int theCelCol, int theCelRow)
{
    DBG_ASSERT(theCelCol >= 0);
    DBG_ASSERT(theCelRow >= 0);

	SexyRect aCelRect = theImageStrip->GetCelRect(theCelCol, theCelRow);
	DrawImageF(theImageStrip, static_cast<float>(theX), static_cast<float>(theY), aCelRect);
}

/*!***********************************
// @return    	void
// @param     	theImageStrip
// @param     	theDestRect
// @param     	theCelCol
// @param     	theCelRow
//
// \brief		
// \details 	<b>Description:</b> Draw the specified cel of the passed theImageStrip 
// in the specified theDestRect, stretching to fit if necessary.
//************************************/
void Graphics::DrawImageCel(SexyImage* theImageStrip, const SexyRect& theDestRect, int theCelCol, int theCelRow)
{
    DBG_ASSERT(theCelCol >= 0);
    DBG_ASSERT(theCelRow >= 0);

	SexyRect aCelRect = theImageStrip->GetCelRect(theCelCol, theCelRow);
	DrawImage(theImageStrip, theDestRect, aCelRect);
}

/*!***********************************
// @return    	void
// @param     	theImageStrip
// @param     	theX
// @param     	theY
// @param     	theCel
//
// \brief		
// \details 	<b>Description:</b> Draw the specified cel of the passed theImageStrip 
// at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageCel(SexyImage* theImageStrip, int theX, int theY, int theCel)
{
    DBG_ASSERT(theCel >= 0);

    SexyRect aCelRect = theImageStrip->GetCelRect(theCel % theImageStrip->mNumTotal);
    DrawImage(theImageStrip, theX, theY, aCelRect);
}

/*!***********************************
// @return    	void
// @param     	theImageAnim
// @param     	theX
// @param     	theY
// @param     	theTime
//
// \brief		
// \details 	<b>Description:</b> Draw the next animation cel of the passed theImageStrip based on theTime
// at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageAnim(SexyImage* theImageAnim, int theX, int theY, int theTime)
{
	// We only draw with floats, so pass it to DrawImageF (JSULLIVAN)
	DrawImageF(theImageAnim, static_cast<float>(theX), static_cast<float>(theY), theImageAnim->GetAnimCelRect(theTime));
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	mirror
//
// \brief		Draw the image mirrored at the passed coords.
// \details 	<b>Description:</b> Draw the image mirrored at the passed coords.
//************************************/
void Graphics::DrawImageMirror(SexyImage* theImage, int theX, int theY, bool mirror /*= true*/)
{
	if (theImage == NULL) return;

	SexyRect aDestRect = SexyRect(theX, theY, theImage->GetWidth(), theImage->GetHeight());
	SexyRect aSrcRect = SexyRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
	DrawImageMirror(theImage, aDestRect, aSrcRect, mirror);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theSrcRect
// @param     	mirror
//
// \brief		
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// mirrored and at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageMirror(SexyImage* theImage, int theX, int theY, const SexyRect& theSrcRect, bool mirror /*= true*/)
{
	if (theImage == NULL) return;

	SexyRect aDestRect = SexyRect(theX, theY, theX + theImage->GetWidth(), theY + theImage->GetHeight());
	DrawImageMirror(theImage, aDestRect, theSrcRect, mirror);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theDestRect
// @param     	theSrcRect
// @param     	mirror
//
// \brief		Draw the part of the passed Image specified by theSrcRect mirrored
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// mirrored and in the specified dest rect.
//************************************/
void Graphics::DrawImageMirror(SexyImage* theImage, const SexyRect& theDestRect, const SexyRect& theSrcRect, bool mirror /*= true*/)
{
	if (theImage == NULL) return;

	if (!mirror)
	{
		DrawImage(theImage, theDestRect, theSrcRect);
		return;
	}

	Sexy::Transform aTransform;

	aTransform.Scale(-1.0, 1.0); // Must come before Translate (JSullivan)

	if ((theDestRect.mWidth != theSrcRect.mWidth) || (theDestRect.mHeight != theSrcRect.mHeight))
	{
		aTransform.Scale(static_cast<float>(theDestRect.mWidth) / theSrcRect.mWidth, 
			static_cast<float>(theDestRect.mHeight) / theSrcRect.mHeight);
	}
	aTransform.Translate(static_cast<float>(theDestRect.mWidth / 2), static_cast<float>(theDestRect.mHeight / 2));
	DrawImageMatrix(theImage, aTransform.GetMatrix(), theSrcRect, theDestRect, static_cast<float>(theDestRect.mX), static_cast<float>(theDestRect.mY));
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theRot
// @param     	theSrcRect
//
// \brief		Draw the part of the passed Image specified by theSrcRect
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// rotated by the passed rotation and at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageRotated(SexyImage* theImage, int theX, int theY, float theRot, const SexyRect *theSrcRect)
{
	if (theImage == NULL) return;

	float aRotCenterX = 0;
	float aRotCenterY = 0;

	if (theSrcRect == NULL)
	{
		aRotCenterX = theImage->GetWidth() / 2.0f;
		aRotCenterY = theImage->GetHeight() / 2.0f;
	}
	else
	{
		aRotCenterX = theSrcRect->mWidth / 2.0f;
		aRotCenterY = theSrcRect->mHeight / 2.0f;
	}

	DrawImageRotatedF(theImage, static_cast<float>(theX), static_cast<float>(theY), theRot, aRotCenterX, aRotCenterY, theSrcRect);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theRot
// @param     	theRotCenterX
// @param     	theRotCenterY
// @param     	theSrcRect
//
// \brief		Draw the part of the passed Image specified by theSrcRec Rotated
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect, 
// rotated by the passed rotation at the passed rotation center, and at 
// the specified x and y screen coords. Got it!?
//************************************/
void Graphics::DrawImageRotated(SexyImage* theImage, int theX, int theY, float theRot, int theRotCenterX, int theRotCenterY, const SexyRect *theSrcRect)
{
	if (theImage == NULL) return;

	DrawImageRotatedF(theImage, static_cast<float>(theX), static_cast<float>(theY), theRot, 
		static_cast<float>(theRotCenterX), static_cast<float>(theRotCenterY), theSrcRect);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theRot
// @param     	theSrcRect
//
// \brief		Draw the part of the passed Image specified by theSrcRect 
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// rotated by the passed rotation and at the specified x and y screen coords (float version).
//************************************/
void Graphics::DrawImageRotatedF(SexyImage* theImage, float theX, float theY, float theRot, const SexyRect *theSrcRect)
{
	if (theImage == NULL) return;

	float aRotCenterX = 0;
	float aRotCenterY = 0;

	if (theSrcRect == NULL)
	{
		aRotCenterX = theImage->GetWidth() / 2.0f;
		aRotCenterY = theImage->GetHeight() / 2.0f;
	}
	else
	{
		aRotCenterX = theSrcRect->mWidth / 2.0f;
		aRotCenterY = theSrcRect->mHeight / 2.0f;
	}

	DrawImageRotatedF(theImage, theX, theY, theRot, aRotCenterX, aRotCenterY, theSrcRect);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theX
// @param     	theY
// @param     	theRot
// @param     	theRotCenterX
// @param     	theRotCenterY
// @param     	theSrcRect
//
// \brief		Rotate Image specified by theSrcRect, 
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect, 
// rotated by the passed rotation at the passed rotation center, and at 
// the specified x and y screen coords (float version).
// This is also where all the other calls lead to and where the actual draw occurs.
//************************************/
void Graphics::DrawImageRotatedF(SexyImage* theImage, float theX, float theY, float theRot, 
								 float theRotCenterX, float theRotCenterY, const SexyRect *theSrcRect)
{
	if (theImage == NULL) return;

	if (theSrcRect == NULL)
	{
		SexyRect aSrcRect = SexyRect(0, 0, theImage->GetWidth(), theImage->GetHeight());

		Sexy::Transform aTransform;
		aTransform.Translate(theRotCenterX, theRotCenterY); // Move to rotation center (default is 0,0)
		aTransform.RotateDeg(theRot); // Rotate
		aTransform.Translate(-theRotCenterX, -theRotCenterY); // Move back to center

		// Shift (0,0) from center of image to UPPER LEFT CORNER as (0,0)
		aTransform.Translate(static_cast<float>(aSrcRect.mWidth / 2), static_cast<float>(aSrcRect.mHeight / 2));
		DrawImageMatrix(theImage, aTransform.GetMatrix(), aSrcRect, theX, theY); // Draw with the X/Y Offset
	}
	else
	{
		Sexy::Transform aTransform;
		aTransform.Translate(theRotCenterX, theRotCenterY); // Move to rotation center (default is 0,0)
		aTransform.RotateDeg(theRot);
		aTransform.Translate(-theRotCenterX, -theRotCenterY); // Move back to center

		// Shift (0,0) from center of image to UPPER LEFT CORNER as (0,0)
		aTransform.Translate(static_cast<float>(theSrcRect->mWidth / 2), static_cast<float>(theSrcRect->mHeight / 2));
		DrawImageMatrix(theImage, aTransform.GetMatrix(), *theSrcRect, theX, theY);// Draw with the X/Y Offset
	}
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theTransform
// @param     	theX
// @param     	theY
//
// \brief		Draw the passed Image at the specified x and y screen coords.
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageTransformF(SexyImage* theImage, const Transform &theTransform, float theX, float theY)
{
	if (theImage == NULL) return;

	SexyRect aSrcRect = SexyRect(0, 0, theImage->GetWidth(),theImage->GetHeight());
	DrawImageMatrix(theImage, theTransform.GetMatrix(), aSrcRect, theX, theY);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theTransform
// @param     	theSrcRect
// @param     	theX
// @param     	theY
//
// \brief		Draw the part of the passed Image specified by theSrcRect 
// \details 	<b>Description:</b> Draw the part of the passed Image specified by theSrcRect 
// at the specified x and y screen coords.
//************************************/
void Graphics::DrawImageTransformF(SexyImage* theImage, const Transform &theTransform, const SexyRect &theSrcRect, float theX, float theY)
{
	if (theImage == NULL) return;

	DrawImageMatrix(theImage, theTransform.GetMatrix(), theSrcRect, theX, theY);
}

/*!***********************************
// @return    	void
// @param     	theTexture
// @param     	v1
// @param     	v2
// @param     	v3
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawTriangleTex(SexyImage *theTexture, const Sexy::SexyTriVertex& v1, const Sexy::SexyTriVertex& v2, const Sexy::SexyTriVertex& v3)
{
	if (theTexture == NULL) return;
	PreDraw();

	KGraphic* aPTKImage = theTexture->GetKanjiImage();
	if(mColorizeImages)
		aPTKImage->setBlitColor((float)mColor.mRed/255.0f, (float)mColor.mGreen/255.0f, (float)mColor.mBlue/255.0f, (float)mColor.mAlpha/255.0f);
	else
		aPTKImage->setBlitColor(1, 1, 1, 1);

	aPTKImage->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);
	aPTKImage->setTextureQuality(mLinearBlend && !theTexture->GetNoBlendOverride());

	float src1_x = v1.u * theTexture->GetWidth();
	float src1_y = v1.v * theTexture->GetHeight();
	float src2_x = v2.u * theTexture->GetWidth();
	float src2_y = v2.v * theTexture->GetHeight();
	float src3_x = v3.u * theTexture->GetWidth();
	float src3_y = v3.v * theTexture->GetHeight();

	SexyColor color_v1(v1.color);
	SexyColor color_v2(v2.color);
	SexyColor color_v3(v3.color);

	float z = 0.0f;
	float w = 1.0f;

	// Begin drawing to batch.
	mBatch->beginBatch(aPTKImage);

	//mBatch->blitPerspectiveTriangle(src1_x, src2_y, src2_x, src1_y, src3_x, src3_y, v1.x, v1.y, 0.0f, 1.0f, v2.x, v2.y, 0.0f, 1.0f, v3.x, v3.y, 0.0f, 1.0f);
	if(aPTKImage->isSpliced())
		aPTKImage->blitArbitraryQuad(src1_x, src1_y, src2_x, src2_y, src3_x, src3_y, src3_x, src3_y,
		v1.x + mTransX, v1.y + mTransY, v2.x + mTransX, v2.y + mTransY, v3.x + mTransX, v3.y + mTransY, v3.x + mTransX, v3.y + mTransY);
	else
		mBatch->blitPerspectiveTriangleColor (src1_x, src1_y, src2_x, src2_y, src3_x, src3_y,
			v1.x + mTransX, v1.y + mTransY, z, w, (float)color_v1.mRed/255.0f, (float) color_v1.mGreen/255.0f, (float) color_v1.mBlue/255.0f,
			v2.x + mTransX, v2.y + mTransY, z, w, (float)color_v2.mRed/255.0f, (float) color_v2.mGreen/255.0f, (float) color_v2.mBlue/255.0f,
			v3.x + mTransX, v3.y + mTransY, z, w, (float)color_v3.mRed/255.0f, (float) color_v3.mGreen/255.0f, (float) color_v3.mBlue/255.0f);


	// End drawing to batch.
	mBatch->endBatch();
}

/*!***********************************
// @return    	void
// @param     	theTexture
// @param     	theVertices
// @param     	theNumTriangles
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawTrianglesTex(SexyImage *theTexture, const Sexy::SexyTriVertex theVertices[][3], int theNumTriangles)
{
	if (theTexture == NULL) return;
	PreDraw();

	KGraphic* aPTKImage = theTexture->GetKanjiImage();
	if(mColorizeImages)
		aPTKImage->setBlitColor((float)mColor.mRed/255.0f, (float)mColor.mGreen/255.0f, (float)mColor.mBlue/255.0f, (float)mColor.mAlpha/255.0f);
	else
		aPTKImage->setBlitColor(1, 1, 1, 1);

	aPTKImage->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);
	aPTKImage->setTextureQuality(mLinearBlend && !theTexture->GetNoBlendOverride());

	// Begin drawing to batch.
	mBatch->beginBatch(aPTKImage);

	for (int i = 0; i < theNumTriangles; ++i)
	{
		Sexy::SexyTriVertex v1 = theVertices[i][0];
		Sexy::SexyTriVertex v2  = theVertices[i][1];
		Sexy::SexyTriVertex v3 = theVertices[i][2];

		float src1_x = v1.u * theTexture->GetWidth();
		float src1_y = v1.v * theTexture->GetHeight();
		float src2_x = v2.u * theTexture->GetWidth();
		float src2_y = v2.v * theTexture->GetHeight();
		float src3_x = v3.u * theTexture->GetWidth();
		float src3_y = v3.v * theTexture->GetHeight();

		SexyColor color_v1(v1.color, v1.color>>24);
		SexyColor color_v2(v2.color, v2.color>>24);
		SexyColor color_v3(v3.color, v3.color>>24);

		float z = 0.0f;
		float w = 1.0f;

		if(color_v1.GetAlpha() == 0 && color_v2.GetAlpha() == 0 && color_v3.GetAlpha() == 0) 
			continue;
		//mBatch->blitPerspectiveTriangle(src1_x, src1_y, src2_x, src2_y, src3_x, src3_y, v1.x, v1.y, z, w, v2.x, v2.y, z, w, v3.x, v3.y, z, w);
		if(aPTKImage->isSpliced())
			aPTKImage->blitArbitraryQuad(src1_x,src1_y, src2_x,src2_y,src3_x, src3_y, src3_x, src3_y,
											v1.x + mTransX,v1.y + mTransY,v2.x + mTransX,v2.y + mTransY,v3.x + mTransX,v3.y + mTransY,v3.x + mTransX,v3.y + mTransY);
		else
		mBatch->blitPerspectiveTriangleColor (src1_x, src1_y, src2_x, src2_y, src3_x, src3_y,
			v1.x + mTransX, v1.y + mTransY, z, w, (float)color_v1.mRed/255.0f, (float) color_v1.mGreen/255.0f, (float) color_v1.mBlue/255.0f,
			v2.x + mTransX, v2.y + mTransY, z, w, (float)color_v2.mRed/255.0f, (float) color_v2.mGreen/255.0f, (float) color_v2.mBlue/255.0f,
			v3.x + mTransX, v3.y + mTransY, z, w, (float)color_v3.mRed/255.0f, (float) color_v3.mGreen/255.0f, (float) color_v3.mBlue/255.0f);
	}

	// End drawing to batch.
	mBatch->endBatch();
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theMatrix
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, float x, float y)
{
	SexyRect aSrcRect(0, 0, theImage->GetWidth(), theImage->GetHeight());
	SexyRect aDestRect = SexyRect(static_cast<int>(x), static_cast<int>(y), aSrcRect.mWidth, aSrcRect.mHeight);
	DrawImageMatrix(theImage, theMatrix, aSrcRect, aDestRect, x, y);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theMatrix
// @param     	theSrcRect
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, const SexyRect &theSrcRect, float x, float y)
{
	SexyRect aDestRect = SexyRect(static_cast<int>(x), static_cast<int>(y), theSrcRect.mWidth, theSrcRect.mHeight);
	DrawImageMatrix(theImage, theMatrix, theSrcRect, aDestRect, x, y);
}

/*!***********************************
// @return    	void
// @param     	theImage
// @param     	theMatrix
// @param     	theSrcRect
// @param     	theDestRect
// @param     	x
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, const SexyRect &theSrcRect, const SexyRect &theDestRect, float x, float y)
{
	// Early Checks to avoid Draw Call
	if (theImage == NULL || theImage->GetKanjiImage() == NULL) return;

	PreDraw();

	// Make a copy to keep all calls const (JSullivan)
	SexyRect aSrcRect = theSrcRect;

	// theSrcRect Clipped to image size (JSULLIVAN)
	if ((aSrcRect.mX + aSrcRect.mWidth) > theImage->GetWidth())
	{
		aSrcRect.mWidth = theImage->GetWidth() - aSrcRect.mX;
	}
	if ((aSrcRect.mY + aSrcRect.mHeight) > theImage->GetHeight())
	{
		aSrcRect.mHeight = theImage->GetHeight() - aSrcRect.mY;
	}

	x += mTransX;
	y += mTransY;

	KGraphic* aPTKImage = theImage->GetKanjiImage();

	if (aPTKImage && (aSrcRect.mWidth > 0) && (aSrcRect.mHeight > 0))
	{
		if(mColorizeImages)
		{
			aPTKImage->setBlitColor(static_cast<float>(mColor.mRed) / 255.0f, static_cast<float>(mColor.mGreen) / 255.0f, 
				static_cast<float>(mColor.mBlue) / 255.0f, static_cast<float>(mColor.mAlpha) / 255.0f);
		}
		else
		{
			aPTKImage->setBlitColor(1, 1, 1, 1);
		}

		aPTKImage->setAlphaMode((mDrawMode == DRAWMODE_ADDITIVE) ? K_ALPHA_ADDITIVE : K_ALPHA_NORMAL);
		aPTKImage->setTextureQuality(mLinearBlend && !theImage->GetNoBlendOverride()); // let an image turn off linear blending (JPOAG)

		//TODO: Clip the screen to mClipRect

		//////////////////////////////////////////////////////////////////////////
		// Matrix multiply coords
		//////////////////////////////////////////////////////////////////////////
		float w2 = (float)aSrcRect.mWidth / 2.0f;
		float h2 = (float)aSrcRect.mHeight / 2.0f;

		// Note: the arbitrary quad blitter converts to UV space for us
		float u0 = static_cast<float>(aSrcRect.mX);
		float u1 = static_cast<float>(aSrcRect.mX + aSrcRect.mWidth);
		float v0 = static_cast<float>(aSrcRect.mY);
		float v1 = static_cast<float>(aSrcRect.mY + aSrcRect.mHeight);

		SexyTriVertex aVerts[4];
		aVerts[0] = SexyTriVertex(-w2, -h2, u0, v0, 0xFFFFFFFF);
		aVerts[1] = SexyTriVertex(w2, -h2, u1, v0, 0xFFFFFFFF);
		aVerts[2] = SexyTriVertex(w2, h2, u1, v1, 0xFFFFFFFF);
		aVerts[3] = SexyTriVertex(-w2, h2, u0, v1, 0xFFFFFFFF);

		SexyMatrix3 aMatrix;
		aMatrix = theMatrix;// * mTransform.GetMatrix(); // Move the master trasnform to KWindow::setWorldMatrix

		for (int i = 0; i < 4; i++)
		{
			SexyVector3 v(aVerts[i].x, aVerts[i].y, 1);
			v = aMatrix * v;
			aVerts[i].x = v.x + x;
			aVerts[i].y = v.y + y;
		}

		aPTKImage->blitArbitraryQuad(u0, v0, u1, v0, u1, v1, u0, v1, 
			aVerts[0].x, aVerts[0].y, 
			aVerts[1].x, aVerts[1].y,
			aVerts[2].x, aVerts[2].y,
			aVerts[3].x, aVerts[3].y);
	}
}

/*!***********************************
// @return    	bool
// @param     	theStartX
// @param     	theStartY
// @param     	theEndX
// @param     	theEndY
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Graphics::DrawLineClipHelper(float* theStartX, float* theStartY, float* theEndX, float* theEndY)
{
	float aStartX = *theStartX;
	float aStartY = *theStartY;
	float aEndX = *theEndX;
	float aEndY = *theEndY;

	// Clip X
	if (aStartX > aEndX)
	{
		std::swap(aStartX,aEndX);
		std::swap(aStartY,aEndY);
	}

	if (aStartX < mClipRect.mX)
	{
		if (aEndX < mClipRect.mX)
			return false;
					
		float aSlope = (aEndY - aStartY) / (aEndX - aStartX);
		aStartY += static_cast<float>(mClipRect.mX - aStartX ) * aSlope;
		aStartX = static_cast<float>(mClipRect.mX);
	}

	if (aEndX >= mClipRect.mX + mClipRect.mWidth)
	{
		if (aStartX >= mClipRect.mX + mClipRect.mWidth)
			return false;

		float aSlope = (aEndY - aStartY) / (aEndX - aStartX);
		aEndY += (mClipRect.mX + mClipRect.mWidth - 1 - aEndX) * aSlope;
		aEndX = static_cast<float>(mClipRect.mX + mClipRect.mWidth - 1);
	}

	// Clip Y
	if (aStartY > aEndY)
	{
		std::swap(aStartX,aEndX);
		std::swap(aStartY,aEndY);
	}


	if (aStartY < mClipRect.mY)
	{
		if (aEndY < mClipRect.mY)
			return false;
					
		float aSlope = (aEndX - aStartX) / (aEndY - aStartY);
		aStartX += (mClipRect.mY - aStartY ) * aSlope;			

		aStartY = static_cast<float>(mClipRect.mY);
	}

	if (aEndY >= mClipRect.mY + mClipRect.mHeight)
	{
		if (ceil(aStartY) >= mClipRect.mY + mClipRect.mHeight)
			return false;

		float aSlope = (aEndX - aStartX) / (aEndY - aStartY);
		aEndX += (mClipRect.mY + mClipRect.mHeight - 1 - aEndY) * aSlope;			
		aEndY = static_cast<float>(mClipRect.mY + mClipRect.mHeight - 1);
	}

	*theStartX = aStartX;
	*theStartY = aStartY;
	*theEndX = aEndX;
	*theEndY = aEndY;

	return true;
}

/*!***********************************
// @return    	int
// @param     	u
// @param     	v
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Graphics::PFCompareInd(const void* u, const void* v) 
{
	return mPFPoints[*((int*) u)].mY <= mPFPoints[*((int*) v)].mY ? -1 : 1;
}

/*!***********************************
// @return    	int
// @param     	u
// @param     	v
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Graphics::PFCompareActive(const void* u, const void* v)
{
	return ((Edge*) u)->mX <= ((Edge*) v)->mX ? -1 : 1;
}

/*!***********************************
// @return    	void
// @param     	i
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::PFDelete(int i) // remove edge i from active list
{
    int j;

    for (j=0; j<mPFNumActiveEdges && mPFActiveEdgeList[j].i!=i; j++);    
	if (j>=mPFNumActiveEdges) return;	/* edge not in active list; happens at aMinY*/
    
	mPFNumActiveEdges--;
    memcpy(&mPFActiveEdgeList[j], &mPFActiveEdgeList[j+1], (mPFNumActiveEdges-j)*sizeof mPFActiveEdgeList[0]);
}

/*!***********************************
// @return    	void
// @param     	i
// @param     	y
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Graphics::PFInsert(int i, int y) // append edge i to end of active list
{
    int j;
    double dx;
    const SexyPoint *p, *q;

    j = i<mPFNumVertices-1 ? i+1 : 0;
    if (mPFPoints[i].mY < mPFPoints[j].mY) 
	{
		p = &mPFPoints[i]; 
		q = &mPFPoints[j];
	}
    else		   
	{
		p = &mPFPoints[j]; 
		q = &mPFPoints[i];
	}
    /* initialize x position at intersection of edge with scanline y */
    mPFActiveEdgeList[mPFNumActiveEdges].mDX = dx = (q->mX - p->mX)/(double) (q->mY - p->mY);
    mPFActiveEdgeList[mPFNumActiveEdges].mX = dx*(y+0.5 - p->mY - mTransY) + p->mX + mTransX;
    mPFActiveEdgeList[mPFNumActiveEdges].i = i;
	mPFActiveEdgeList[mPFNumActiveEdges].b = p->mY - 1.0/dx * p->mX;
    mPFNumActiveEdges++;
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Sexy::Graphics::PreDraw()
{
	// This isn't really a render target.  This is a placeholder to let us
	// know to draw directly to the screen.
	if(mRenderTarget == NULL)
	{
		if (gCurrentRenderTarget != NULL && gCurrentRenderTarget->GetKanjiImage() != NULL)
		{
			gCurrentRenderTarget->GetKanjiImage()->endRenderToTarget();
			gCurrentRenderTarget = NULL;
		}
	}
	else if(mRenderTarget != NULL && gCurrentRenderTarget != mRenderTarget)
	{
		if(gCurrentRenderTarget != NULL && gCurrentRenderTarget->GetKanjiImage() != NULL)
		{
			gCurrentRenderTarget->GetKanjiImage()->endRenderToTarget();
		}

		gCurrentRenderTarget = mRenderTarget;
		gCurrentRenderTarget->GetKanjiImage()->beginRenderToTarget();
	}
	else if (gCurrentRenderTarget->GetKanjiImage()->isRenderTargetLost())
	{
		gCurrentRenderTarget->GetKanjiImage()->beginRenderToTarget();
	}

	//////////////////////////////////////////////////////////////////////////
	// After the Render Targets are sorted, setup the world transforms
	//////////////////////////////////////////////////////////////////////////
	if (gSexyAppBase && gSexyAppBase->GetKWindow())
	{
		if(!mAccumTransStack.empty())
		{
			KMatrix aMatrix = mAccumTransStack.top();
			gSexyAppBase->GetKWindow()->setWorldMatrix(&aMatrix);
		}
		else
		{
			static KMatrix anIdentityMatrix;
			gSexyAppBase->GetKWindow()->setWorldMatrix(&anIdentityMatrix);
		}

		// Clipping Rect
#ifndef K_IPHONE 
		gSexyAppBase->GetKWindow()->setClipRect(true, (float)mClipRect.mX, (float)mClipRect.mY, (float)mClipRect.mX + mClipRect.mWidth, (float)mClipRect.mY + mClipRect.mHeight);
#else /// This is the only way to get iPhone's to draw correctly without the buffer - Karl/Galaxy613
		gSexyAppBase->GetKWindow()->setClipRect(false, 0.0f,0.0f,480.0f,320.0f);
#endif
	}
}

/*!***********************************
// @return    	void
//
// \brief		Stops all Render-To-Target Drawing.
// \details 	<b>Description:</b> Useful for saving a RTT texture so it can be drawn.
//************************************/
void Sexy::Graphics::Finalize()
{
	if(gCurrentRenderTarget)
	{
		gCurrentRenderTarget->GetKanjiImage()->endRenderToTarget();
		gCurrentRenderTarget = NULL;
	}
}

/*!***********************************
// @return    	void
//
// \brief		Clears all Transformations and loads the identity transform
// \details 	<b>Description:</b> Call this method to reset the camera
//************************************/
void Sexy::Graphics::ClearTransforms()
{
	while(!mAccumTransStack.empty())mAccumTransStack.pop();

	if (gSexyAppBase && gSexyAppBase->GetKWindow())
	{
		static KMatrix anIndentityMatrix;
		gSexyAppBase->GetKWindow()->setWorldMatrix(&anIndentityMatrix);
	}
}

/*!***********************************
// @return    	void
// @param     	theTransform
//
// \brief		Pushes the Transform onto the Transform Stack.
// \details 	<b>Description:</b> This will accumulate the transform
				with the previous transform.
//************************************/
void Sexy::Graphics::PushTransform( const SexyKanjiTransform& theTransform )
{
	KMatrix aMat;

	if(!mAccumTransStack.empty())
		aMat = mAccumTransStack.top();

	KMatrix inMat = theTransform.GetKMatrix();

	mAccumTransStack.push(inMat * aMat);
}

/*!***********************************
// @return    	void
//
// \brief		Pops the previously pushed transform from the stack.
// \details 	<b>Description:</b> This method will pop off the previous
				transform.  If no transforms are remaining, then the identity
				transform will be set.  Otherwise, we will use the total accumulation
				of transforms remaining. 
//************************************/
void Sexy::Graphics::PopTransform()
{
	if (!mAccumTransStack.empty())
		mAccumTransStack.pop();
}