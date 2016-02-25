#pragma once

#include "NetBuffer.h"

class NetOutgoingMessage : public NetBuffer
{
public:
	int m_msgIndex;

	union SingleUIntUnion
	{
		float SingleValue;
		unsigned int UIntValue;
	};


	template<class MESSAGE_TYPE> 
	bool Serialize(const MESSAGE_TYPE * msg)
	{
		if(max_serialize_data_length < msg->ByteSize())
		{
			LOG(INFO) << __FUNCTION__ << "() " << msg->Utf8DebugString();
			LOG(FATAL) << __FUNCTION__ << "() error";
			return false;
		}

		if(GetBufferSize() < msg->ByteSize())
		{
			LOG(INFO) << __FUNCTION__ << "() " << msg->Utf8DebugString();
			LOG(FATAL) << __FUNCTION__ << "() error";
			return false;
		}

		google::protobuf::io::ArrayOutputStream out_test(GetBuffer(), (int)(msg->ByteSize()));
		google::protobuf::io::CodedOutputStream output_stream(&out_test);
		if(msg->SerializeToCodedStream(&output_stream)==false)
		{
			LOG(INFO) << __FUNCTION__ << "() " << msg->Utf8DebugString();
			LOG(INFO) << __FUNCTION__ << "() error";
			return false;
		}

		m_bitLength += (8 * msg->ByteSize());

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// write

	/// <summary>
	/// Ensures the buffer can hold this number of bits
	/// </summary>
	void EnsureBufferSize(int numberOfBits)
	{
		//int byteLen = ((numberOfBits + 7) >> 3);
		//if (m_data == null)
		//{
		//	m_data = new byte[byteLen + c_overAllocateAmount];
		//	return;
		//}
		//if (m_data.Length < byteLen)
		//	Array.Resize<byte>(ref m_data, byteLen + c_overAllocateAmount);
		//return;
	}

	/// <summary>
	/// Ensures the buffer can hold this number of bits
	/// </summary>
	void InternalEnsureBufferSize(int numberOfBits)
	{
		//int byteLen = ((numberOfBits + 7) >> 3);
		//if (m_data == null)
		//{
		//	m_data = new byte[byteLen];
		//	return;
		//}
		//if (m_data.Length < byteLen)
		//	Array.Resize<byte>(ref m_data, byteLen);
		//return;
	}

	/// <summary>
	/// Writes a boolean value using 1 bit
	/// </summary>
	void Write(bool value)
	{
		//EnsureBufferSize(m_bitLength + 8);
		//NetBitWriter::WriteByte(value, 8, m_data, m_bitLength);
		//m_bitLength += 8;

		EnsureBufferSize(m_bitLength + 1);
		NetBitWriter::WriteByte((value ? (byte)1 : (byte)0), 1, m_data, m_bitLength);
		m_bitLength += 1;
	}

	/// <summary>
	/// Write a byte
	/// </summary>
	void Write(byte source)
	{
		EnsureBufferSize(m_bitLength + 8);
		NetBitWriter::WriteByte(source, 8, m_data, m_bitLength);
		m_bitLength += 8;
	}

	/// <summary>
	/// Writes a signed byte
	/// </summary>
	void Write(sbyte source)
	{
		EnsureBufferSize(m_bitLength + 8);
		NetBitWriter::WriteByte((byte)source, 8, m_data, m_bitLength);
		m_bitLength += 8;
	}

	/// <summary>
	/// Writes 1 to 8 bits of a byte
	/// </summary>
	void Write(byte source, int numberOfBits)
	{
		//NetException.Assert((numberOfBits > 0 && numberOfBits <= 8), "Write(byte, numberOfBits) can only write between 1 and 8 bits");
		EnsureBufferSize(m_bitLength + numberOfBits);
		NetBitWriter::WriteByte(source, numberOfBits, m_data, m_bitLength);
		m_bitLength += numberOfBits;
	}

	/// <summary>
	/// Writes all bytes in an array
	/// </summary>
	void Write(byte* source, int numberOfBytes)
	{
		//if (source == null)
		//	throw new ArgumentNullException("source");
		int bits = numberOfBytes * 8;
		EnsureBufferSize(m_bitLength + bits);
		NetBitWriter::WriteBytes(source, 0, numberOfBytes, m_data, m_bitLength);
		m_bitLength += bits;
	}

	/// <summary>
	/// Writes the specified number of bytes from an array
	/// </summary>
	void Write(byte* source, int offsetInBytes, int numberOfBytes)
	{
		//if (source == null)
		//	throw new ArgumentNullException("source");
		int bits = numberOfBytes * 8;
		EnsureBufferSize(m_bitLength + bits);
		NetBitWriter::WriteBytes(source, offsetInBytes, numberOfBytes, m_data, m_bitLength);
		m_bitLength += bits;
	}

	/// <summary>
	/// Writes an unsigned 16 bit integer
	/// </summary>
	/// <param name="source"></param>
	void Write(UInt16 source)
	{
		EnsureBufferSize(m_bitLength + 16);
		NetBitWriter::WriteUInt16(source, 16, m_data, m_bitLength);
		m_bitLength += 16;
	}

	/// <summary>
	/// Writes a 16 bit unsigned integer at a given offset in the buffer
	/// </summary>
	void WriteAt(Int32 offset, UInt16 source)
	{
		int newBitLength = (m_bitLength < offset + 16) ? offset + 16 : m_bitLength;
		EnsureBufferSize(newBitLength);
		NetBitWriter::WriteUInt16(source, 16, m_data, offset);
		m_bitLength = newBitLength;
	}

	/// <summary>
	/// Writes an unsigned integer using 1 to 16 bits
	/// </summary>
	void Write(UInt16 source, int numberOfBits)
	{
		//NetException.Assert((numberOfBits > 0 && numberOfBits <= 16), "Write(ushort, numberOfBits) can only write between 1 and 16 bits");
		EnsureBufferSize(m_bitLength + numberOfBits);
		NetBitWriter::WriteUInt16(source, numberOfBits, m_data, m_bitLength);
		m_bitLength += numberOfBits;
	}

	/// <summary>
	/// Writes a signed 16 bit integer
	/// </summary>
	void Write(Int16 source)
	{
		EnsureBufferSize(m_bitLength + 16);
		NetBitWriter::WriteUInt16((ushort)source, 16, m_data, m_bitLength);
		m_bitLength += 16;
	}

	/// <summary>
	/// Writes a 16 bit signed integer at a given offset in the buffer
	/// </summary>
	void WriteAt(Int32 offset, Int16 source)
	{
		int newBitLength = (m_bitLength < offset + 16) ? offset + 16 : m_bitLength;
		EnsureBufferSize(newBitLength);
		NetBitWriter::WriteUInt16((ushort)source, 16, m_data, offset);
		m_bitLength = newBitLength;
	}

#if UNSAFE
	/// <summary>
	/// Writes a 32 bit signed integer
	/// </summary>
	unsafe void Write(Int32 source)
	{
		EnsureBufferSize(m_bitLength + 32);

		// can write fast?
		if (m_bitLength % 8 == 0)
		{
			fixed (byte* numRef = &Data[m_bitLength / 8])
			{
				*((int*)numRef) = source;
			}
		}
		else
		{
			NetBitWriter::WriteUInt32((UInt32)source, 32, Data, m_bitLength);
		}
		m_bitLength += 32;
	}
#else
	/// <summary>
	/// Writes a 32 bit signed integer
	/// </summary>
	void Write(Int32 source)
	{
		EnsureBufferSize(m_bitLength + 32);
		NetBitWriter::WriteUInt32((UInt32)source, 32, m_data, m_bitLength);
		m_bitLength += 32;
	}
#endif

	/// <summary>
	/// Writes a 32 bit signed integer at a given offset in the buffer
	/// </summary>
	void WriteAt(Int32 offset, Int32 source)
	{
		int newBitLength = (m_bitLength < offset + 32) ? offset + 32 : m_bitLength;;
		EnsureBufferSize(newBitLength);
		NetBitWriter::WriteUInt32((UInt32)source, 32, m_data, offset);
		m_bitLength = newBitLength;
	}

#if UNSAFE
	/// <summary>
	/// Writes a 32 bit unsigned integer
	/// </summary>
	unsafe void Write(UInt32 source)
	{
		EnsureBufferSize(m_bitLength + 32);

		// can write fast?
		if (m_bitLength % 8 == 0)
		{
			fixed (byte* numRef = &Data[m_bitLength / 8])
			{
				*((uint*)numRef) = source;
			}
		}
		else
		{
			NetBitWriter::WriteUInt32(source, 32, Data, m_bitLength);
		}

		m_bitLength += 32;
	}
#else
	/// <summary>
	/// Writes a 32 bit unsigned integer
	/// </summary>
	void Write(UInt32 source)
	{
		EnsureBufferSize(m_bitLength + 32);
		NetBitWriter::WriteUInt32(source, 32, m_data, m_bitLength);
		m_bitLength += 32;
	}
#endif

	/// <summary>
	/// Writes a 32 bit unsigned integer at a given offset in the buffer
	/// </summary>
	void WriteAt(Int32 offset, UInt32 source)
	{
		int newBitLength = (m_bitLength < offset + 32) ? offset + 32 : m_bitLength;;
		EnsureBufferSize(newBitLength);
		NetBitWriter::WriteUInt32(source, 32, m_data, offset);
		m_bitLength = newBitLength;
	}

	/// <summary>
	/// Writes a 32 bit signed integer
	/// </summary>
	void Write(UInt32 source, int numberOfBits)
	{
		//NetException.Assert((numberOfBits > 0 && numberOfBits <= 32), "Write(uint, numberOfBits) can only write between 1 and 32 bits");
		EnsureBufferSize(m_bitLength + numberOfBits);
		NetBitWriter::WriteUInt32(source, numberOfBits, m_data, m_bitLength);
		m_bitLength += numberOfBits;
	}

	/// <summary>
	/// Writes a signed integer using 1 to 32 bits
	/// </summary>
	void Write(Int32 source, int numberOfBits)
	{
		//NetException.Assert((numberOfBits > 0 && numberOfBits <= 32), "Write(int, numberOfBits) can only write between 1 and 32 bits");
		EnsureBufferSize(m_bitLength + numberOfBits);

		if (numberOfBits != 32)
		{
			// make first bit sign
			int signBit = 1 << (numberOfBits - 1);
			if (source < 0)
				source = (-source - 1) | signBit;
			else
				source &= (~signBit);
		}

		NetBitWriter::WriteUInt32((uint)source, numberOfBits, m_data, m_bitLength);

		m_bitLength += numberOfBits;
	}

	/// <summary>
	/// Writes a 64 bit unsigned integer
	/// </summary>
	void Write(UInt64 source)
	{
		EnsureBufferSize(m_bitLength + 64);
		NetBitWriter::WriteUInt64(source, 64, m_data, m_bitLength);
		m_bitLength += 64;
	}

	/// <summary>
	/// Writes a 64 bit unsigned integer at a given offset in the buffer
	/// </summary>
	void WriteAt(Int32 offset, UInt64 source)
	{
		int newBitLength = (m_bitLength < offset + 64) ? offset + 64 : m_bitLength;;
		EnsureBufferSize(newBitLength);
		NetBitWriter::WriteUInt64(source, 64, m_data, offset);
		m_bitLength = newBitLength;
	}

	/// <summary>
	/// Writes an unsigned integer using 1 to 64 bits
	/// </summary>
	void Write(UInt64 source, int numberOfBits)
	{
		EnsureBufferSize(m_bitLength + numberOfBits);
		NetBitWriter::WriteUInt64(source, numberOfBits, m_data, m_bitLength);
		m_bitLength += numberOfBits;
	}

	/// <summary>
	/// Writes a 64 bit signed integer
	/// </summary>
	void Write(Int64 source)
	{
		EnsureBufferSize(m_bitLength + 64);
		ulong usource = (ulong)source;
		NetBitWriter::WriteUInt64(usource, 64, m_data, m_bitLength);
		m_bitLength += 64;
	}

	/// <summary>
	/// Writes a signed integer using 1 to 64 bits
	/// </summary>
	void Write(Int64 source, int numberOfBits)
	{
		EnsureBufferSize(m_bitLength + numberOfBits);
		ulong usource = (ulong)source;
		NetBitWriter::WriteUInt64(usource, numberOfBits, m_data, m_bitLength);
		m_bitLength += numberOfBits;
	}

	//
	// Floating point
	//
#if UNSAFE
	/// <summary>
	/// Writes a 32 bit floating point value
	/// </summary>
	unsafe void Write(float source)
	{
		uint val = *((uint*)&source);
#if BIGENDIAN
		val = NetUtility.SwapByteOrder(val);
#endif
		Write(val);
	}
#else
	/// <summary>
	/// Writes a 32 bit floating point value
	/// </summary>
	void Write(float source)
	{
		// Use union to avoid BitConverter.GetBytes() which allocates memory on the heap
		SingleUIntUnion su;
		su.UIntValue = 0; // must initialize every member of the union to avoid warning
		su.SingleValue = source;

#if BIGENDIAN
		// swap byte order
		su.UIntValue = NetUtility.SwapByteOrder(su.UIntValue);
#endif
		Write(su.UIntValue);
	}
#endif

#if UNSAFE
	/// <summary>
	/// Writes a 64 bit floating point value
	/// </summary>
	unsafe void Write(double source)
	{
		ulong val = *((ulong*)&source);
#if BIGENDIAN
		val = NetUtility.SwapByteOrder(val);
#endif
		Write(val);
	}
#else
	/// <summary>
	/// Writes a 64 bit floating point value
	/// </summary>
	void Write(double source)
	{
		byte val[8];
		(*(Int64*)val)= (*(Int64*)&source);
		//byte* val = BitConverter.GetBytes(source);
#if BIGENDIAN
		// 0 1 2 3   4 5 6 7

		// swap byte order
		byte tmp = val[7];
		val[7] = val[0];
		val[0] = tmp;

		tmp = val[6];
		val[6] = val[1];
		val[1] = tmp;

		tmp = val[5];
		val[5] = val[2];
		val[2] = tmp;

		tmp = val[4];
		val[4] = val[3];
		val[3] = tmp;
#endif
		Write(val, 8);
	}
#endif

	//
	// Variable bits
	//

	/// <summary>
	/// Write Base128 encoded variable sized unsigned integer of up to 32 bits
	/// </summary>
	/// <returns>number of bytes written</returns>
	int WriteVariableUInt32(uint value)
	{
		int retval = 1;
		uint num1 = (uint)value;
		while (num1 >= 0x80)
		{
			Write((byte)(num1 | 0x80));
			num1 = num1 >> 7;
			retval++;
		}
		Write((byte)num1);
		return retval;
	}

	/// <summary>
	/// Write Base128 encoded variable sized signed integer of up to 32 bits
	/// </summary>
	/// <returns>number of bytes written</returns>
	int WriteVariableInt32(int value)
	{
		uint zigzag = (uint)(value << 1) ^ (uint)(value >> 31);
		return WriteVariableUInt32(zigzag);
	}

	/// <summary>
	/// Write Base128 encoded variable sized signed integer of up to 64 bits
	/// </summary>
	/// <returns>number of bytes written</returns>
	int WriteVariableInt64(Int64 value)
	{
		ulong zigzag = (ulong)(value << 1) ^ (ulong)(value >> 63);
		return WriteVariableUInt64(zigzag);
	}

	/// <summary>
	/// Write Base128 encoded variable sized unsigned integer of up to 64 bits
	/// </summary>
	/// <returns>number of bytes written</returns>
	int WriteVariableUInt64(UInt64 value)
	{
		int retval = 1;
		UInt64 num1 = (UInt64)value;
		while (num1 >= 0x80)
		{
			Write((byte)(num1 | 0x80));
			num1 = num1 >> 7;
			retval++;
		}
		Write((byte)num1);
		return retval;
	}

	/// <summary>
	/// Compress (lossy) a float in the range -1..1 using numberOfBits bits
	/// </summary>
	void WriteSignedSingle(float value, int numberOfBits)
	{
		//NetException.Assert(((value >= -1.0) && (value <= 1.0)), " WriteSignedSingle() must be passed a float in the range -1 to 1; val is " + value);

		float unit = (value + 1.0f) * 0.5f;
		int maxVal = (1 << numberOfBits) - 1;
		uint writeVal = (uint)(unit * (float)maxVal);

		Write(writeVal, numberOfBits);
	}

	/// <summary>
	/// Compress (lossy) a float in the range 0..1 using numberOfBits bits
	/// </summary>
	void WriteUnitSingle(float value, int numberOfBits)
	{
		//NetException.Assert(((value >= 0.0) && (value <= 1.0)), " WriteUnitSingle() must be passed a float in the range 0 to 1; val is " + value);

		int maxValue = (1 << numberOfBits) - 1;
		uint writeVal = (uint)(value * (float)maxValue);

		Write(writeVal, numberOfBits);
	}

	/// <summary>
	/// Compress a float within a specified range using a certain number of bits
	/// </summary>
	void WriteRangedSingle(float value, float min, float max, int numberOfBits)
	{
		//NetException.Assert(((value >= min) && (value <= max)), " WriteRangedSingle() must be passed a float in the range MIN to MAX; val is " + value);

		float range = max - min;
		float unit = ((value - min) / range);
		int maxVal = (1 << numberOfBits) - 1;
		Write((UInt32)((float)maxVal * unit), numberOfBits);
	}

	/// <summary>
	/// Writes an integer with the least amount of bits need for the specified range
	/// Returns number of bits written
	/// </summary>
	//int WriteRangedInteger(int min, int max, int value)
	//{
	//	//NetException.Assert(value >= min && value <= max, "Value not within min/max range!");

	//	uint range = (uint)(max - min);
	//	int numBits = NetUtility.BitsToHoldUInt(range);

	//	uint rvalue = (uint)(value - min);
	//	Write(rvalue, numBits);

	//	return numBits;
	//}

	/// <summary>
	/// Write a std::string
	/// </summary>
	void Write(std::wstring source)
	{
		if (source.length() == 0)
		{
			WriteVariableUInt32(0);
			return;
		}

		std::string str = System::Text::Encoding::UTF8::GetBytes(source);
		EnsureBufferSize(m_bitLength + 8 + ((int)str.length() * 8));
		WriteVariableUInt32((uint)str.length());
		Write((byte*)&str[0], (int)str.length());
	}

	void Write(std::string source)
	{
		if (source.length() == 0)
		{
			WriteVariableUInt32(0);
			return;
		}

		EnsureBufferSize(m_bitLength + 8 + ((int)source.length() * 8));
		WriteVariableUInt32((uint)source.length());
		Write((byte*)&source[0], (int)source.length());
	}


	/// <summary>
	/// Writes an endpoint description
	/// </summary>
	template<class ENDPOINT_TYPE>
	void Write(ENDPOINT_TYPE endpoint)
	{
		std::string addr = endpoint.address().to_string();
		Write((byte)4);
		Write((UInt32)::htonl(endpoint.address().to_v4().to_ulong()));
		//Write((ushort)::htons(endpoint.port()));
		Write((ushort)endpoint.port());
	}

	//void Write(IPEndPoint endPoint)
	//{
	//	byte* bytes = endPoint.Address.GetAddressBytes();
	//	Write((byte)bytes.Length);
	//	Write(bytes);
	//	Write((ushort)endPoint.Port);
	//}

	/// <summary>
	/// Writes the current local time to a message; readable (and convertable to local time) by the remote host using ReadTime()
	/// </summary>
	//void WriteTime(bool highPrecision)
	//{
	//	double localTime = NetTime.Now;
	//	if (highPrecision)
	//		Write(localTime);
	//	else
	//		Write((float)localTime);
	//}

	/// <summary>
	/// Writes a local timestamp to a message; readable (and convertable to local time) by the remote host using ReadTime()
	/// </summary>
	//void WriteTime(double localTime, bool highPrecision)
	//{
	//	if (highPrecision)
	//		Write(localTime);
	//	else
	//		Write((float)localTime);
	//}

	/// <summary>
	/// Pads data with enough bits to reach a full byte. Decreases cpu usage for subsequent byte writes.
	/// </summary>
	void WritePadBits()
	{
		m_bitLength = ((m_bitLength + 7) >> 3) * 8;
		EnsureBufferSize(m_bitLength);
	}

	/// <summary>
	/// Pads data with the specified number of bits.
	/// </summary>
	void WritePadBits(int numberOfBits)
	{
		m_bitLength += numberOfBits;
		EnsureBufferSize(m_bitLength);
	}

	/// <summary>
	/// Append all the bits of message to this message
	/// </summary>
	//void Write(NetBuffer buffer)
	//{
	//	EnsureBufferSize(m_bitLength + (buffer.LengthBytes * 8));

	//	Write(buffer.m_data, 0, buffer.LengthBytes);

	//	// did we write excessive bits?
	//	int bitsInLastByte = (buffer.m_bitLength % 8);
	//	if (bitsInLastByte != 0)
	//	{
	//		int excessBits = 8 - bitsInLastByte;
	//		m_bitLength -= excessBits;
	//	}
	//}
};