// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyFont.h"

#include "KTrueText.h"
#include "XMLParser.h"

#include "SexyAppBase.h"
#include "ImageManager.h"
#include "SexyImage.h"
#include "ImageManager.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void SexyFont::PreInit()
{
	mAscent = 0;
	mLineSpacingOffset = 0;
	mAscentPadding = 0;
	mHeight = 0;
	mDescent = 0;
	mGlobalKerning = 0;

	mFontSize = 12; // default size in KTrueText

	mFontTable = NULL;
	mKTextFace = NULL;
	mUpLoaded = false;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyFont::SexyFont()
{
	PreInit();
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyFont::~SexyFont()
{
	delete [] mFontTable;
	mFontTable = NULL;

	delete mKTextFace;
	mKTextFace = NULL;
}

/*!***********************************
// @return    	
// @param     	theFont
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyFont::SexyFont( const SexyFont& theFont )
{
	// perform a deep copy
	mAscent = theFont.mAscent;
	mLineSpacingOffset = theFont.mLineSpacingOffset;
	mAscentPadding = theFont.mAscentPadding;
	mHeight = theFont.mHeight;
	mDescent = theFont.mDescent;
	mGlobalKerning = theFont.mGlobalKerning;

	mFontSize = theFont.mFontSize; // default size in KTrueText
	mFontFamilyName = theFont.mFontFamilyName; // default size in KTrueText
	mFontStyle = theFont.mFontStyle; // default size in KTrueText

	mFontTable = NULL;
	mKTextFace = NULL;
	mUpLoaded = false;
	mFontFilePath = theFont.mFontFilePath;

	if (dynamic_cast<KText*>(theFont.mKTextFace))
	{
		for (int i = 0; i < MAX_KTEXT_GLYPH_PAGES; i++)
			mGlyphPages[i] = theFont.mGlyphPages[i];

		mGlyphs.resize(theFont.mGlyphs.size());
		std::copy(theFont.mGlyphs.begin(), theFont.mGlyphs.end(), mGlyphs.begin());

		mKTextFace = new KText();

	}
	else
		OpenFontFile(theFont.mFontFilePath); // probably a TTF font (no appropriate copy ctor) (JPOAG)
}

/*!***********************************
// @return    	
// @param     	theFontFilePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyFont::SexyFont( std::wstring theFontFilePath )
{
	PreInit();
	OpenFontFile(theFontFilePath);
}

/*!***********************************
// @return    	
// @param     	theFontFilePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Sexy::SexyFont::SexyFont( std::string theFontFilePath )
{
	PreInit();
	OpenFontFile(theFontFilePath);
}

/*!***********************************
// @return    	bool
// @param     	theFilePath
//
// \brief		Parses the Image Font from the XML
// \details 	<b>Description:</b> Parses the Image Font from the XML file and preloads the graphics planes
//				in a thread-safe manner.
//************************************/
bool Sexy::SexyFont::ParseXMLFile( SexyString theFilePath )
{
	XMLParser aParser;

	if (aParser.OpenFile(theFilePath) && !aParser.HasFailed())
	{
		XMLElement aNode;

		while(aParser.NextElement(&aNode))
		{
			switch (aNode.mType)
			{
			case XMLElement::TYPE_START:
				{
					if (aNode.mValue == _S("KanjiFont"))
					{
						if(XMLParser::HasAttribute(&aNode, _S("ascent")))
						{
							sexysscanf(aNode.mAttributes[_S("ascent")].c_str(), _S("%d"), &mAscent);
							mAscent += 2;
						}

						if(XMLParser::HasAttribute(&aNode, _S("descent")))
							sexysscanf(aNode.mAttributes[_S("descent")].c_str(), _S("%d"), &mDescent);

						if(XMLParser::HasAttribute(&aNode, _S("kerning")))
							sexysscanf(aNode.mAttributes[_S("kerning")].c_str(), _S("%d"), &mGlobalKerning);

// 						if(XMLParser::HasAttribute(&aNode, L"line_spacing"))
// 							mLineSpacingOffset = aNode.mAttributes[L"line_spacing"];

						if(XMLParser::HasAttribute(&aNode, _S("size")))
							sexysscanf(aNode.mAttributes[_S("size")].c_str(), _S("%d"), &mFontSize);

						if(XMLParser::HasAttribute(&aNode, _S("style")))
							mFontStyle = aNode.mAttributes[_S("style")];

						if(XMLParser::HasAttribute(&aNode, _S("name")))
							mFontFamilyName = aNode.mAttributes[_S("name")];

						if(XMLParser::HasAttribute(&aNode, _S("height")))
							sexysscanf(aNode.mAttributes[_S("height")].c_str(), _S("%d"), &mHeight);

						mAscentPadding = mHeight - mAscent - mDescent;
					}
					else if (aNode.mValue == _S("ImagePlanes"))
					{
						SexyString aFontPathDir = GetFileDir(theFilePath, true);
						bool bParsingGlyphPlanes = true;
						while (bParsingGlyphPlanes && aParser.NextElement(&aNode))
						{
							switch (aNode.mType)
							{
							case XMLElement::TYPE_START:
								{
									if (aNode.mValue == _S("Plane"))
									{
										if (XMLParser::HasAttribute(&aNode, _S("id")))
										{
											int anID = 0;
											sexysscanf(aNode.mAttributes[_S("id")].c_str(), _S("%d"), &anID);
											if (anID >= 0 && anID < MAX_KTEXT_GLYPH_PAGES)
											{
												if (XMLParser::HasAttribute(&aNode, _S("path") )
													&& gSexyAppBase && gSexyAppBase->GetImageManager())
												{
													// TODO: Figure out the directory situation.  We should get the directory
													// of the descriptor file and prepend it to the image file. (JPOAG)
													delete mGlyphPages[anID];
													mGlyphPages[anID] = gSexyAppBase->GetImageManager()->GetSharedImage(aNode.mAttributes[_S("path")]);

													if(((SexyImage*)mGlyphPages[anID]) == NULL) // attempt to look in a different directory
														mGlyphPages[anID] = gSexyAppBase->GetImageManager()->GetSharedImage(aFontPathDir + aNode.mAttributes[_S("path")]);

													if (((SexyImage*)mGlyphPages[anID]) == NULL)
													{
														K_LOG("Sexy::SexyFont::ParseXMLFile :XML Font Loading Error:", StrFormat("Could not load Glyph Page: %s", SexyStringToStringFast(aNode.mAttributes[_S("path")]).c_str()).c_str());
														return false;
													}
												}
											}
										}
									}
									break;
								}
							case XMLElement::TYPE_END:
								{
									if(aNode.mValue == _S("ImagePlanes"))
										bParsingGlyphPlanes = false;
									break;
								}
							}
						}
					} // end: else if(Image Planes)
					else if (aNode.mValue == _S("FontTable"))
					{
						bool bParsingFontTable = true;
						while (bParsingFontTable && aParser.NextElement(&aNode))
						{
							switch (aNode.mType)
							{
							case XMLElement::TYPE_START:
								{
									if (aNode.mValue == _S("Char"))
									{
										KFont aChar;
										memset(&aChar, 0, sizeof(KFont));

										if (XMLParser::HasAttribute(&aNode, _S("code")))
											aChar._c = (unsigned long)aNode.mAttributes[_S("code")][0];
										if (XMLParser::HasAttribute(&aNode, _S("graphic_id")))
											sexysscanf(aNode.mAttributes[_S("graphic_id")].c_str(), _S("%d"), &aChar._nGraphic);

										float aReadFloat = 0;

										if (XMLParser::HasAttribute(&aNode, _S("sx1")))
										{
											if(sexysscanf(aNode.mAttributes[_S("sx1")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._x1 = (short)floor(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("sy1")))
										{
											if(sexysscanf(aNode.mAttributes[_S("sy1")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._y1 = (short)floor(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("sx2")))
										{
											if(sexysscanf(aNode.mAttributes[_S("sx2")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._x2 = (short)ceil(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("sy2")))
										{
											if(sexysscanf(aNode.mAttributes[_S("sy2")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._y2 = (short)ceil(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("advance")))
										{
											if(sexysscanf(aNode.mAttributes[_S("advance")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._w = (short)ceil(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("x_offset")))
										{
											if(sexysscanf(aNode.mAttributes[_S("x_offset")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._xOffset = (short)ceil(aReadFloat);
										}
										if (XMLParser::HasAttribute(&aNode, _S("y_offset")))
										{
											if(sexysscanf(aNode.mAttributes[_S("y_offset")].c_str(), _S("%f"), &aReadFloat) == 1)
												aChar._yOffset = (short)ceil(aReadFloat);
										}

										mGlyphs.push_back(aChar);
									}
									break;
								}
							case XMLElement::TYPE_END:
								{
									if(aNode.mValue == _S("FontTable"))
										bParsingFontTable = false;
									break;
								}
							}
						}
					}
					break;
				}
			case XMLElement::TYPE_END:
				{
					break;
				}
			}
		}

		return true;
	}
	return false;
}

/*!***********************************
// @return    	KTextFace*
//
// \brief		Returns the Kanji TextFace Object managed by this class.
// \details 	<b>Description:</b> Returns the Kanji TextFace Object managed by this class.
//************************************/
KTextFace* Sexy::SexyFont::GetKTextFace() 
{
	if (!mUpLoaded)
	{
		mUpLoaded = true;

		//////////////////////////////////////////////////////////////////////////
		// KTEXT is an Image Font.  The Font is possibly loaded in a second
		// thread and some of the objects can only be created in the main thread.
		// E.g. KGraphic uploads to the texture.
		//////////////////////////////////////////////////////////////////////////
		KText* pKText = dynamic_cast<KText*>(mKTextFace);
		if (pKText)
		{
			for (int i = 0; i < MAX_KTEXT_GLYPH_PAGES; i++)
			{
				// This should also cause the SexyImage to upload the graphic to a texture (JPOAG)
				if(((SexyImage*)mGlyphPages[i]) != NULL)
					pKText->setKGraphic(mGlyphPages[i]->GetKanjiImage(), i);

// This is a temp fix for font issues.
// 				if(((SexyImage*)mGlyphPages[i]) != NULL)
// 					if(mGlyphPages[i]->GetKanjiImage())
// 						mGlyphPages[i]->GetKanjiImage()->setTextureQuality(false);
			}

			delete [] mFontTable;
			mFontTable = NULL;

			//////////////////////////////////////////////////////////////////////////
			// KText knows the end of the font table by a NULL entry
			KFont aFinalChar;
			memset(&aFinalChar, 0, sizeof(KFont));
			mGlyphs.push_back(aFinalChar);

			//////////////////////////////////////////////////////////////////////////
			// KText doesn't manage the lifetime of the FontTable array
			mFontTable = new KFont[mGlyphs.size()];
			memcpy(mFontTable, &mGlyphs[0], sizeof(KFont) * mGlyphs.size());

			mGlyphs.pop_back();

			pKText->setFontTable(mFontTable);
			for(long i = 0; i < KTEXT_MAXPAGES; i++) pKText->setGlobalKerning(i, (float)mGlobalKerning);
			pKText->setYSpacing((float)mLineSpacingOffset);
		}

	}
	return mKTextFace;
}

/*!***********************************
// @return    	int
// @param     	theChar
//
// \brief		Width of the specified character.  Extremely accurate.
// \details 	<b>Description:</b> Width of the specified character.  Extremely accurate.
//************************************/
int Sexy::SexyFont::CharWidth( wchar_t theChar )
{
	if(!mUpLoaded) GetKTextFace();

	if (mKTextFace != NULL)
	{
		KText* anImageFont = dynamic_cast<KText*>(mKTextFace);
		
		// Attempt local glyph lookup: more accurate character size
		if (anImageFont && mGlyphs.size())
		{
			for (unsigned int i = 0; i < mGlyphs.size(); i++)
			{
				if (mGlyphs[i]._c == (unsigned long)theChar)
				{
					return mGlyphs[i]._w;
				}
			}
		}

		return (int)ceil(mKTextFace->getCharWidth(theChar, 0));
	}

	return -1;
}

/*!***********************************
// @return    	int
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::SexyFont::StringWidth( const SexyString& theString )
{
	if(!mUpLoaded) GetKTextFace();

	if (mKTextFace)
		return (int)mKTextFace->getStringWidth((const char*)theString.c_str(), 0, 1.0, SexyStrEncoding);

	return -1;
}

/*!***********************************
// @return    	bool
// @param     	theFontFilePath
// @param     	theGlyphPage0Path
//
// \brief		Opens the KFNT format
// \details 	<b>Description:</b> Opens the KFNT format
//************************************/
bool Sexy::SexyFont::OpenKFontFile( std::string theFontFilePath, std::string theGlyphPage0Path )
{
	return false; // TODO : Implement in a way so that the images are loaded in a thread safe manner.
}
/*!***********************************
// @return    	bool
// @param     	theFontFilePath
//
// \brief		Returns false if something went wrong when opening the font.
// \details 	<b>Description:</b> Returns false if something went wrong when opening the font.
//************************************/
bool Sexy::SexyFont::OpenFontFile( std::wstring theFontFilePath )
{
	//////////////////////////////////////////////////////////////////////////
	// Clear out Previous font
	//////////////////////////////////////////////////////////////////////////
	delete mKTextFace;
	mKTextFace = NULL;

	for (int i = 0; i < MAX_KTEXT_GLYPH_PAGES; i++)
		mGlyphPages[i].Release();

	//////////////////////////////////////////////////////////////////////////
	// Load new Font: XML File is Image Font, otherwise try to use FreeType
	//////////////////////////////////////////////////////////////////////////
	mFontFilePath = WStringToSexyStringFast(theFontFilePath);
	SexyString anExt = WStringToSexyStringFast(GetFileExt(theFontFilePath));
	if (Lower(anExt) == Lower(_S("xml")))
	{
		if(ParseXMLFile(mFontFilePath))
			mKTextFace = new KText();
		else
			return false;
	}
	else
		mKTextFace = new KTrueText(WStringToString(theFontFilePath).c_str());

	mUpLoaded = false;

	return true;
}

/*!***********************************
// @return    	bool
// @param     	theFontFilePath
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Sexy::SexyFont::OpenFontFile( std::string theFontFilePath )
{
	return OpenFontFile(StringToWString(theFontFilePath));
}

/*!***********************************
// @return    	SexyFont*
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyFont* Sexy::SexyFont::Duplicate()
{
	return new SexyFont(*this);
}

/*!***********************************
// @return    	void
// @param     	thePointHeight
//
// \brief		
// \details 	<b>Description:</b> This sets the point size of the TTF font after the font has 
//				been loaded (JPOAG)
//				Call this anytime, but beware a performance penalty for changing 
//				point sizes often.
//************************************/
void Sexy::SexyFont::SetFontPointHeight( int thePointHeight )
{
	KTrueText* pKTrueText = dynamic_cast<KTrueText*>(mKTextFace);
	if (pKTrueText)
	{
		mFontSize = thePointHeight;
		pKTrueText->setHeightPt(thePointHeight);
	}
}

/*!***********************************
// @return    	int
//
// \brief		Returns the Height of the Font in Pixels.
// \details 	<b>Description:</b> Returns the Height of the Font in Pixels.
//************************************/
int Sexy::SexyFont::GetHeight()
{
	if (mKTextFace != NULL) 
		return K_MAX(mKTextFace->getHeightPix(), mHeight);

	return mHeight;
}

/*!***********************************
// @return    	void
// @param     	val
//
// \brief		The spacing adjustment between characters [-10,10]
// \details 	<b>Description:</b> The spacing adjustment between characters [-10,10]
//************************************/
void Sexy::SexyFont::SetGlobalKerning( int val )
{
	mGlobalKerning = val;

	if (mUpLoaded)
	{
		KText* pKText = dynamic_cast<KText*>(mKTextFace);
		if (pKText)
			for(long i = 0; i < KTEXT_MAXPAGES; i++) 
				pKText->setGlobalKerning(i, (float)mGlobalKerning);
	}
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> the distance from the top of the character on one row to the top of the 
//				character on the next row
//************************************/
int Sexy::SexyFont::GetLineSpacing()
{
	if(mKTextFace != NULL)
		return (int)K_MAX(mKTextFace->getYSpacing() + mKTextFace->getHeightPix(), mLineSpacingOffset + GetHeight());

	return mLineSpacingOffset + GetHeight();
}

/*!***********************************
// @return    	int
//
// \brief		the global space adjustment between characters.
// \details 	<b>Description:</b> the global space adjustment between characters.
//************************************/
int Sexy::SexyFont::GetGlobalKerning() const
{
	return mGlobalKerning;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> the space between the bottom of the previous row and the
//				top of the next row.
//************************************/
int Sexy::SexyFont::GetLineSpacingOffset()
{
	KTrueText* pKTrueText = dynamic_cast<KTrueText*>(mKTextFace);
	if (pKTrueText)
		return (int)K_MAX(mKTextFace->getYSpacing(), mLineSpacingOffset);

	return mLineSpacingOffset;
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Sexy::SexyFont::GetFontSize() const
{
	return mFontSize;
}

