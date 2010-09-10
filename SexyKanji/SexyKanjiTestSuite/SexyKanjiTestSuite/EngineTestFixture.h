#pragma once
#include "minicppunit.hxx"
#include "SexyAppBase.h"

class EngineTestFixture : public TestFixture<EngineTestFixture>
{
protected:
	Sexy::SexyAppBase*	mApp;
	KSysThread*			mAppThread;	

public:
	TEST_FIXTURE( EngineTestFixture )
	{
		mApp = new Sexy::SexyAppBase();
		mAppThread = new KSysThread(engineThread, mApp, 0);

		TEST_CASE( testEngineInit );
		TEST_CASE( testEngineRun );
		TEST_CASE( testEngineShutdown );
	}

	virtual ~EngineTestFixture();

public:
	virtual void setUp();
	virtual void tearDown();

	void testEngineInit();
	void testEngineRun();
	void testEngineShutdown();

	static void engineThread(void *lpParam, long nParam);
};
REGISTER_FIXTURE( EngineTestFixture );