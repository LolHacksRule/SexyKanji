#include "FontManager.h"
#include "SexyFont.h"

// This needs to be the last include directive.  All other
// #includes should be above this point.
#include "memmgr.h"

using namespace Sexy;

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
FontManager::FontManager()
{
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
FontManager::~FontManager()
{
	// automatically clean up
	CleanSharedFonts(true);
}

/*!***********************************
// @return    	SexyFont*
// @param     	theFileName
//
// \brief		Returns a new SexyFont loaded from the specified file.
// \details 	<b>Description:</b> This method does not manage the SexyFont object
//				lifetime.  This creates a SexyFont and forgets that it exists.
//************************************/
SexyFont* Sexy::FontManager::GetFont( const std::wstring& theFileName )
{
	return new SexyFont(theFileName);
}

/*!***********************************
// @return    	SexyFont*
// @param     	theFileName
//
// \brief		Returns a new SexyFont loaded from the specified file.
// \details 	<b>Description:</b> This method does not manage the SexyFont object
//				lifetime.  This creates a SexyFont and forgets that it exists.
//************************************/
SexyFont* Sexy::FontManager::GetFont( const std::string& theFileName )
{
	return new SexyFont(theFileName);
}

/*!***********************************
// @return    	Sexy::SharedFontRef
// @param     	theFileName
// @param     	theVariant Forces a variation of the Font to be loaded.
// @param     	isNew pointer to Boolean variable.  This is set to true 
//				shared font is first loaded.
//
// \brief		Returns a SharedFontRef object for a SexyFont that the Font Manager manages.
// \details 	<b>Description:</b> FontManager will manage the lifetime of the SexyFont Object.
//************************************/
Sexy::SharedFontRef Sexy::FontManager::GetSharedFont( const std::wstring& theFileName, const std::wstring& theVariant /*= L""*/, bool* isNew /*= NULL*/ )
{
	SexyString anUpperFileName = StringToUpper(WStringToSexyStringFast(theFileName));
	SexyString anUpperVariant = StringToUpper(WStringToSexyStringFast(theVariant));

	std::pair<SharedFontMap::iterator, bool> aResultPair;
	SharedFontRef aSharedFontRef;

	{
		// Make sure the Map container isn't simultaneously accessed by separate threads.
		AutoCrit anAutoLock(mFontManagerCritSect);
		aResultPair = mSharedFontMap.insert(SharedFontMap::value_type(SharedFontMap::key_type(anUpperFileName, anUpperVariant), SharedFont()));
		aSharedFontRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second) // newly created
	{
		// Pass in a '!' as the first char of the file name to create a new Font
		if ((theFileName.length() > 0) && (theFileName[0] == '!'))
			aSharedFontRef.mSharedItem->mItem = new SexyFont();
		else
			aSharedFontRef.mSharedItem->mItem = GetFont(theFileName);
	}

	return aSharedFontRef;
}

/*!***********************************
// @return    	Sexy::SharedFontRef
// @param     	theFileName
// @param     	theVariant Forces a variation of the font to be loaded.
// @param     	isNew pointer to Boolean variable.  This is set to true 
//				shared font is first loaded.
//
// \brief		Returns a SharedFontRef object for a SexyFont that the Font Manager manages.
// \details 	<b>Description:</b> FontManager will manage the lifetime of the SexyFont Object.
//************************************/
Sexy::SharedFontRef Sexy::FontManager::GetSharedFont( const std::string& theFileName, const std::string& theVariant /*= ""*/, bool* isNew /*= NULL*/ )
{
	SexyString anUpperFileName = StringToUpper(StringToSexyStringFast(theFileName));
	SexyString anUpperVariant = StringToUpper(StringToSexyStringFast(theVariant));

	std::pair<SharedFontMap::iterator, bool> aResultPair;
	SharedFontRef aSharedFontRef;

	{
		// Make sure the Map container isn't simultaneously accessed by separate threads.
		AutoCrit anAutoLock(mFontManagerCritSect);
		aResultPair = mSharedFontMap.insert(SharedFontMap::value_type(SharedFontMap::key_type(anUpperFileName, anUpperVariant), SharedFont()));
		aSharedFontRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second)
	{
		// Pass in a '!' as the first char of the file name to create a new aRetImage
		if ((theFileName.length() > 0) && (theFileName[0] == '!'))
			aSharedFontRef.mSharedItem->mItem = new SexyFont();
		else
			aSharedFontRef.mSharedItem->mItem = GetFont(theFileName);
	}

	return aSharedFontRef;
}

/*!***********************************
// @return    	void
// @param     	forceDeleteAll Force all fonts to be deleted.
//
// \brief		Cleans up any used fonts.
// \details 	<b>Description:</b> ANy managed SexyFont object with reference count of zero
//				is removed from the list.
//************************************/
void Sexy::FontManager::CleanSharedFonts( bool forceDeleteAll /*= false*/ )
{
	// Make sure the Map container isn't simultaneously accessed by separate threads.
	AutoCrit anAutoLock(mFontManagerCritSect);

	// Delete shared images with reference counts of 0 or all if forced.
	SharedFontMap::iterator aSharedFontItr = mSharedFontMap.begin();
	while (aSharedFontItr != mSharedFontMap.end())
	{
		SharedFont* aSharedFont = &aSharedFontItr->second;
		if ((aSharedFont->mRefCount == 0) || forceDeleteAll)
		{
			delete aSharedFont->mItem; // This is probably redundant!
			mSharedFontMap.erase(aSharedFontItr++);
			// This might throw an error someday (JPOAG)
		}
		else
		{
			++aSharedFontItr;
		}
	}
}