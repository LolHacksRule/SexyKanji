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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SexyColor.h"
#include "SexyMatrix.h"
#include "SexyImage.h"
#include "SexyPoint.h"
#include "SexyRect.h"
#include "SexyVector.h"
#include "Common.h"
#include "KMath3d.h"

#include <stack>

class KBatch;
class KGraphic;

namespace Sexy
{
	// Span and Edge structs are for PolyFill
	struct Span
	{
		int mY;
		int mX;
		int mWidth;
	};

	struct Edge
	{
		double mX;
		double mDX;
		int i;
		double b;
	};


	class SexyTriVertex;
	class SexyFont;

	//////////////////////////////////////////////////////////////////////////
	// GraphicsState: Base class for Graphics. Allows us to keep a push/pop 
	// stack of the current state of the graphics object.
	////////////////////////////////////////////////////////////////(JeremyS)/
	class GraphicsState
	{
	protected:
		SexyImage*				mRenderTarget;
		SexyImage*				mLineGraphic;
	public:
		float					mTransX;
		float					mTransY;
		SexyRect				mClipRect;
		SexyColor				mColor;
		SexyFont*				mFont;
		int						mDrawMode;
		bool					mColorizeImages;
		bool					mWriteColoredString;
		bool					mLinearBlend;

		//////////////////////////////////////////////////////////////////////////
		// World Transform variables.
		//////////////////////////////////////////////////////////////////////////
		std::stack<KMatrix>		mAccumTransStack; // The top of the stack is the result of accumulated transforms.

	public:
		void CopyStateFrom(const GraphicsState* theState);
	};

	typedef std::list<GraphicsState> GraphicsStateList;

	class Graphics : public GraphicsState
	{
		friend class SexyAppBase;
		//////////////////////////////////////////////////////////////////////////
		// Const defines
		//////////////////////////////////////////////////////////////////////////
	public:
		static const int				BATCH_BUFFER_SIZE = 500;
		static const int				MAX_TEMP_SPANS = 8192;
		enum{DRAWMODE_NORMAL,DRAWMODE_ADDITIVE};
		enum{LEFT_JUSTIFIED = -1,CENTER_JUSTIFIED = 0,RIGHT_JUSTIFIED = 1};

		//////////////////////////////////////////////////////////////////////////
		// Members
		//////////////////////////////////////////////////////////////////////////
	protected:
		GraphicsStateList				mStateStack;

		KBatch*							mBatch;
		
		//////////////////////////////////////////////////////////////////////////
		// Poly Fill Variables
		//////////////////////////////////////////////////////////////////////////
		Edge*							mPFActiveEdgeList;
		int								mPFNumActiveEdges;
		static const SexyPoint*			mPFPoints;
		int								mPFNumVertices;

		bool							mIsImageOwner; // This determines if mLineGraphic gets delete during DTor

		//////////////////////////////////////////////////////////////////////////
		// Construction
		//////////////////////////////////////////////////////////////////////////
	public:
		Graphics(const Graphics& theGraphics);
		Graphics(SexyImage* theRenderTarget = NULL);
		~Graphics();

		//////////////////////////////////////////////////////////////////////////
		// Graphics object state management.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					PushState();
		virtual void					PopState();

		//////////////////////////////////////////////////////////////////////////
		// Accessors.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					SetFont(SexyFont* theFont);
		virtual SexyFont*				GetFont();

		virtual void					SetColor(const SexyColor& theColor);
		virtual const SexyColor&		GetColor();

		virtual void					SetDrawMode(int theDrawMode);
		virtual int						GetDrawMode();

		virtual void					SetColorizeImages(bool colorizeImages);
		virtual bool					GetColorizeImages();

		virtual void					SetWriteColoredStrings(bool writeColoredStrings);
		virtual bool					GetWriteColoredStrings();

		virtual void					SetLinearBlend(bool bTurnOnBlending);
		virtual bool					GetLinearBlend();

		virtual void					SetTransform(const SexyKanjiTransform& theTransform);
		virtual void					ClearTransforms();
		virtual void					PushTransform(const SexyKanjiTransform& theTransform);
		virtual void					PopTransform();

		virtual void					Translate(int theTransX, int theTransY);
		virtual void					TranslateF(float theTransX, float theTransY);


		//////////////////////////////////////////////////////////////////////////
		// Clip Rect routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					ClearClipRect();
		virtual void					SetClipRect(int theX, int theY, int theWidth, int theHeight);
		virtual void					SetClipRect(const SexyRect& theRect);
		virtual void					ClipRect(int theX, int theY, int theWidth, int theHeight);
		virtual void					ClipRect(const SexyRect& theRect);


		//////////////////////////////////////////////////////////////////////////
		// Standard primitives drawing routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawLine(int theStartX, int theStartY, int theEndX, int theEndY);
		virtual void					DrawRect(int theX, int theY, int theWidth, int theHeight);
		virtual void					DrawRect(const SexyRect& theRect);
		virtual void					FillRect(int theX, int theY, int theWidth, int theHeight);
		virtual void					FillRect(const SexyRect& theRect);
		virtual void					ClearRect(int theX, int theY, int theWidth, int theHeight);
		virtual void					ClearRect(const SexyRect& theRect);
		virtual void					PolyFill(const SexyPoint *theVertexList, int theNumVertices);

		//////////////////////////////////////////////////////////////////////////
		// String drawing routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawString(const SexyString& theString, int theX, int theY);
		virtual int						DrawStringColor(const SexyString& theString, int theX, int theY, int theOldColor = -1); //works like DrawString but can have color tags like ^ff0000^.
		virtual void					DrawStringClipped(const SexyString& theString, int theX, int theY);

		virtual int						WriteString(const SexyString& theString, int theX, int theY, int theWidth = -1, int theJustification = 0, bool drawString = true, int theOffset = 0, int theLength = -1, int theOldColor = -1);
		virtual int						WriteWordWrapped(const SexyRect& theRect, const SexyString& theString, int theLineSpacing = -1, int theJustification = -1, bool doBlit = true);
		virtual int						WriteWordWrapped(const SexyString& theString, int theX, int theY, int theWrapWidth = 10000000, int theLineSpacing = -1, int theJustification = -1, bool doBlit = true);
		virtual int						GetWordWrappedHeight(const SexyString& theString, const SexyRect& theRect, int theLineSpacing = -1, int theJustification = -1);
		virtual int						GetWordWrappedHeight(const SexyString& theString, int theWrapWidth = 10000000, int theLineSpacing = -1, int theJustification = -1);

		//////////////////////////////////////////////////////////////////////////
		// Standard image drawing routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawImage(SexyImage* theImage, int theX, int theY);
		virtual void					DrawImage(SexyImage* theImage, int theX, int theY, const SexyRect& theSrcRect);
		virtual void					DrawImage(SexyImage* theImage, const SexyRect& theDestRect, const SexyRect& theSrcRect);
		virtual void					DrawImage(SexyImage* theImage, int theX, int theY, int theStretchedWidth, int theStretchedHeight);
		virtual void					DrawImageF(SexyImage* theImage, float theX, float theY);
		virtual void					DrawImageF(SexyImage* theImage, float theX, float theY, const SexyRect& theSrcRect);

		virtual void					DrawImageBox(const SexyRect& theDest, SexyImage* theComponentImage);
		virtual void					DrawImageBox(const SexyRect& theSrc, const SexyRect& theDest, SexyImage* theComponentImage);


		//////////////////////////////////////////////////////////////////////////
		// Image strip/animation cel drawing routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawImageCel(SexyImage* theImageStrip, int theX, int theY, int theCelCol, int theCelRow);
		virtual void					DrawImageCel(SexyImage* theImageStrip, const SexyRect& theDestRect, int theCelCol, int theCelRow);
        virtual void					DrawImageCel(SexyImage* theImageStrip, int theX, int theY, int theCel);
		virtual void					DrawImageAnim(SexyImage* theImageAnim, int theX, int theY, int theTime);

		//////////////////////////////////////////////////////////////////////////
		// Transformed image drawing routines.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawImageMirror(SexyImage* theImage, int theX, int theY, bool mirror = true);
		virtual void					DrawImageMirror(SexyImage* theImage, int theX, int theY, const SexyRect& theSrcRect, bool mirror = true);
		virtual void					DrawImageMirror(SexyImage* theImage, const SexyRect& theDestRect, const SexyRect& theSrcRect, bool mirror = true);

		virtual void					DrawImageRotated(SexyImage* theImage, int theX, int theY, float theRot, const SexyRect *theSrcRect = NULL);
		virtual void					DrawImageRotated(SexyImage* theImage, int theX, int theY, float theRot, int theRotCenterX, int theRotCenterY, const SexyRect *theSrcRect = NULL);
		virtual void					DrawImageRotatedF(SexyImage* theImage, float theX, float theY, float theRot, const SexyRect *theSrcRect = NULL);
		virtual void					DrawImageRotatedF(SexyImage* theImage, float theX, float theY, float theRot, float theRotCenterX, float theRotCenterY, const SexyRect *theSrcRect = NULL);

		virtual void					DrawImageTransformF(SexyImage* theImage, const Transform& theTransform, float theX = 0, float theY = 0);
		virtual void					DrawImageTransformF(SexyImage* theImage, const Transform& theTransform, const SexyRect &theSrcRect, float theX = 0, float theY = 0);
		virtual void					DrawTriangleTex(SexyImage* theTexture, const Sexy::SexyTriVertex& v1, const Sexy::SexyTriVertex& v2, const Sexy::SexyTriVertex& v3);
		virtual void					DrawTrianglesTex(SexyImage* theTexture, const Sexy::SexyTriVertex theVertices[][3], int theNumTriangles);


		//////////////////////////////////////////////////////////////////////////
		// Matrix drawing routines. The majority of drawing routines end up here.
		////////////////////////////////////////////////////////////////(JeremyS)/
	public:
		virtual void					DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, float x = 0, float y = 0);
		virtual void					DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, const SexyRect &theSrcRect, float x = 0, float y = 0);
		virtual void					DrawImageMatrix(SexyImage* theImage, const SexyMatrix3 &theMatrix, const SexyRect &theSrcRect, const SexyRect &theDestRect, float x = 0, float y = 0);

		static void						Finalize();
	protected:
		bool							DrawLineClipHelper(float* theStartX, float* theStartY, float* theEndX, float* theEndY);
		virtual void					PreDraw();

		// These are all for PolyFill
		static int						PFCompareInd(const void* u, const void* v);
		static int						PFCompareActive(const void* u, const void* v);
		void							PFDelete(int i); 
		void							PFInsert(int i, int y);
	};


	/* GraphicsAutoState - simple class for auto pushing/popping the graphics state */
	/* Usage: 
		Sexy::GraphicsAutoState anAutoState(g); 
		at start of draw routines to automatically restore state after draw routine ends. */
	class GraphicsAutoState
	{
	public:
		Graphics* mGraphics;

	public:
		
		GraphicsAutoState(Graphics* theGraphics) : mGraphics(theGraphics)
		{
			mGraphics->PushState();
		}

		~GraphicsAutoState()
		{
			mGraphics->PopState();
		}
	};
}

#endif // __GRAPHICS_H__
