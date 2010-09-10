// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <vector>

#include "Common.h"

namespace Sexy
{

typedef std::vector<unsigned char> ByteVector;

class Buffer
{
public:
	ByteVector				mData;
	int						mDataBitSize;
	mutable int				mReadBitPos;
	mutable int				mWriteBitPos;	

public:
	Buffer();
	virtual ~Buffer();

	//////////////////////////////////////////////////////////////////////////
	// Serialization Helpers
	//////////////////////////////////////////////////////////////////////////
	static bool				ReadBufferFromFile(SexyString theFileName, Buffer& theBuffer);	
	static bool				WriteBufferToFile(SexyString theFileName, Buffer& theBuffer);	
			

	//////////////////////////////////////////////////////////////////////////
	// Interface
	//////////////////////////////////////////////////////////////////////////
public:
	void					SeekFront() const;
	void					Clear();

	void					FromWebString(const std::string& theString);
	void					WriteByte(unsigned char theByte);
	void					WriteNumBits(int theNum, int theBits);
	static int				GetBitsRequired(int theNum, bool isSigned);
	void					WriteBoolean(bool theBool);
	void					WriteShort(short theShort);
	void					WriteLong(long theLong);
	void					WriteString(const std::string& theString);
	void					WriteUTF8String(const std::wstring& theString);
	void					WriteLine(const std::string& theString);	
	void					WriteBuffer(const ByteVector& theBuffer);
	void					WriteBytes(const unsigned char* theByte, int theCount);
	void					SetData(const ByteVector& theBuffer);
	void					SetData(unsigned char* thePtr, int theCount);

	std::string				ToWebString() const;
	std::wstring			UTF8ToWideString() const;
	unsigned char			ReadByte() const;
	int						ReadNumBits(int theBits, bool isSigned) const;
	bool					ReadBoolean() const;
	short					ReadShort() const;
	long					ReadLong() const;
	std::string				ReadString() const;	
	std::wstring			ReadUTF8String() const;
	std::string				ReadLine() const;
	void					ReadBytes(unsigned char* theData, int theLen) const;
	void					ReadBuffer(ByteVector* theByteVector) const;

	const unsigned char*	GetDataPtr() const;
	int						GetDataLen() const;	
	int						GetDataLenBits() const;
	unsigned long			GetCRC32(unsigned long theSeed = 0) const;

	bool					AtEnd() const;
	bool					PastEnd() const;
};

}

#endif //__BUFFER_H__