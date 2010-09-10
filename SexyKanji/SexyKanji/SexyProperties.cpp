// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "SexyProperties.h"
#include "KPTK.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyProperties::SexyProperties()
{
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
SexyProperties::~SexyProperties()
{
}
/*!***********************************
// @return    	bool
// @param     	theId
//
// \brief		Returns the Boolean value paired with theId Key.
// \details 	<b>Description:</b> Returns the Boolean value paired with theId Key.
//************************************/
bool Sexy::SexyProperties::GetBoolean( const SexyString& theId )
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);
	if(anItr == mBoolProperties.end())
		K_LOG("Sexy::SexyProperties::GetBoolean :Warning: %s Boolean Variable not found. {Default = false;}", SexyStringToStringFast(theId).c_str());

	if (anItr != mBoolProperties.end())	
		return anItr->second;
	else
		return false;
}

/*!***********************************
// @return    	bool
// @param     	theId
// @param     	theDefault
//
// \brief		Returns the Boolean value paired with theId Key or theDefault value if Key is not in Map.
// \details 	<b>Description:</b> Returns the Boolean value paired with theId Key or theDefault value if Key is not in Map.
//************************************/
bool Sexy::SexyProperties::GetBoolean( const SexyString& theId, bool theDefault )
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);	

	if (anItr != mBoolProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}

/*!***********************************
// @return    	int
// @param     	theId
//
// \brief		Returns the Integer value paired with theId Key.
// \details 	<b>Description:</b> Returns the Integer value paired with theId Key.
//************************************/
int Sexy::SexyProperties::GetInteger( const SexyString& theId )
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);
	if(anItr == mIntProperties.end())
		K_LOG("Sexy::SexyProperties::GetInteger :Warning: %s Integer Variable not found. {Default = 0;}", SexyStringToStringFast(theId).c_str());

	if (anItr != mIntProperties.end())	
		return anItr->second;
	else
		return 0;
}
/*!***********************************
// @return    	int
// @param     	theId
// @param     	theDefault
//
// \brief		Returns the Integer value paired with theId Key or theDefault value if Key is not in Map.
// \details 	<b>Description:</b> Returns the Integer value paired with theId Key or theDefault value if Key is not in Map.
//************************************/
int Sexy::SexyProperties::GetInteger( const SexyString& theId, int theDefault )
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);	

	if (anItr != mIntProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}
/*!***********************************
// @return    	double
// @param     	theId
//
// \brief		Returns the Double value paired with theId Key.
// \details 	<b>Description:</b> Returns the Double value paired with theId Key.
//************************************/
double Sexy::SexyProperties::GetDouble( const SexyString& theId )
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);
	if(anItr == mDoubleProperties.end())
		K_LOG("Sexy::SexyProperties::GetDouble :Warning: %s Double Variable not found. {Default = 0.0;}", SexyStringToStringFast(theId).c_str());

	if (anItr != mDoubleProperties.end())	
		return anItr->second;
	else
		return 0.0;
}
/*!***********************************
// @return    	double
// @param     	theId
// @param     	theDefault
//
// \brief		Returns the Double value paired with theId Key or theDefault value if Key is not in Map.
// \details 	<b>Description:</b> Returns the Double value paired with theId Key or theDefault value if Key is not in Map.
//************************************/
double Sexy::SexyProperties::GetDouble( const SexyString& theId, double theDefault )
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);	

	if (anItr != mDoubleProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}
/*!***********************************
// @return    	SexyString
// @param     	theId
//
// \brief		Returns the String value paired with theId Key.
// \details 	<b>Description:</b> Returns the String value paired with theId Key.
//************************************/
SexyString Sexy::SexyProperties::GetString( const SexyString& theId )
{
	StringStringMap::iterator anItr = mStringProperties.find(theId);
	if(anItr == mStringProperties.end())
		K_LOG("Sexy::SexyProperties::GetString :Warning: %s String Variable not found. {Default = "";}", SexyStringToStringFast(theId).c_str());

	if (anItr != mStringProperties.end())	
		return (anItr->second);
	else
		return _S("");
}

/*!***********************************
// @return    	SexyString
// @param     	theId
// @param     	theDefault
//
// \brief		Returns the String value paired with theId Key or theDefault value if Key is not in Map.
// \details 	<b>Description:</b> Returns the String value paired with theId Key or theDefault value if Key is not in Map.
//************************************/
SexyString Sexy::SexyProperties::GetString( const SexyString& theId, const SexyString& theDefault )
{
	StringStringMap::iterator anItr = mStringProperties.find(theId);	

	if (anItr != mStringProperties.end())	
		return (anItr->second);
	else
		return theDefault;
}

/*!***********************************
// @return    	StringVector
// @param     	theId
//
// \brief		Returns the StringVector paired with theId Key.
// \details 	<b>Description:</b> Returns the StringVector paired with theId Key.
//************************************/
StringVector Sexy::SexyProperties::GetStringVector( const SexyString& theId )
{
	StringStringVectorMap::iterator anItr = mStringVectorProperties.find(theId);
	if(anItr == mStringVectorProperties.end())
		K_LOG("Sexy::SexyProperties::GetStringVector :Warning: %s String Vector not found. Default = {}", SexyStringToStringFast(theId).c_str());

	if (anItr != mStringVectorProperties.end())	
		return anItr->second;
	else
		return StringVector();
}

/*!***********************************
// @return    	void
// @param     	theId
// @param     	theValue
//
// \brief		Inserts the Value into the Boolean Map.
// \details 	<b>Description:</b> Inserts the Value into the Boolean Map.
//************************************/
void Sexy::SexyProperties::SetBoolean( const SexyString& theId, bool theValue )
{
	std::pair<StringBoolMap::iterator, bool> aPair = mBoolProperties.insert(StringBoolMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}
/*!***********************************
// @return    	void
// @param     	theId
// @param     	theValue
//
// \brief		Inserts the Value into the Integer Map.
// \details 	<b>Description:</b> Inserts the Value into the Integer Map.
//************************************/
void Sexy::SexyProperties::SetInteger( const SexyString& theId, int theValue )
{
	std::pair<StringIntMap::iterator, bool> aPair = mIntProperties.insert(StringIntMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

/*!***********************************
// @return    	void
// @param     	theId
// @param     	theValue
//
// \brief		Inserts the Value into the Double Map.
// \details 	<b>Description:</b> Inserts the Value into the Double Map.
//************************************/
void Sexy::SexyProperties::SetDouble( const SexyString& theId, double theValue )
{
	std::pair<StringDoubleMap::iterator, bool> aPair = mDoubleProperties.insert(StringDoubleMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

/*!***********************************
// @return    	void
// @param     	theId
// @param     	theValue
//
// \brief		Inserts the String Value into the String Map.
// \details 	<b>Description:</b> Inserts the String Value into the String Map.
//************************************/
void Sexy::SexyProperties::SetString( const SexyString& theId, const SexyString& theValue )
{
	std::pair<StringStringMap::iterator, bool> aPair = mStringProperties.insert(StringStringMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}
/*!***********************************
// @return    	void
//
// \brief		Clears all property values.
// \details 	<b>Description:</b> Clears all property values.
//************************************/
void Sexy::SexyProperties::Clear()
{
	mStringVectorProperties.clear();
	mBoolProperties.clear();
	mStringProperties.clear();
	mIntProperties.clear();
	mDoubleProperties.clear();
}
/*!***********************************
// @return    	void
// @param     	theId
// @param     	theValue
//
// \brief		Inserts the String vector into the StringVector Map.
// \details 	<b>Description:</b> Inserts the String vector into the StringVector Map.
//************************************/
void Sexy::SexyProperties::SetStringVector( const SexyString& theId, const StringVector& theValue )
{
	std::pair<StringStringVectorMap::iterator, bool> aPair = mStringVectorProperties.insert(StringStringVectorMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}