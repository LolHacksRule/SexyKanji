#pragma once

#include "MiniCppUnit.hxx"
#include "Buffer.h"

class BufferTestFixture : public TestFixture<BufferTestFixture>
{
protected:
	Sexy::Buffer		mBuffer;
public:
	TEST_FIXTURE(BufferTestFixture)
	{
		TEST_CASE(testWriteBufferFile);
		TEST_CASE(testReadBufferFile);
		TEST_CASE(testWriteToBufferFile);
		TEST_CASE(testReadFromBufferFile);
	}
	virtual ~BufferTestFixture(void){}

public:
	void testReadBufferFile();
	void testWriteBufferFile();
	void testReadFromBufferFile();
	void testWriteToBufferFile();
};
REGISTER_FIXTURE( BufferTestFixture );