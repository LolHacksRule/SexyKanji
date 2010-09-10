// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#pragma once
#include "KTextFace.h"
#include "KText.h"

#include "SexyColor.h"
#include "SexyRect.h"
#include "Common.h"

#include "SharedImage.h"

#include <vector>

#define MAX_KTEXT_GLYPH_PAGES KTEXT_MAXPAGES // Soon to be 8 (JPOAG)

namespace Sexy
{
	class Graphics;
	class SexyImage;

	class SexyFont
	{
	protected:
		KTextFace*				mKTextFace;
		SharedImageRef			mGlyphPages[MAX_KTEXT_GLYPH_PAGES];
		bool					mUpLoaded;
		int						mGlobalKerning;
		SexyString				mFontFilePath;
		SexyString				mFontFamilyName;
		SexyString				mFontStyle;
		int						mFontSize;
		std::vector<KFont>		mGlyphs;
		KFont*					mFontTable;
		virtual void			PreInit();
		virtual bool			ParseXMLFile(SexyString theFilePath);

	public:
		int						mAscent;
		int						mDescent;
		int						mAscentPadding; // How much space is above the avg uppercase char
		int						mHeight;
		int						mLineSpacingOffset; // This plus height should get added between lines

	public:
		SexyFont(void);
		SexyFont(const SexyFont& theFont);
		SexyFont(std::wstring theFontFilePath);
		SexyFont(std::string theFontFilePath);
		virtual ~SexyFont(void);

		virtual bool			OpenFontFile( std::wstring theFontFilePath );
		virtual bool			OpenFontFile( std::string theFontFilePath );
		virtual bool			OpenKFontFile( std::string theFontFilePath, std::string theGlyphPage0Path );
		virtual void			SetFontPointHeight(int thePointHeight); // for TTF fonts (JPOAG)

		virtual int				GetAscent(){return mAscent;};
		virtual int				GetAscentPadding(){return mAscentPadding;};
		virtual int				GetDescent(){return mDescent;}
		virtual int				GetHeight();
		virtual int				GetGlobalKerning() const;
		virtual void			SetGlobalKerning(int val);
		virtual int				GetLineSpacingOffset();;
		virtual int				GetLineSpacing();
		virtual int				StringWidth(const SexyString& theString);
		virtual int				CharWidth(wchar_t theChar);
		virtual int				CharWidthKern(wchar_t theChar, wchar_t thePrevChar){return mGlobalKerning;}
		virtual SexyString		GetFontFamilyName() const { return mFontFamilyName; }
		virtual SexyString		GetFontStyle() const { return mFontStyle; }
		virtual int				GetFontSize() const;
		virtual void			SetFontSize(int val) { mFontSize = val; }

		virtual void			DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const SexyColor& theColor, const SexyRect& theClipRect){};

		virtual SexyFont*		Duplicate();;
		virtual KTextFace*		GetKTextFace() ;

	};
}

