// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
	SexyKanji PerfTimer Version 0.1
	-------------------------------------------------------------	
	Created:	2010/03/06
	Filename: 	SexyKanji\PerfTimer.cpp
	
	Purpose:	SexyKanji debug utility for measuring performance

				Heiko Fischer - heiko@fischeronline.de

*********************************************************************/

#include "PerfTimer.h"

// to call gSexyAppBase->GetClock()
#include "SexyAppBase.h"

// Last include directive:
#include "memmgr.h"

using namespace Sexy;

// declare data  structures and globals

// the data for the collated perf informations
struct PerfInfo
{
	const char*				mPerfName;		// name
	mutable unsigned long	mStartTime;		// perf start time
	mutable unsigned long	mDuration;		// perf duration in ms
	mutable unsigned int	mLongestCall;	// longest call duration in ms
	mutable unsigned int	mStartCount;	// count number of start calls
	mutable unsigned int	mCallCount;		// number of calls to this perf

	PerfInfo(const char* theName) : mPerfName(theName), mStartTime(0), mDuration(0), mStartCount(0), mCallCount(0), mLongestCall(0) { }

	bool operator<(const PerfInfo &theInfo) const { return strcmp(mPerfName, theInfo.mPerfName) < 0; }
};

// the data for one entry for each StartTiming() and StopTiming() call
struct PerfRecord
{
	const char*			mName;
	unsigned long		mStartTime;	
	bool				mIsStarted;

	PerfRecord() { }
	PerfRecord(const char* theName, bool start) : mName(theName), mIsStarted(start) { mStartTime = gSexyAppBase->GetClock(); }
};

typedef std::set<PerfInfo>		PerfInfoSet;
typedef std::vector<PerfRecord> PerfRecordVector;

static PerfInfoSet		gPerfInfoSet;		// to collate information of the perfs after perf is stopped via EndPerf()
static bool				gPerfOn = false;	// is perf on ?
static unsigned long	gStartTime;			// the start time for BeginPerf()
static unsigned long	gCollateTime;		// consumed time for collating the records

unsigned long			gDuration = 0;		// total time of perf in ms
unsigned int			gStartCount = 0;	// counter for StartTiming() and StopTiming() calls

PerfRecordVector		gPerfRecordVector;	// one entry for each StartTiming() and StopTiming() call 
unsigned int			gPerfRecordTop = 0;	// count the entries in gPerfRecordVector

/*!***********************************
// @return    	void
// @param     	theRecord
//
// \brief		insert/update record in the resulting perf set
// \details 	<b>Description:</b> insert/update record in the resulting perf set
//************************************/
static inline void InsertPerfRecord(PerfRecord& theRecord)
{
	if (theRecord.mIsStarted == true)
	{
		// StartTiming() sets mIsStarted = true -> insert the record in the set

		PerfInfoSet::iterator anItr = gPerfInfoSet.insert(PerfInfo(theRecord.mName)).first;

		anItr->mCallCount++;

		anItr->mStartCount++;

		if (anItr->mStartCount == 1)
			anItr->mStartTime = theRecord.mStartTime;
	}
	else
	{
		// StopTiming() sets mIsStarted = false -> find the before inserted record

		PerfInfoSet::iterator anItr = gPerfInfoSet.find(theRecord.mName);

		if (anItr != gPerfInfoSet.end())
		{
			anItr->mStartCount--;

			if(anItr->mStartCount == 0)
			{
				// add duration, save longest duration
				unsigned long aDuration = theRecord.mStartTime - anItr->mStartTime;
				anItr->mDuration += aDuration;

				if (aDuration > anItr->mLongestCall)
					anItr->mLongestCall = aDuration;
			}
		}
	}
}

/*!***********************************
// @return    	void
//
// \brief		collate the records, measure and save collate time
// \details 	<b>Description:</b> collate the records, measure and save collate time
//************************************/
static inline void CollatePerfRecords()
{
	unsigned long aTime1, aTime2;

	aTime1 = gSexyAppBase->GetClock();

	for (unsigned int i=0; i < gPerfRecordTop; i++)
	{
		InsertPerfRecord(gPerfRecordVector[i]);
	}

	gPerfRecordTop = 0;

	aTime2 = gSexyAppBase->GetClock();

	gCollateTime += (aTime2 - aTime1);
}

/*!***********************************
// @return    	void
// @param     	theRecord
//
// \brief		push a record into the vector
// \details 	<b>Description:</b> , previously filled 
//				records will be overwritten
//************************************/
static inline void PushPerfRecord(PerfRecord theRecord)
{
	if (gPerfRecordTop >= (unsigned int)gPerfRecordVector.size())
	{
		gPerfRecordVector.push_back(theRecord);
	}
	else
	{
		// there are records from a previously started perf, overwrite them
		gPerfRecordVector[gPerfRecordTop] = theRecord;
	}

	gPerfRecordTop++;
}

/*!***********************************
// @return    	
//
// \brief		ctor, init member
// \details 	<b>Description:</b> ctor, init member
//************************************/
PerfTimer::PerfTimer()
{
	mStartTime	= 0;
	mDuration	= 0;
	mIsRunning	= false;
}

/*!***********************************
// @return    	void
//
// \brief		calculate the current duration
// \details 	<b>Description:</b> 
//************************************/
void PerfTimer::CalcDuration()
{
	mDuration = gSexyAppBase->GetClock() - mStartTime;
}

/*!***********************************
// @return    	void
//
// \brief		start the timing
// \details 	<b>Description:</b>start the timing 
//************************************/
void PerfTimer::Start()
{
	mIsRunning = true;
	mStartTime = gSexyAppBase->GetClock();
}

/*!***********************************
// @return    	void
//
// \brief		stop the timing
// \details 	<b>Description:</b> stop the timing
//************************************/
void PerfTimer::Stop()
{
	if (mIsRunning == true)
	{
		CalcDuration();
		mIsRunning = false;
	}
}

/*!***********************************
// @return    	unsigned long
//
// \brief		get the current duration
// \details 	<b>Description:</b> get the current duration
//************************************/
unsigned long PerfTimer::GetDuration()
{
	// if running -> re-calculate duration 
	if (mIsRunning == true)
		CalcDuration();

	return mDuration;
}

/*!***********************************
// @return    	bool true if perf is on, otherwise false
//
// \brief		get the perf state
// \details 	<b>Description:</b> get the perf state
//************************************/
bool SexyPerf::IsPerfOn()
{
	return gPerfOn;
}

/*!***********************************
// @return    	void
//
// \brief		start the perf measuring
// \details 	<b>Description:</b> start the perf measuring
//************************************/
void SexyPerf::BeginPerf()
{
	gPerfInfoSet.clear();
	gPerfRecordTop	= 0;
	gStartCount		= 0;
	gCollateTime	= 0;

	gPerfOn			= true;
	
	gStartTime = gSexyAppBase->GetClock();
}

/*!***********************************
// @return    	void
//
// \brief		end the perf measuring
// \details 	<b>Description:</b> end the perf measuring
//************************************/
void SexyPerf::EndPerf()
{
	CollatePerfRecords();

	gPerfOn = false;

	// calculate overall duration
	gDuration = gSexyAppBase->GetClock() - gStartTime - gCollateTime;
}


/*!***********************************
// @return    	void
// @param     	theName name of the perf
//
// \brief		start the timing
// \details 	<b>Description:</b> start the timing
//************************************/
void SexyPerf::StartTiming(const char* theName)
{
	if (gPerfOn == true)
	{
		gStartCount++;
		PushPerfRecord(PerfRecord(theName, true));
	}
}

/*!***********************************
// @return    	void
// @param     	theName
//
// \brief		stop the timing
// \details 	<b>Description:</b> stop the timing
//************************************/
void SexyPerf::StopTiming(const char* theName)
{
	if (gPerfOn == true)
	{
		PushPerfRecord(PerfRecord(theName, false));
		
		gStartCount--;
		
		if (gStartCount == 0)
		{
			CollatePerfRecords();
		}
	}
}

/*!***********************************
// @return    	std::string the results
//
// \brief		get the results 
// \details 	<b>Description:</b> get the results 
//************************************/
std::string SexyPerf::GetResults()
{
	std::string aResult;
	char aBuf[512];

	sprintf(aBuf,"Total Time: %u ms\n", gDuration);
	aResult += aBuf;

	for (PerfInfoSet::iterator anItr = gPerfInfoSet.begin(); anItr != gPerfInfoSet.end(); ++anItr)
	{
		const PerfInfo &anInfo = *anItr;
		
		sprintf(aBuf, "%s (%d calls, %%%.2f time): %u ms (%.2f avg, %u longest)\n", 
					anInfo.mPerfName,											// name
					anInfo.mCallCount,											// number of calls
					static_cast<float>(anInfo.mDuration) / gDuration * 100.0f,	// percentage of total time
					anInfo.mDuration,											// total duration in ms
					static_cast<float>(anInfo.mDuration) / anInfo.mCallCount,	// average duration in ms
					anInfo.mLongestCall);										// longest duration in ms
		
		aResult += aBuf;
	}

	return aResult;
}