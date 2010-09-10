// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#pragma once

#include "Common.h"

namespace Sexy
{
	class SexyProperties
	{
		friend class SexyPropertiesWriter;
		friend class SexyPropertiesParser;
		//////////////////////////////////////////////////////////////////////////
		// Property Data
		//////////////////////////////////////////////////////////////////////////
	protected:
		StringStringVectorMap			mStringVectorProperties;
		StringBoolMap					mBoolProperties;
		StringStringMap					mStringProperties;
		StringIntMap					mIntProperties;
		StringDoubleMap					mDoubleProperties;

		//////////////////////////////////////////////////////////////////////////
		// Meta-Tags for binary serialization
		//////////////////////////////////////////////////////////////////////////
		enum{STRING_MAP_META_TAG, BOOL_MAP_META_TAG, INT_MAP_META_TAG, 
			DOUBLE_MAP_META_TAG, STRING_VECTOR_MAP_META_TAG,};
	public:
		SexyProperties(void);
		virtual ~SexyProperties(void);
		//////////////////////////////////////////////////////////////////////////
		// Access Methods
		//////////////////////////////////////////////////////////////////////////
		virtual bool					GetBoolean(const SexyString& theId);
		virtual bool					GetBoolean(const SexyString& theId, bool theDefault);	
		virtual int						GetInteger(const SexyString& theId);
		virtual int						GetInteger(const SexyString& theId, int theDefault);
		virtual double					GetDouble(const SexyString& theId);
		virtual double					GetDouble(const SexyString& theId, double theDefault);
		virtual SexyString				GetString(const SexyString& theId);
		virtual SexyString				GetString(const SexyString& theId, const SexyString& theDefault);

		virtual StringVector			GetStringVector(const SexyString& theId);

		virtual void					SetBoolean(const SexyString& theId, bool theValue);
		virtual void					SetInteger(const SexyString& theId, int theValue);
		virtual void					SetDouble(const SexyString& theId, double theValue);
		virtual void					SetString(const SexyString& theId, const SexyString& theValue);
		virtual void					SetStringVector(const SexyString& theId, const StringVector& theValue);

		virtual bool					HasString(SexyString theKey){return mStringProperties.find(theKey) != mStringProperties.end();}
		virtual bool					HasInteger(SexyString theKey){return mIntProperties.find(theKey) != mIntProperties.end();}
		virtual bool					HasBoolean(SexyString theKey){return mBoolProperties.find(theKey) != mBoolProperties.end();}
		virtual bool					HasDouble(SexyString theKey){return mDoubleProperties.find(theKey) != mDoubleProperties.end();}
		virtual bool					HasStringVector(SexyString theKey){return mStringVectorProperties.find(theKey) != mStringVectorProperties.end();}

		virtual void					Clear();

	};
};

#include "SexyAppBase.h"

#define _GETSTRING(a) gSexyAppBase->GetProperties()->GetString(a)
#define _GETDEFSTRING(a,b) gSexyAppBase->GetProperties()->GetString(a,b)

#define _GETINT(a) gSexyAppBase->GetProperties()->GetInteger(a)
#define _GETDEFINT(a,b) gSexyAppBase->GetProperties()->GetInteger(a,b)

#define _GETBOOL(a) gSexyAppBase->GetProperties()->GetBoolean(a)
#define _GETDEFBOOL(a,b) gSexyAppBase->GetProperties()->GetBoolean(a,b)

#define _GETDOUBLE(a) gSexyAppBase->GetProperties()->GetDouble(a)
#define _GETDEFDOUBLE(a,b) gSexyAppBase->GetProperties()->GetDouble(a,b)