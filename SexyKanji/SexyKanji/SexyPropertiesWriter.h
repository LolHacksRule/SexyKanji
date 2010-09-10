// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#pragma once

#include "Common.h"

namespace Sexy
{
	class SexyProperties;

	class SexyPropertiesWriter
	{
	protected:
		SexyProperties*						mProperties;
		SexyString							mError;
		bool								mHasFailed;
	public:
		SexyPropertiesWriter(SexyProperties* theProperties = NULL);
		virtual ~SexyPropertiesWriter(void);


		virtual SexyProperties*				GetProperties() const { return mProperties; }
		virtual void						SetProperties(SexyProperties* val) { mProperties = val; }

		virtual bool						WritePropertiesFile(const SexyString& theFilename);
		virtual bool						WritePropertiesFileBinary(const SexyString& theFilename);

		virtual SexyString					GetErrorText() {return mError;}
		virtual bool						HasFailed() const { return mHasFailed; }

		//////////////////////////////////////////////////////////////////////////
		// Internal Helpers
		//////////////////////////////////////////////////////////////////////////
	protected:
		virtual bool						Fail(const SexyString& theErrorText);
	};
};

