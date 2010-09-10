#include "BufferTestFixture.h"
#include "KPTK.h"

using namespace Sexy;

const char garbage[] = "adlkjfhlakjhdlkjfhladoufaiodopifpqojekjflakdjflkjalkjoiuqeoiurtoaihkjhbxkcnmvb,msnlkdhaoiuydf iahdkjfhkj";

void BufferTestFixture::testWriteBufferFile()
{
	mBuffer.Clear();
	mBuffer.WriteBytes((const unsigned char*)garbage, sizeof(garbage));

	ASSERT(Buffer::WriteBufferToFile(StringToSexyStringFast(KMiscTools::makeFilePath(("TestWrite.buf"))), mBuffer));
	ASSERT(FileExists((KMiscTools::makeFilePath(("TestWrite.buf")))));
}

void BufferTestFixture::testReadBufferFile()
{
	mBuffer.Clear();

	ASSERT(Buffer::ReadBufferFromFile(StringToSexyStringFast(KMiscTools::makeFilePath(("TestWrite.buf"))), mBuffer));
	
	char garbage_in[256] = {0};
	mBuffer.ReadBytes((unsigned char*)garbage_in, sizeof(garbage));

	ASSERT(K_strcasecmp(garbage_in, garbage) == 0);

	ASSERT(RemoveFile(KMiscTools::makeFilePath("TestWrite.buf")));

	ASSERT(!FileExists(KMiscTools::makeFilePath("TestWrite.buf")));	
}


void BufferTestFixture::testWriteToBufferFile()
{
	mBuffer.Clear();
	mBuffer.WriteBoolean(false);
	mBuffer.WriteBoolean(true);
	mBuffer.WriteByte(0xFE);
	mBuffer.WriteBytes((const unsigned char*)garbage, sizeof(garbage));
	mBuffer.WriteLine("This is a Test");
	mBuffer.WriteLong(0xFFEEFFEE);
	mBuffer.WriteShort(0xAABB);
	mBuffer.WriteString("AnotherTest.");
	mBuffer.WriteUTF8String(L"Màn");

	ASSERT(Buffer::WriteBufferToFile(StringToSexyStringFast(KMiscTools::makeFilePath(("TestWrite.buf"))), mBuffer));
	ASSERT(FileExists((KMiscTools::makeFilePath(("TestWrite.buf")))));
}

void BufferTestFixture::testReadFromBufferFile()
{
	mBuffer.Clear();
	ASSERT(Buffer::ReadBufferFromFile(StringToSexyStringFast(KMiscTools::makeFilePath(("TestWrite.buf"))), mBuffer));

	ASSERT(!mBuffer.ReadBoolean());
	ASSERT(mBuffer.ReadBoolean());
	ASSERT_EQUALS((unsigned char)0xFE, mBuffer.ReadByte());

	char garbage_in[256] = {0};
	mBuffer.ReadBytes((unsigned char*)garbage_in, sizeof(garbage));
	ASSERT(K_strcasecmp(garbage_in, garbage) == 0);

	ASSERT(mBuffer.ReadLine() == "This is a Test");
	ASSERT_EQUALS((unsigned long)0xFFEEFFEE, (unsigned long)mBuffer.ReadLong());
	ASSERT_EQUALS((unsigned short)0xAABB, (unsigned short)mBuffer.ReadShort());

	ASSERT(mBuffer.ReadString() == "AnotherTest.");
	ASSERT(mBuffer.ReadUTF8String() == L"Màn");

	ASSERT(RemoveFile(KMiscTools::makeFilePath("TestWrite.buf")));
	ASSERT(!FileExists(KMiscTools::makeFilePath("TestWrite.buf")));	
}
