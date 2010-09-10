// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#include "Buffer.h"
#include "Common.h"

#include "KResource.h"

#include "memmgr.h"

#define POLYNOMIAL 0x04c11db7L // CRC Polynomial

static bool 	     bCrcTableGenerated = false;
static unsigned long crc_table[256];

using namespace Sexy;
using namespace std;

static char* gWebEncodeMap = ".-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static int gWebDecodeMap[256] = 
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, 0, -1, 1, 0, -1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1, -1, -1
, -1, -1, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
, 30, 31, 32, 33, 34, 35, 36, 37, -1, -1, -1, -1, -1, -1, 38, 39, 40, 41, 42, 43
, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

//----------------------------------------------------------------------------
// Generate the table of CRC remainders for all possible bytes.
//----------------------------------------------------------------------------
/*!***********************************
// @return    	void
//
// \brief Generate the table of CRC remainders for all possible bytes.
// \details <b>Description:</b> Generate the table of CRC remainders for all possible bytes.
//************************************/
static void GenerateCRCTable()
{
	bCrcTableGenerated = false;

	register int i, j;
	register unsigned long crc_accum;
	for (i = 0;  i < 256;  i++)
	{
		crc_accum = ((unsigned long) i << 24);
		for ( j = 0;  j < 8;  j++ )
		{
			if (crc_accum & 0x80000000L)
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			else
				crc_accum = (crc_accum << 1);
		}
		crc_table[i] = crc_accum;
	}
}

//----------------------------------------------------------------------------
// Update the CRC on the data block one byte at a time.
//----------------------------------------------------------------------------
/*!***********************************
// @return    	unsigned long
// @param     	crc_accum
// @param     	data_blk_ptr
// @param     	data_blk_size
//
// \brief		Update the CRC on the data block one byte at a time.
// \details 	<b>Description:</b> Update the CRC on the data block one byte at a time.
//************************************/
static unsigned long UpdateCRC(unsigned long crc_accum,
						const char *data_blk_ptr,
						int data_blk_size)
{
	if (!bCrcTableGenerated)
		GenerateCRCTable();
	
	register int i, j;
	for (j = 0; j < data_blk_size; j++)
	{
		i = ((int) (crc_accum >> 24) ^ *data_blk_ptr++) & 0xff;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}

//----------------------------------------------------------------------------
// Stream UTF8 data in a const char* to keep receiving wchar_ts
//----------------------------------------------------------------------------
/*!***********************************
// @return    	int
// @param     	theBuffer
// @param     	theLen
// @param     	theChar
//
// \brief		Stream UTF8 data in a const char* to keep receiving wchar_ts
// \details 	<b>Description:</b> Stream UTF8 data in a const char* to keep receiving wchar_ts
//************************************/
static int GetUTF8Char(const char** theBuffer, int theLen, wchar_t* theChar)
{
	static const unsigned short aMaskData[] = {
		0xC0,		// 1 extra byte
		0xE0,		// 2 extra bytes
		0xF0,		// 3 extra bytes
		0xF8,		// 4 extra bytes
		0xFC		// 5 extra bytes
	};

	if (theLen == 0) return 0;

	const char* aBuffer = *theBuffer;

	int aTempChar = int((unsigned char)*aBuffer++);
	if ((aTempChar & 0x80) != 0)
	{
		if ((aTempChar & 0xC0) != 0xC0) return 0; // sanity check: high bit should not be set without the next highest bit being set, too.

		int aBytesRead[6];
		int* aBytesReadPtr = &aBytesRead[0];

		*aBytesReadPtr++ = aTempChar;

		int aLen;
		for (aLen = 0; aLen < (int)(sizeof(aMaskData)/sizeof(*aMaskData)); ++aLen)
		{
			if ( (aTempChar & aMaskData[aLen]) == ((aMaskData[aLen] << 1) & aMaskData[aLen]) ) break;
		}
		if (aLen >= (int)(sizeof(aMaskData)/sizeof(*aMaskData))) return 0;

		aTempChar &= ~aMaskData[aLen];
		int aTotalLen = aLen+1;

		if (aTotalLen < 2 || aTotalLen > 6) return 0;

		int anExtraChar = 0;
		while (aLen > 0 && (aBuffer - *theBuffer) < theLen)
		{
			anExtraChar = int((unsigned char)*aBuffer++);
			if ((anExtraChar & 0xC0) != 0x80) return 0; // sanity check: high bit set, and next highest bit NOT set.

			*aBytesReadPtr++ = anExtraChar;

			aTempChar = (aTempChar << 6) | (anExtraChar & 0x3F);
			--aLen;
		}
		if (aLen > 0) return 0; // ran out of data before ending sequence

		// validate substrings
		bool valid = true;
		switch (aTotalLen)
		{
			case 2:
				valid = !((aBytesRead[0] & 0x3E) == 0);
				break;
			case 3:
				valid = !((aBytesRead[0] & 0x1F) == 0 && (aBytesRead[1] & 0x20) == 0);
				break;
			case 4:
				valid = !((aBytesRead[0] & 0x0F) == 0 && (aBytesRead[1] & 0x30) == 0);
				break;
			case 5:
				valid = !((aBytesRead[0] & 0x07) == 0 && (aBytesRead[1] & 0x38) == 0);
				break;
			case 6:
				valid = !((aBytesRead[0] & 0x03) == 0 && (aBytesRead[1] & 0x3C) == 0);
				break;
		}
		if (!valid) return 0;
	}

	int aConsumedCount = aBuffer - *theBuffer;
	
	if ( (aTempChar >= 0xD800 && aTempChar <= 0xDFFF) || (aTempChar >= 0xFFFE && aTempChar <= 0xFFFF) ) 
		return 0;

	*theChar = (wchar_t)aTempChar;

	*theBuffer = aBuffer;
	return aConsumedCount;
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Buffer::Buffer()
{
	mDataBitSize = 0;
	mReadBitPos = 0;
	mWriteBitPos = 0;	
}

/*!***********************************
// @return    	
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
Buffer::~Buffer()
{
}

/*!***********************************
// @return    	std::string
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Buffer::ToWebString() const
{
	std::string aString;
	int aSizeBits = mWriteBitPos;
	
	int anOldReadBitPos = mReadBitPos;
	mReadBitPos = 0;

	char aStr[256];
	sprintf(aStr, "%08X", aSizeBits);
	aString += aStr;

	int aNumChars = (aSizeBits + 5) / 6;
	for (int aCharNum = 0; aCharNum < aNumChars; aCharNum++)
		aString += gWebEncodeMap[ReadNumBits(6, false)];
	
	mReadBitPos = anOldReadBitPos;
	
	return aString;
}

/*!***********************************
// @return    	std::wstring
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Buffer::UTF8ToWideString() const
{
	const char* aData = (const char*)GetDataPtr();
	int aLen = GetDataLen();

	bool firstChar = true;

	std::wstring aString;
	aString.reserve(aLen); // worst case
	while (aLen > 0)
	{
		wchar_t aChar;
		int aConsumed = GetUTF8Char(&aData, aLen, &aChar);
		if (aConsumed == 0) break;
		aLen -= aConsumed;

		if (firstChar)
		{
			firstChar = false;
			if (aChar == 0xFEFF) continue;
		}

		aString += aChar;
	}
	return aString;
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::FromWebString(const std::string& theString)
{
	Clear();

	if (theString.size() < 4)
		return;
	
	int aSizeBits = 0;

	for (int aDigitNum = 0; aDigitNum < 8; aDigitNum++)
	{
		char aChar = theString[aDigitNum];
		int aVal = 0;

		if ((aChar >= '0') && (aChar <= '9'))
			aVal = aChar - '0';
		else if ((aChar >= 'A') && (aChar <= 'F'))
			aVal = (aChar - 'A') + 10;
		else if ((aChar >= 'a') && (aChar <= 'f'))
			aVal = (aChar - 'f') + 10;

		aSizeBits += (aVal << ((7 - aDigitNum) * 4));
	}

	int aCharIdx = 8;
	int aNumBitsLeft = aSizeBits;
	while (aNumBitsLeft > 0)
	{
		unsigned char aChar = theString[aCharIdx++];
		int aVal = gWebDecodeMap[aChar];
		int aNumBits = min(aNumBitsLeft, 6);
		WriteNumBits(aVal, aNumBits);
		aNumBitsLeft -= aNumBits;		
	}

	SeekFront();
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::SeekFront() const
{
	mReadBitPos = 0;	
}

/*!***********************************
// @return    	void
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::Clear()
{
	mReadBitPos = 0;
	mWriteBitPos = 0;
	mDataBitSize = 0;
	mData.clear();
}

/*!***********************************
// @return    	void
// @param     	theByte
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteByte(unsigned char theByte)
{	
	if (mWriteBitPos % 8 == 0)
		mData.push_back((char) theByte);
	else
	{		
		int anOfs = mWriteBitPos  % 8;
		mData[mWriteBitPos /8] |= theByte << anOfs;
		mData.push_back((char) (theByte >> (8-anOfs)));		
	}

	mWriteBitPos += 8;
	if (mWriteBitPos > mDataBitSize)
		mDataBitSize = mWriteBitPos;
}

/*!***********************************
// @return    	void
// @param     	theNum
// @param     	theBits
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteNumBits(int theNum, int theBits)
{
	for (int aBitNum = 0; aBitNum < theBits; aBitNum++)
	{
		if (mWriteBitPos % 8 == 0)
			mData.push_back(0);
		if ((theNum & (1<<aBitNum)) != 0)
			mData[mWriteBitPos/8] |= 1 << (mWriteBitPos  % 8);
		mWriteBitPos++;
	}

	if (mWriteBitPos > mDataBitSize)
		mDataBitSize = mWriteBitPos;
}

/*!***********************************
// @return    	int
// @param     	theNum
// @param     	isSigned
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Buffer::GetBitsRequired(int theNum, bool isSigned)
{
	if (theNum < 0) // two's compliment stuff
		theNum = -theNum - 1;
	
	int aNumBits = 0;
	while (theNum >= 1<<aNumBits)
		aNumBits++;
		
	if (isSigned)
		aNumBits++;
		
	return aNumBits;
}

/*!***********************************
// @return    	void
// @param     	theBool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteBoolean(bool theBool)
{
	WriteByte(theBool ? 1 : 0);
}

/*!***********************************
// @return    	void
// @param     	theShort
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteShort(short theShort)
{
	WriteByte((unsigned char)theShort);
	WriteByte((unsigned char)(theShort >> 8));
}

/*!***********************************
// @return    	void
// @param     	theLong
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteLong(long theLong)
{
	WriteByte((unsigned char)theLong);
	WriteByte((unsigned char)(theLong >> 8));
	WriteByte((unsigned char)(theLong >> 16));
	WriteByte((unsigned char)(theLong >> 24));
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteString(const std::string& theString)
{
	WriteShort((short) theString.length());
	for (int i = 0; i < (int)theString.length(); i++)
		WriteByte(theString[i]);
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteUTF8String(const std::wstring& theString)
{
	if ((mWriteBitPos & 7) != 0) // boo! let's get byte aligned.
		mWriteBitPos = (mWriteBitPos + 8) & ~7;

	WriteShort((short) theString.length());
	for (int i = 0; i < (int)theString.length(); ++i)
	{
		const unsigned int c = (unsigned int)theString[i]; // just in case wchar_t is only 16 bits, and it generally is in visual studio
		if (c < 0x80)
		{
			WriteByte((unsigned char)c);
		}
		else if (c < 0x800) 
		{
			WriteByte((unsigned char)(0xC0 | (c>>6)));
			WriteByte((unsigned char)(0x80 | (c & 0x3F)));
		}
		else if (c < 0x10000) 
		{
			WriteByte((unsigned char)(0xE0 | c>>12));
			WriteByte((unsigned char)(0x80 | ((c>>6) & 0x3F)));
			WriteByte((unsigned char)(0x80 | (c & 0x3F)));
		}
		else if (c < 0x110000) 
		{
			WriteByte((unsigned char)(0xF0 | (c>>18)));
			WriteByte((unsigned char)(0x80 | ((c>>12) & 0x3F)));
			WriteByte((unsigned char)(0x80 | ((c>>6) & 0x3F)));
			WriteByte((unsigned char)(0x80 | (c & 0x3F)));
		} // are the remaining ranges really necessary? add if so!
	}
}

/*!***********************************
// @return    	void
// @param     	theString
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteLine(const std::string& theString)
{
	WriteBytes((const unsigned char*) (theString + "\r\n").c_str(), (int) theString.length() + 2);
}

/*!***********************************
// @return    	void
// @param     	theBuffer
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteBuffer(const ByteVector& theBuffer)
{
	WriteLong((short) theBuffer.size());
	for (int i = 0; i < (int)theBuffer.size(); i++)
		WriteByte(theBuffer[i]);
}

/*!***********************************
// @return    	void
// @param     	theByte
// @param     	theCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::WriteBytes(const unsigned char* theByte, int theCount)
{
	for (int i = 0; i < theCount; i++)
		WriteByte(theByte[i]);
}

/*!***********************************
// @return    	void
// @param     	theBuffer
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::SetData(const ByteVector& theBuffer)
{
	mData = theBuffer;
	mDataBitSize = mData.size() * 8;
}

/*!***********************************
// @return    	void
// @param     	thePtr
// @param     	theCount
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::SetData(unsigned char* thePtr, int theCount)
{
	mData.clear();
	mData.insert(mData.begin(), thePtr, thePtr + theCount);
	mDataBitSize = mData.size() * 8;
}

/*!***********************************
// @return    	unsigned char
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
unsigned char Buffer::ReadByte() const
{
	if ((mReadBitPos + 7)/8 >= (int)mData.size())
	{		
		return 0; // Underflow
	}

	if (mReadBitPos % 8 == 0)
	{
		unsigned char b = mData[mReadBitPos/8];
		mReadBitPos += 8;
		return b;
	}
	else
	{
		int anOfs = mReadBitPos % 8;
			
		unsigned char b = 0;
		
		b = mData[mReadBitPos/8] >> anOfs;
		b |= mData[(mReadBitPos/8)+1] << (8 - anOfs);
		
		mReadBitPos += 8;		
		
		return b;
	}
}

/*!***********************************
// @return    	int
// @param     	theBits
// @param     	isSigned
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Buffer::ReadNumBits(int theBits, bool isSigned) const
{	
	int aByteLength = (int) mData.size();

	int theNum = 0;
	bool bset = false;
	for (int aBitNum = 0; aBitNum < theBits; aBitNum++)
	{
		int aBytePos = mReadBitPos/8;

		if (aBytePos >= aByteLength)
			break;

		if (bset = (mData[aBytePos] & (1<<(mReadBitPos%8))) != 0)			
			theNum |= 1<<aBitNum;
		
		mReadBitPos++;
	}
	
	if ((isSigned) && (bset)) // sign extend
		for (int aBitNum = theBits; aBitNum < 32; aBitNum++)
			theNum |= 1<<aBitNum;
	
	return theNum;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Buffer::ReadBoolean() const
{
	return ReadByte() != 0;
}

/*!***********************************
// @return    	short
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
short Buffer::ReadShort() const
{
	short aShort = ReadByte();
	aShort |= ((short) ReadByte() << 8);
	return aShort;	
}

/*!***********************************
// @return    	long
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
long Buffer::ReadLong() const
{
	long aLong = ReadByte();
	aLong |= ((long) ReadByte()) << 8;
	aLong |= ((long) ReadByte()) << 16;
	aLong |= ((long) ReadByte()) << 24;

	return aLong;
}

/*!***********************************
// @return    	std::string
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string	Buffer::ReadString() const
{
	std::string aString;
	int aLen = ReadShort();

	for (int i = 0; i < aLen; i++)
		aString += (char) ReadByte();

	return aString;
}

/*!***********************************
// @return    	std::wstring
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::wstring Buffer::ReadUTF8String() const
{
	if ((mReadBitPos & 7) != 0)
		mReadBitPos = (mReadBitPos + 8) & ~7; // byte align the read position

	std::wstring aString;
	int aLen = ReadShort();

	const char* aData = (const char*)(&mData[mReadBitPos/8]);
	int aDataSizeBytes = (mDataBitSize - mReadBitPos)/8;
	int aStartSizeBytes = aDataSizeBytes;

	int i;
	for (i = 0; aDataSizeBytes > 0 && i < aLen; ++i)
	{
		wchar_t aChar;
		int aConsumed = GetUTF8Char(&aData, aDataSizeBytes, &aChar);
		if (aConsumed == 0) break;
		aDataSizeBytes -= aConsumed;

		aString += aChar;
	}
//	assert(i == aLen); // if this fires, the UTF-8 data was malformed.

	mReadBitPos += (aStartSizeBytes - aDataSizeBytes) * 8;
	return aString;
}

/*!***********************************
// @return    	std::string
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
std::string Buffer::ReadLine() const
{
	std::string aString;

	for (;;)
	{
		char c = ReadByte();

		if ((c == 0) || (c == '\n'))
			break;

		if (c != '\r')
			aString += c;
	}

	return aString;
}

/*!***********************************
// @return    	void
// @param     	theData
// @param     	theLen
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::ReadBytes(unsigned char* theData, int theLen) const
{
	for (int i = 0; i < theLen; i++)
		theData[i] = ReadByte();
}

/*!***********************************
// @return    	void
// @param     	theByteVector
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
void Buffer::ReadBuffer(ByteVector* theByteVector) const
{
	theByteVector->clear();
	
	unsigned long aLength = ReadLong();

        // can't resize if lenght = 0
        if (aLength == 0) return;

	theByteVector->resize(aLength);
	ReadBytes(&(*theByteVector)[0], aLength);
}

/*!***********************************
// @return    	const unsigned char*
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
const unsigned char* Buffer::GetDataPtr() const
{
	if (mData.size() == 0)
		return NULL;
	return &mData[0];
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Buffer::GetDataLen() const
{
	return (mDataBitSize + 7) / 8; // Round up
}

/*!***********************************
// @return    	int
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
int Buffer::GetDataLenBits() const
{
	return mDataBitSize;
}

/*!***********************************
// @return    	unsigned long
// @param     	theSeed
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
unsigned long Buffer::GetCRC32(unsigned long theSeed) const
{	
	unsigned long aCRC = theSeed;
	aCRC = UpdateCRC(aCRC, (const char*) &mData[0], (int) mData.size());	
	return aCRC;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Buffer::AtEnd() const
{ 
	//return mReadBitPos >= (int)mData.size()*8;
	return mReadBitPos >= mDataBitSize;
}

/*!***********************************
// @return    	bool
//
// \brief		
// \details 	<b>Description:</b> 
//************************************/
bool Buffer::PastEnd() const
{
	return mReadBitPos > mDataBitSize;
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	theBuffer
//
// \brief		This Static method opens and reads a file into a Buffer Object
// \details 	<b>Description:</b> This Static method opens and reads a file into a Buffer Object
//************************************/
bool Sexy::Buffer::ReadBufferFromFile( SexyString theFileName, Buffer& theBuffer )
{
	std::string aFilePath = SexyStringToStringFast(theFileName);

	unsigned char* aFileBuffer = NULL;
	size_t	aFileBufferSize = 0;

	if (KResource::loadResource(aFilePath.c_str(), aFileBuffer, aFileBufferSize))
	{
		theBuffer.Clear();
		theBuffer.SetData(aFileBuffer, aFileBufferSize);

		delete [] aFileBuffer;

		return true;
	}

	return false;
}

/*!***********************************
// @return    	bool
// @param     	theFileName
// @param     	theBuffer
//
// \brief		Saves the Contents of the Buffer to the File
// \details 	<b>Description:</b> Saves the Contents of the Buffer to the File\n
//				This does not qualify the path for the file being saved.
//************************************/
bool Sexy::Buffer::WriteBufferToFile( SexyString theFileName, Buffer& theBuffer )
{
	std::string aFilePath = SexyStringToStringFast(theFileName);
	std::string aFileDir = GetFileDir(aFilePath);
	MkDir(aFileDir);

	FILE* aFP = fopen(aFilePath.c_str(), "w+b");

	if (aFP != NULL)
	{
		fwrite(theBuffer.GetDataPtr(), 1, theBuffer.GetDataLen(), aFP);
		fclose(aFP);

		return true;
	}

	return false;
}