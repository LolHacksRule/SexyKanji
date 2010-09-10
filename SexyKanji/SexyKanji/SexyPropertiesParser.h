// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __PROPERTIESPARSER_H_995fa57b_3454_4bd6_b397_5b5ee3d3e5c0__
#define __PROPERTIESPARSER_H_995fa57b_3454_4bd6_b397_5b5ee3d3e5c0__

#include "SexyAppBase.h"
#include "Common.h"

namespace Sexy
{
	class XMLParser;
	class SexyProperties;

	class SexyPropertiesParser
	{
	public:
		XMLParser*					mXMLParser;
		SexyString					mError;
		bool						mHasFailed;
		SexyProperties*				mProperties;

	public:
		SexyPropertiesParser(SexyProperties* theProperties = NULL);
		virtual ~SexyPropertiesParser();

		bool						ParsePropertiesFile(const SexyString& theFilename);
		bool						ParsePropertiesFileBinary(const SexyString& theFilename);
		SexyString					GetErrorText();
		virtual SexyProperties*		GetProperties() const { return mProperties; }
		virtual void				SetProperties(SexyProperties* val) { mProperties = val; }

		//////////////////////////////////////////////////////////////////////////
		// Internal Helpers
		//////////////////////////////////////////////////////////////////////////
	protected:
		void						Fail(const SexyString& theErrorText);

		bool						ParseSingleElement(SexyString* theString);
		bool						ParseStringArray(StringVector* theStringVector);
		bool						ParseProperties();
		bool						DoParseProperties();
	};

}

#endif //__PROPERTIESPARSER_H__