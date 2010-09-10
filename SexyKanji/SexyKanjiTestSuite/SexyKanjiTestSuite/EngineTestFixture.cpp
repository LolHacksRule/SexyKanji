#include "EngineTestFixture.h"
#include "KSysThread.h"

bool g_bRunning = false;
bool g_bInit = false;
bool b_bInited = false;
bool g_bRun = false;

void EngineTestFixture::setUp()
{

}

void EngineTestFixture::tearDown()
{

}

void EngineTestFixture::testEngineInit()
{
	g_bInit = true;
	while(!b_bInited && !mAppThread->hasCompleted())
		KSysThread::sleep(100);

	ASSERT(!mAppThread->hasCompleted());
}

void EngineTestFixture::testEngineRun()
{
	g_bRun = true;

	while(!g_bRunning && !mAppThread->hasCompleted())
		KSysThread::sleep(100);

	ASSERT(g_bRunning && !mAppThread->hasCompleted());
}

void EngineTestFixture::testEngineShutdown()
{
	ASSERT(!mAppThread->hasCompleted());

	mApp->Shutdown();

	while(g_bRunning && !mAppThread->hasCompleted())
		KSysThread::sleep(100);

	ASSERT(mAppThread->hasCompleted() && !g_bRunning);
}

void EngineTestFixture::engineThread( void *lpParam, long nParam )
{
	Sexy::SexyAppBase* theApp = (Sexy::SexyAppBase*)lpParam;

	if (theApp != NULL)
	{
		while(!g_bInit)
			KSysThread::sleep(100);
		try
		{
			ASSERT(theApp->Init());
			b_bInited = true;
		}
		catch (...)
		{
			return;
		}

		while(!g_bRun)
			KSysThread::sleep(100);

		g_bRunning = true;
		try
		{
			theApp->Start();
		}
		catch (...)
		{
			return;
		}
		g_bRunning = false;
	}
}

EngineTestFixture::~EngineTestFixture()
{
	if(mAppThread && !mAppThread->hasCompleted())
	{
		mApp->Shutdown();
		mAppThread->waitForCompletion();
	}
	delete mApp;
	mApp = NULL;
	Sexy::gSexyAppBase = NULL;

	delete mAppThread;
	mAppThread = NULL;
}