#ifndef __DEBUG_INCLUDED__
#define __DEBUG_INCLUDED__

#include "Common.h"
#include <cassert>

extern bool gInAssert;

#ifndef SEXY_TRACING_ENABLED
void SexyTrace(const char *theStr);
#define SEXY_TRACE(theStr) SexyTrace(theStr)
#else
#define SEXY_TRACE(theStr)
#endif

extern void SexyTraceFmt(const SexyChar* fmt ...);
extern void OutputDebug(const SexyChar* fmt ...);

#ifdef _DEBUG

#define DBG_ASSERTE(exp)	{ gInAssert = true; assert(exp); gInAssert = false; }
#define DBG_ASSERT(exp)		{ gInAssert = true; assert(exp); gInAssert = false; }

#else

#define DBG_ASSERTE(exp)	((void)0)
#define DBG_ASSERT(exp)		((void)0)

#endif // _DEBUG

#endif //__DEBUG_INCLUDED__