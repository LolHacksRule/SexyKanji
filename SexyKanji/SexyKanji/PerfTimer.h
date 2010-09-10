// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji PerfTimer Version 0.1
	-------------------------------------------------------------	
	Created:	2010/03/06
	Filename: 	SexyKanji\PerfTimer.h
	
	Purpose:	SexyKanji debug utility for measuring performance

				Heiko Fischer - heiko@fischeronline.de

	Usage:		Place this in the file(s) you want to do perf timing in (in the .cpp file):

				#define SEXY_PERF_ENABLED
				#include "PerfTimer.h"

				There are then a few different ways to do things but the clearest I think 
				is to just call the macro SEXY_PERF_BEGIN() at the start of the block of code 
				you want to analyze. At the end of the block, put SEXY_PERF_END() where is 
				that same string tag. Like so:

				SEXY_PERF_BEGIN("START of something");
					DoSomethingThatTakesTime();
				SEXY_PERF_END("START of something");

				Then when you run your game, make sure debug keys are on (ctrl-alt-d if they aren't) 
				and press F2 to start timing. Press F2 again to stop timing. When you hit F2 the 
				second time, a summary dialog box pops up telling you how many milliseconds were 
				spent in that function as well as what % of the overall app time was spent on it.

				You can profile as many things as you want. I tend to initially start with a wider 
				scope, profiling a large block of code, until I find the block that is taking up 
				an abnormally large chunk of time. At that point, I drill down and profile smaller 
				chunks within that previously large chunk until I get to the function or sub block 
				that is the culprit. (thanks for the explanation Architekt)

*********************************************************************/

#ifndef __SEXY_PERFTIMER_H__
#define __SEXY_PERFTIMER_H__

#include <string>
#include <set>
#include <vector>

namespace Sexy
{

	class PerfTimer 
	{
	protected:
		unsigned long		mStartTime;		// remember the start time
		unsigned long		mDuration;		// save the duration
		bool				mIsRunning;		// is the perf timer running ?

		virtual void		CalcDuration(void);

	public:
		PerfTimer();

		virtual void		Start(void);	
		virtual void		Stop(void);

		virtual unsigned long GetDuration(void);
	};


	class SexyPerf
	{
	public:
		static void			BeginPerf(void);
		static void			EndPerf(void);
		static bool			IsPerfOn(void);

		static void			StartTiming(const char* theName);
		static void			StopTiming(const char* theName);

		static std::string	GetResults(void);
	};


	class SexyAutoPerf
	{
	protected:
		const char*			mName;			
		bool				mIsStarted;		

	public:

		SexyAutoPerf(const char* theName) : mName(theName), mIsStarted(true) { SexyPerf::StartTiming(theName); }
		SexyAutoPerf(const char* theName, bool theStart) : mIsStarted(theStart), mName(theName) 
		{ 
			if (theStart == true) 
				SexyPerf::StartTiming(theName);
		}

		~SexyAutoPerf() { Stop(); }

		virtual void Start(void)
		{
			if (mIsStarted == false)
			{
				mIsStarted = true;
				SexyPerf::StartTiming(mName);
			}
		}

		virtual void Stop(void) 
		{ 
			if (mIsStarted == true) 
			{ 
				SexyPerf::StopTiming(mName);
				mIsStarted = false;
			}
		}
	};

} // namespace Sexy

#endif

//----------------------------------------------------------------------------
// these are the macros you will be using 
//----------------------------------------------------------------------------
// #define SEXY_PERF_ENABLED
#if defined(SEXY_PERF_ENABLED) && !defined(RELEASEFINAL)

#define SEXY_PERF_BEGIN(theName) SexyPerf::StartTiming(theName)
#define SEXY_PERF_END(theName) SexyPerf::StopTiming(theName)
#define SEXY_AUTO_PERF_MULTI(theName,theSuffix) SexyAutoPerf anAutoPerf##theSuffix(theName)
#define SEXY_AUTO_PERF_2(theName,theSuffix) SEXY_AUTO_PERF_MULTI(theName,theSuffix)
#define SEXY_AUTO_PERFL(theName) SEXY_AUTO_PERF_2(theName,__LINE__) // __LINE__ doesn't work correctly if Edit-and-Continue (/ZI) is enabled
#define SEXY_AUTO_PERF(theName) SEXY_AUTO_PERF_2(theName,UNIQUE)

#define SEXY_PERF_BEGIN_COND(theName,theCond) if(theCond) SexyPerf::StartTiming(theName)
#define SEXY_PERF_END_COND(theName,theCond) if(theCond) SexyPerf::StopTiming(theName)
#define SEXY_AUTO_PERF_MULTI_COND(theName,theSuffix,theCond) SexyAutoPerf anAutoPerf##theSuffix(theName,theCond); 
#define SEXY_AUTO_PERF_COND_2(theName,theSuffix,theCond) SEXY_AUTO_PERF_MULTI_COND(theName,theSuffix,theCond); 
#define SEXY_AUTO_PERF_CONDL(theName) SEXY_AUTO_PERF_COND_2(theName,__LINE__,theCond)
#define SEXY_AUTO_PERF_COND(theName) SEXY_AUTO_PERF_COND_2(theName,UNIQUE,theCond)

#else

#define SEXY_PERF_BEGIN(theName) 
#define SEXY_PERF_END(theName) 
#define SEXY_AUTO_PERF_MULTI(theName, theSuffix)
#define SEXY_AUTO_PERF(theName) 

#define SEXY_PERF_BEGIN_COND(theName, theCond) 
#define SEXY_PERF_END_COND(theName, theCond) 
#define SEXY_AUTO_PERF_MULTI_COND(theName, theSuffix, theCond) 
#define SEXY_AUTO_PERF_COND(theName) 

#endif
