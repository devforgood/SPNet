#pragma once

#include "NetBuffer.h"

class NetIncomingMessage : public NetBuffer
{
public:

	template<class MESSAGE_TYPE>
	inline bool Parse(MESSAGE_TYPE * msg)
	{
		if(BodyLength()<=0 || LengthBytes()-PositionInBytes() < BodyLength())
		{
			msg->Clear();
			return true;
		}

		google::protobuf::io::ArrayInputStream input((const void*)&m_data[PositionInBytes()], BodyLength());
		google::protobuf::io::CodedInputStream input_stream(&input);
		if(msg->ParseFromCodedStream(&input_stream)==false)
		{
			LOG(INFO) << __FUNCTION__ << "() error";
			return false;
		}

		m_readPosition += (8 * BodyLength());

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// read

	bool ReadBoolean()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 1, c_readOverflowError);
		byte retval = NetBitWriter::ReadByte(m_data, 1, m_readPosition);
		m_readPosition += 1;
		return (retval > 0 ? true : false);
	}

	bool ReadBoolean(bool & result)
	{
		//if (m_bitLength - m_readPosition < 8)
		//{
		//	result = 0;
		//	return false;
		//}
		//result = (byte)ReadByte();


		if (m_bitLength - m_readPosition < 1)
		{
			result = 0;
			return false;
		}
		result = ReadBoolean();

		return true;
	}

	/// <summary>
	/// Reads a byte
	/// </summary>
	byte ReadByte()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 8, c_readOverflowError);
		byte retval = NetBitWriter::ReadByte(m_data, 8, m_readPosition);
		m_readPosition += 8;
		return retval;
	}

	/// <summary>
	/// Reads a byte and returns true or false for success
	/// </summary>
	bool ReadByte(byte & result)
	{
		if (m_bitLength - m_readPosition < 8)
		{
			result = 0;
			return false;
		}
		result = NetBitWriter::ReadByte(m_data, 8, m_readPosition);
		m_readPosition += 8;
		return true;
	}

	/// <summary>
	/// Reads a signed byte
	/// </summary>
	sbyte ReadSByte()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 8, c_readOverflowError);
		byte retval = NetBitWriter::ReadByte(m_data, 8, m_readPosition);
		m_readPosition += 8;
		return (sbyte)retval;
	}

	/// <summary>
	/// Reads 1 to 8 bits into a byte
	/// </summary>
	byte ReadByte(int numberOfBits)
	{
		//NetException.Assert(numberOfBits > 0 && numberOfBits <= 8, "ReadByte(bits) can only read between 1 and 8 bits");
		byte retval = NetBitWriter::ReadByte(m_data, numberOfBits, m_readPosition);
		m_readPosition += numberOfBits;
		return retval;
	}

	/// <summary>
	/// Reads the specified number of bytes
	/// </summary>
	byte* ReadBytes(int numberOfBytes)
	{
		//NetException.Assert(m_bitLength - m_readPosition + 7 >= (numberOfBytes * 8), c_readOverflowError);

		byte* retval = new byte[numberOfBytes];
		NetBitWriter::ReadBytes(m_data, numberOfBytes, m_readPosition, retval, 0);
		m_readPosition += (8 * numberOfBytes);
		return retval;
	}

	byte* ReadBytesZeroCopy(int numberOfBytes)
	{
		//NetException.Assert(m_bitLength - m_readPosition + 7 >= (numberOfBytes * 8), c_readOverflowError);

		byte * retval =  &m_data[m_readPosition];
		m_readPosition += (8 * numberOfBytes);
		return retval;
	}

	/// <summary>
	/// Reads the specified number of bytes and returns true for success
	/// </summary>
	bool ReadBytes(int numberOfBytes, byte** result)
	{
		if ((m_bitLength - m_readPosition + 7) < (numberOfBytes * 8))
		{
			return false;
		}

		*result = new byte[numberOfBytes];
		NetBitWriter::ReadBytes(m_data, numberOfBytes, m_readPosition, *result, 0);
		m_readPosition += (8 * numberOfBytes);
		return true;
	}

	/// <summary>
	/// Reads the specified number of bytes into a preallocated array
	/// </summary>
	/// <param name="into">The destination array</param>
	/// <param name="offset">The offset where to start writing in the destination array</param>
	/// <param name="numberOfBytes">The number of bytes to read</param>
	void ReadBytes(byte* into, int offset, int numberOfBytes)
	{
		//NetException.Assert(m_bitLength - m_readPosition + 7 >= (numberOfBytes * 8), c_readOverflowError);
		//NetException.Assert(offset + numberOfBytes <= into.Length);

		NetBitWriter::ReadBytes(m_data, numberOfBytes, m_readPosition, into, offset);
		m_readPosition += (8 * numberOfBytes);
		return;
	}

	/// <summary>
	/// Reads the specified number of bits into a preallocated array
	/// </summary>
	/// <param name="into">The destination array</param>
	/// <param name="offset">The offset where to start writing in the destination array</param>
	/// <param name="numberOfBits">The number of bits to read</param>
	void ReadBits(byte* into, int offset, int numberOfBits)
	{
		//NetException.Assert(m_bitLength - m_readPosition >= numberOfBits, c_readOverflowError);
		//NetException.Assert(offset + NetUtility.BytesToHoldBits(numberOfBits) <= into.Length);

		int numberOfWholeBytes = numberOfBits / 8;
		int extraBits = numberOfBits - (numberOfWholeBytes * 8);

		NetBitWriter::ReadBytes(m_data, numberOfWholeBytes, m_readPosition, into, offset);
		m_readPosition += (8 * numberOfWholeBytes);

		if (extraBits > 0)
			into[offset + numberOfWholeBytes] = ReadByte(extraBits);

		return;
	}

	/// <summary>
	/// Reads a 16 bit signed integer written using Write(Int16)
	/// </summary>
	Int16 ReadInt16()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 16, c_readOverflowError);
		uint retval = NetBitWriter::ReadUInt16(m_data, 16, m_readPosition);
		m_readPosition += 16;
		return (short)retval;
	}

	bool ReadInt16(Int16 & result)
	{
		if (m_bitLength - m_readPosition < 16)
		{
			result = 0;
			return false;
		}

		result = (Int16)NetBitWriter::ReadUInt16(m_data, 16, m_readPosition);
		m_readPosition += 16;
		return true;
	}

	/// <summary>
	/// Reads a 16 bit unsigned integer written using Write(UInt16)
	/// </summary>
	UInt16 ReadUInt16()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 16, c_readOverflowError);
		uint retval = NetBitWriter::ReadUInt16(m_data, 16, m_readPosition);
		m_readPosition += 16;
		return (ushort)retval;
	}

	/// <summary>
	/// Reads a 32 bit signed integer written using Write(Int32)
	/// </summary>
	Int32 ReadInt32()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 32, c_readOverflowError);
		uint retval = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
		m_readPosition += 32;
		return (Int32)retval;
	}

	/// <summary>
	/// Reads a 32 bit signed integer written using Write(Int32)
	/// </summary>
	bool ReadInt32(Int32 & result)
	{
		if (m_bitLength - m_readPosition < 32)
		{
			result = 0;
			return false;
		}

		result = (Int32)NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
		m_readPosition += 32;
		return true;
	}

	/// <summary>
	/// Reads a signed integer stored in 1 to 32 bits, written using Write(Int32, Int32)
	/// </summary>

	//Int32 ReadInt32(int numberOfBits)
	//{
	//	//NetException.Assert(numberOfBits > 0 && numberOfBits <= 32, "ReadInt32(bits) can only read between 1 and 32 bits");
	//	//NetException.Assert(m_bitLength - m_readPosition >= numberOfBits, c_readOverflowError);

	//	uint retval = NetBitWriter::ReadUInt32(m_data, numberOfBits, m_readPosition);
	//	m_readPosition += numberOfBits;

	//	if (numberOfBits == 32)
	//		return (int)retval;

	//	int signBit = 1 << (numberOfBits - 1);
	//	if ((retval & signBit) == 0)
	//		return (int)retval; // positive

	//	// negative
	//	// unchecked
	//	{
	//		uint mask = ((uint)-1) >> (33 - numberOfBits);
	//		uint tmp = (retval & mask) + 1;
	//		return -((int)tmp);
	//	}
	//}

	/// <summary>
	/// Reads an 32 bit unsigned integer written using Write(UInt32)
	/// </summary>
	UInt32 ReadUInt32()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 32, c_readOverflowError);
		uint retval = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
		m_readPosition += 32;
		return retval;
	}

	/// <summary>
	/// Reads an 32 bit unsigned integer written using Write(UInt32) and returns true for success
	/// </summary>
	bool ReadUInt32(UInt32 & result)
	{
		if (m_bitLength - m_readPosition < 32)
		{
			result = 0;
			return false;
		}
		result = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
		m_readPosition += 32;
		return true;
	}

	/// <summary>
	/// Reads an unsigned integer stored in 1 to 32 bits, written using Write(UInt32, Int32)
	/// </summary>
	UInt32 ReadUInt32(int numberOfBits)
	{
		//NetException.Assert(numberOfBits > 0 && numberOfBits <= 32, "ReadUInt32(bits) can only read between 1 and 32 bits");
		//NetException.Assert(m_bitLength - m_readBitPtr >= numberOfBits, "tried to read past buffer size");

		UInt32 retval = NetBitWriter::ReadUInt32(m_data, numberOfBits, m_readPosition);
		m_readPosition += numberOfBits;
		return retval;
	}

	/// <summary>
	/// Reads a 64 bit unsigned integer written using Write(UInt64)
	/// </summary>
	UInt64 ReadUInt64()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 64, c_readOverflowError);

		ulong low = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
		m_readPosition += 32;
		ulong high = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);

		ulong retval = low + (high << 32);

		m_readPosition += 32;
		return retval;
	}

	/// <summary>
	/// Reads a 64 bit signed integer written using Write(Int64)
	/// </summary>
	Int64 ReadInt64()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 64, c_readOverflowError);
		//unchecked
		{
			ulong retval = ReadUInt64();
			Int64 longRetval = (Int64)retval;
			return longRetval;
		}
	}

	void ReadInt64(Int64 & result)
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 64, c_readOverflowError);
		//unchecked
		{
			result = ReadUInt64();
		}
	}

	/// <summary>
	/// Reads an unsigned integer stored in 1 to 64 bits, written using Write(UInt64, Int32)
	/// </summary>
	UInt64 ReadUInt64(int numberOfBits)
	{
		//NetException.Assert(numberOfBits > 0 && numberOfBits <= 64, "ReadUInt64(bits) can only read between 1 and 64 bits");
		//NetException.Assert(m_bitLength - m_readPosition >= numberOfBits, c_readOverflowError);

		ulong retval;
		if (numberOfBits <= 32)
		{
			retval = (ulong)NetBitWriter::ReadUInt32(m_data, numberOfBits, m_readPosition);
		}
		else
		{
			retval = NetBitWriter::ReadUInt32(m_data, 32, m_readPosition);
			retval |= ((ulong)NetBitWriter::ReadUInt32(m_data, numberOfBits - 32, m_readPosition)) << 32;
		}
		m_readPosition += numberOfBits;
		return retval;
	}

	/// <summary>
	/// Reads a signed integer stored in 1 to 64 bits, written using Write(Int64, Int32)
	/// </summary>
	Int64 ReadInt64(int numberOfBits)
	{
		//NetException.Assert(((numberOfBits > 0) && (numberOfBits <= 64)), "ReadInt64(bits) can only read between 1 and 64 bits");
		return (Int64)ReadUInt64(numberOfBits);
	}

	/// <summary>
	/// Reads a 32 bit floating point value written using Write(Single)
	/// </summary>
	float ReadFloat()
	{
		return ReadSingle();
	}

	bool ReadFloat(float & result)
	{
		if (m_bitLength - m_readPosition < 32)
		{
			result = 0.0f;
			return false;
		}

		result = ReadFloat();
		return true;
	}


	static int ToInt32 (byte * value, int startIndex) 
	{  
		//if( value == null)  { 
		//	ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);  
		//} 

		//if ((uint) startIndex >= value.Length) { 
		//	ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_Index);  
		//} 

		//if (startIndex > value.Length -4) {  
		//	ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall); 
		//}  

		byte * pbyte = &value[startIndex];

		if( startIndex % 4 == 0) 
		{ // data is aligned 
			return *((int *) pbyte);  
		} 
		else 
		{  
#if BIGENDIAN
			return (*pbyte << 24) | (*(pbyte + 1) << 16)  | (*(pbyte + 2) << 8) | (*(pbyte + 3)); 
#else // BIGENDIAN
			return (*pbyte) | (*(pbyte + 1) << 8)  | (*(pbyte + 2) << 16) | (*(pbyte + 3) << 24); 
#endif // BIGENDIAN
		}  

		return 0;
	}  

	static Int64 ToInt64 (byte * value, int startIndex) 
	{  
		//if( value == null)  { 
		//	ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value); 
		//} 

		//if ((uint) startIndex >= value.Length) { 
		//	ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_Index);  
		//}  

		//if (startIndex > value.Length -8) {  
		//	ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall); 
		//} 

		byte * pbyte = &value[startIndex];

		if( startIndex % 8 == 0) 
		{ // data is aligned 
			return *((Int64 *) pbyte);  
		}  
		else 
		{ 
#if BIGENDIAN
			int i1 = (*pbyte << 24) | (*(pbyte + 1) << 16)  | (*(pbyte + 2) << 8) | (*(pbyte + 3));  
			int i2  = (*(pbyte+4) << 24) | (*(pbyte + 5) << 16)  | (*(pbyte + 6) << 8) | (*(pbyte + 7));  
			return (uint)i2 | ((Int64)i1 << 32); 

#else // BIGENDIAN
			int i1 = (*pbyte) | (*(pbyte + 1) << 8)  | (*(pbyte + 2) << 16) | (*(pbyte + 3) << 24); 
			int i2  = (*(pbyte+4)) | (*(pbyte + 5) << 8)  | (*(pbyte + 6) << 16) | (*(pbyte + 7) << 24); 
			return (uint)i1 | ((Int64)i2 << 32); 
#endif // BIGENDIAN
		} 
		return 0;
	} 

	static float ToSingle (byte * value, int startIndex)  
	{ 
		int val = ToInt32(value, startIndex);  
		return *(float*)&val;  
	} 

	static double ToDouble (byte * value, int startIndex) 
	{ 
		Int64 val = ToInt64(value, startIndex);  
		return *(double*)&val; 
	} 

	/// <summary>
	/// Reads a 32 bit floating point value written using Write(Single)
	/// </summary>
	float ReadSingle()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 32, c_readOverflowError);

		if ((m_readPosition & 7) == 0) // read directly
		{
			float retval = ToSingle(m_data, m_readPosition >> 3);
			m_readPosition += 32;
			return retval;
		}

		byte* bytes = ReadBytes(4);
		return ToSingle(bytes, 0);
	}

	/// <summary>
	/// Reads a 32 bit floating point value written using Write(Single)
	/// </summary>
	bool ReadSingle(float & result)
	{
		if (m_bitLength - m_readPosition < 32)
		{
			result = 0.0f;
			return false;
		}

		if ((m_readPosition & 7) == 0) // read directly
		{
			result = ToSingle(m_data, m_readPosition >> 3);
			m_readPosition += 32;
			return true;
		}

		byte* bytes = ReadBytes(4);
		result = ToSingle(bytes, 0);
		return true;
	}

	/// <summary>
	/// Reads a 64 bit floating point value written using Write(Double)
	/// </summary>
	double ReadDouble()
	{
		//NetException.Assert(m_bitLength - m_readPosition >= 64, c_readOverflowError);

		if ((m_readPosition & 7) == 0) // read directly
		{
			// read directly
			double retval = ToDouble(m_data, m_readPosition >> 3);
			m_readPosition += 64;
			return retval;
		}

		byte* bytes = ReadBytes(8);
		return ToDouble(bytes, 0);
	}

	bool ReadDouble(double & result)
	{
		if (m_bitLength - m_readPosition < 64)
		{
			result = 0.0f;
			return false;
		}
		result = ReadDouble();
		return true;
	}

	//
	// Variable bit count
	//

	/// <summary>
	/// Reads a variable sized UInt32 written using WriteVariableUInt32()
	/// </summary>
	uint ReadVariableUInt32()
	{
		int num1 = 0;
		int num2 = 0;
		while (m_bitLength - m_readPosition >= 8)
		{
			byte num3 = ReadByte();
			num1 |= (num3 & 0x7f) << num2;
			num2 += 7;
			if ((num3 & 0x80) == 0)
				return (uint)num1;
		}

		// ouch; failed to find enough bytes; malformed variable length number?
		return (uint)num1;
	}

	/// <summary>
	/// Reads a variable sized UInt32 written using WriteVariableUInt32() and returns true for success
	/// </summary>
	bool ReadVariableUInt32(uint & result)
	{
		int num1 = 0;
		int num2 = 0;
		while (m_bitLength - m_readPosition >= 8)
		{
			byte num3;
			if (ReadByte(num3) == false)
			{
				result = 0;
				return false;
			}
			num1 |= (num3 & 0x7f) << num2;
			num2 += 7;
			if ((num3 & 0x80) == 0)
			{
				result = (uint)num1;
				return true;
			}
		}
		result = (uint)num1;
		return false;
	}

	/// <summary>
	/// Reads a variable sized Int32 written using WriteVariableInt32()
	/// </summary>
	int ReadVariableInt32()
	{
		uint n = ReadVariableUInt32();
		return (int)(n >> 1) ^ -(int)(n & 1); // decode zigzag
	}

	/// <summary>
	/// Reads a variable sized Int64 written using WriteVariableInt64()
	/// </summary>
	Int64 ReadVariableInt64()
	{
		UInt64 n = ReadVariableUInt64();
		return (Int64)(n >> 1) ^ -(Int64)(n & 1); // decode zigzag
	}

	/// <summary>
	/// Reads a variable sized UInt32 written using WriteVariableInt64()
	/// </summary>
	UInt64 ReadVariableUInt64()
	{
		UInt64 num1 = 0;
		int num2 = 0;
		while (m_bitLength - m_readPosition >= 8)
		{
			//if (num2 == 0x23)
			//	throw new FormatException("Bad 7-bit encoded integer");

			byte num3 = ReadByte();
			num1 |= ((UInt64)num3 & 0x7f) << num2;
			num2 += 7;
			if ((num3 & 0x80) == 0)
				return num1;
		}

		// ouch; failed to find enough bytes; malformed variable length number?
		return num1;
	}

	/// <summary>
	/// Reads a 32 bit floating point value written using WriteSignedSingle()
	/// </summary>
	/// <param name="numberOfBits">The number of bits used when writing the value</param>
	/// <returns>A floating point value larger or equal to -1 and smaller or equal to 1</returns>
	float ReadSignedSingle(int numberOfBits)
	{
		uint encodedVal = ReadUInt32(numberOfBits);
		int maxVal = (1 << numberOfBits) - 1;
		return ((float)(encodedVal + 1) / (float)(maxVal + 1) - 0.5f) * 2.0f;
	}

	/// <summary>
	/// Reads a 32 bit floating point value written using WriteUnitSingle()
	/// </summary>
	/// <param name="numberOfBits">The number of bits used when writing the value</param>
	/// <returns>A floating point value larger or equal to 0 and smaller or equal to 1</returns>
	float ReadUnitSingle(int numberOfBits)
	{
		uint encodedVal = ReadUInt32(numberOfBits);
		int maxVal = (1 << numberOfBits) - 1;
		return (float)(encodedVal + 1) / (float)(maxVal + 1);
	}

	/// <summary>
	/// Reads a 32 bit floating point value written using WriteRangedSingle()
	/// </summary>
	/// <param name="min">The minimum value used when writing the value</param>
	/// <param name="max">The maximum value used when writing the value</param>
	/// <param name="numberOfBits">The number of bits used when writing the value</param>
	/// <returns>A floating point value larger or equal to MIN and smaller or equal to MAX</returns>
	float ReadRangedSingle(float min, float max, int numberOfBits)
	{
		float range = max - min;
		int maxVal = (1 << numberOfBits) - 1;
		float encodedVal = (float)ReadUInt32(numberOfBits);
		float unit = encodedVal / (float)maxVal;
		return min + (unit * range);
	}

	/// <summary>
	/// Reads a 32 bit integer value written using WriteRangedInteger()
	/// </summary>
	/// <param name="min">The minimum value used when writing the value</param>
	/// <param name="max">The maximum value used when writing the value</param>
	/// <returns>A signed integer value larger or equal to MIN and smaller or equal to MAX</returns>
	//int ReadRangedInteger(int min, int max)
	//{
	//	uint range = (uint)(max - min);
	//	int numBits = NetUtility.BitsToHoldUInt(range);

	//	uint rvalue = ReadUInt32(numBits);
	//	return (int)(min + rvalue);
	//}

	/// <summary>
	/// Reads a string written using Write(string)
	/// </summary>
	std::wstring ReadString()
	{
		int byteLen = (int)ReadVariableUInt32();

		if (byteLen <= 0)
			return L"";

		if ((ulong)(m_bitLength - m_readPosition) < ((ulong)byteLen * 8))
		{
			// not enough data
			m_readPosition = m_bitLength;
			return L""; // unfortunate; but we need to protect against DDOS
		}

		if ((m_readPosition & 7) == 0)
		{
			// read directly
			std::wstring retval = System::Text::Encoding::UTF8::GetString(std::string((char*)&m_data[m_readPosition >> 3],(int)byteLen));
			m_readPosition += (8 * byteLen);
			return retval;
		}

		byte* bytes = ReadBytes(byteLen);
		return System::Text::Encoding::UTF8::GetString(std::string((char*)&bytes[0],byteLen));
	}

	/// <summary>
	/// Reads a string written using Write(std::string) and returns true for success
	/// </summary>
	bool ReadString(std::wstring & result)
	{
		uint byteLen;
		if (ReadVariableUInt32(byteLen) == false)
		{
			result = L"";
			return false;
		}

		if (byteLen <= 0)
		{
			result = L"";
			return true;
		}

		if ((uint)(m_bitLength - m_readPosition) < (byteLen * 8))
		{
			result = L"";
			return false;
		}

		if ((m_readPosition & 7) == 0)
		{
			// read directly
			result = System::Text::Encoding::UTF8::GetString(std::string((char*)&m_data[m_readPosition >> 3],(int)byteLen));
			m_readPosition += (8 * (int)byteLen);
			return true;
		}

		byte* bytes = nullptr;
		if (ReadBytes((int)byteLen, &bytes) == false)
		{
			result = L"";
			return false;
		}

		result = System::Text::Encoding::UTF8::GetString(std::string((char*)&bytes[0],byteLen));
		return true;
	}

	bool ReadString(std::string & result)
	{
		uint byteLen;
		if (ReadVariableUInt32(byteLen) == false)
		{
			result = "";
			return false;
		}

		if (byteLen <= 0)
		{
			result = "";
			return true;
		}

		if ((uint)(m_bitLength - m_readPosition) < (byteLen * 8))
		{
			result = "";
			return false;
		}

		if ((m_readPosition & 7) == 0)
		{
			// read directly
			result = std::string((char*)&m_data[m_readPosition >> 3],(int)byteLen);
			m_readPosition += (8 * (int)byteLen);
			return true;
		}

		byte* bytes = nullptr;
		if (ReadBytes((int)byteLen, &bytes) == false)
		{
			result = "";
			return false;
		}

		result = std::string((char*)&bytes[0],byteLen);
		return true;
	}

	/// <summary>
	/// Reads a value, in local time comparable to NetTime.Now, written using WriteTime() for the connection supplied
	/// </summary>
	//double ReadTime(NetConnection connection, bool highPrecision)
	//{
	//	double remoteTime = (highPrecision ? ReadDouble() : (double)ReadSingle());

	//	if (connection == null)
	//		throw new NetException("Cannot call ReadTime() on message without a connected sender (ie. unconnected messages)");

	//	// lets bypass NetConnection.GetLocalTime for speed
	//	return remoteTime - connection.m_remoteTimeOffset;
	//}

	/// <summary>
	/// Reads a stored IPv4 endpoint description
	/// </summary>
	template<class ENDPOINT_TYPE, class ADDRESS_TYPE>
	ENDPOINT_TYPE ReadIPEndPoint()
	{
		ENDPOINT_TYPE endpoint;
		byte len = ReadByte();
		//byte* addressBytes = ;
		ADDRESS_TYPE addr(::ntohl(*(unsigned long*)ReadBytes(len)));
		int port = (int)ReadUInt16();
		endpoint.address(addr);
		//endpoint.port(::ntohs(port));
		endpoint.port(port);

		//IPAddress address = new IPAddress(addressBytes);
		//return new IPEndPoint(address, port);
		return endpoint;
	}

	/// <summary>
	/// Pads data with enough bits to reach a full byte. Decreases cpu usage for subsequent byte writes.
	/// </summary>
	void SkipPadBits()
	{
		m_readPosition = ((m_readPosition + 7) >> 3) * 8;
	}

	/// <summary>
	/// Pads data with enough bits to reach a full byte. Decreases cpu usage for subsequent byte writes.
	/// </summary>
	void ReadPadBits()
	{
		m_readPosition = ((m_readPosition + 7) >> 3) * 8;
	}

	/// <summary>
	/// Pads data with the specified number of bits.
	/// </summary>
	void SkipPadBits(int numberOfBits)
	{
		m_readPosition += numberOfBits;
	}
	void SkipPadBytes(int numberOfBytes)
	{
		m_readPosition += (numberOfBytes*8);
	}

	bool IsReadable(void)
	{
		if(m_bitLength > m_readPosition)
		{
			if((m_bitLength - m_readPosition) >= (header_length*8))
				return true;
		}

		return false;
	}

	int PacketLength()
	{
		m_lastPacket = &m_data[PositionInBytes()];
		m_lastPacketLength = NetBitWriter::ReadUInt16(m_data, 16, m_readPosition);
		
		m_expectReadPosition = (m_lastPacketLength*8) + m_readPosition + (header_length*8);

		return m_lastPacketLength+header_length;
	}

	bool CheckReadPosition() const
	{
		if(m_expectReadPosition == m_readPosition)
			return true;
		return false;
	}

	void CorrectReadPosition()
	{
		m_readPosition = m_expectReadPosition;
	}

	int BodyLength() const
	{
		return m_lastPacketLength-packet_index_length;
	}
};