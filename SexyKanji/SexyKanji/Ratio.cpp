// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h

#include "Ratio.h"

namespace Sexy
{

	/*!***********************************
	// @return    	
	//
	// \brief		Ratio is a Numerator/Denominator pair kept as separate values
	// \details 	<b>Description:</b> Save precision by NOT dividing numbers, but store as a numerator
	//				denominator pair.
	//************************************/
	Ratio::Ratio()
		: mNumerator(1)
		, mDenominator(1)
	{
	}

	/*!***********************************
	// @return    	
	// @param     	theNumerator
	// @param     	theDenominator
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	Ratio::Ratio(int theNumerator, int theDenominator)
	{
		Set(theNumerator, theDenominator);
	}

	/*!***********************************
	// @return    	void
	// @param     	theNumerator
	// @param     	theDenominator
	//
	// \brief		
	// \details 	<b>Description:</b> 
	//************************************/
	void Ratio::Set(int theNumerator, int theDenominator)
	{
		// find the greatest-common-denominator of theNumerator and theDenominator.
		int t;
		int a = theNumerator;
		int b = theDenominator;
		while (b != 0)
		{
			t = b;
			b = a % b;
			a = t;
		}

		// divide by the g-c-d to reduce mNumerator/mDenominator to lowest terms.
		mNumerator = theNumerator/a;
		mDenominator = theDenominator/a;
	}

}
