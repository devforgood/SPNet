#pragma once

#include "TextEncoding.h"

#include "NetBitWriter.h"
#include <stdlib.h>


typedef unsigned char byte;
typedef char sbyte;
typedef unsigned int uint;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned int UInt32;
typedef int Int32;
typedef unsigned long long UInt64;
typedef long long Int64;
typedef unsigned short ushort;
typedef unsigned long long ulong;


class NetBuffer
{
public:

	enum 
	{ 
		max_length = 1024 * 1024,
		header_length = 2,
		packet_index_length = 1,
		max_packet_length = USHRT_MAX - header_length,
		max_serialize_data_length = USHRT_MAX - (header_length-packet_index_length),
	};

	byte m_data[max_length];
	int m_bitLength;
	int m_readPosition;
	uint m_lastPacketLength;
	byte * m_lastPacket;
	int m_expectReadPosition;



	NetBuffer()
	{
		m_bitLength = 0;
		m_readPosition = 0;
	}

	void Clear()
	{
		m_bitLength = 0;
		m_readPosition = 0;
	}

	int LengthBytes()
	{
		return ((m_bitLength + 7) >> 3);
	}

	void LengthBytes(int value)
	{
		m_bitLength = value * 8;
	}

	int LengthBits()
	{
		return m_bitLength;
	}

	void LengthBits(int value)
	{
		m_bitLength = value;
	}

	Int64 Position()
	{
		return (Int64)m_readPosition;
	}

	void Position(Int64  value)
	{
		m_readPosition = (int)value;
	}

	int PositionInBytes()
	{
		return ((m_readPosition + 7) >> 3);
	}

	void PositionInBytes(int value)
	{
		m_readPosition = value * 8;
	}

	byte * GetBuffer()
	{
		return m_data + LengthBytes();
	}

	int GetBufferSize()
	{
		return max_length - LengthBytes();
	}

	bool Compact()
	{
		if(LengthBytes()>PositionInBytes())
		{
			int remain = LengthBytes() - PositionInBytes();
			memmove(m_data,m_data+PositionInBytes(),remain);
			LengthBytes(remain);
			m_readPosition = 0;
		}
		else if(LengthBytes()==PositionInBytes())
		{
			m_bitLength = 0;
			m_readPosition = 0;
		}
		else
		{
#ifdef _DEBUG
			assert(false);
#endif // _DEBUG
			return false;
		}
		return true;
	}



};
