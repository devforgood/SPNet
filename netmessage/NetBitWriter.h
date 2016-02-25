#pragma once

#include <assert.h>
#include <memory.h>

class NetBitWriter
{

public:
	/// <summary>
	/// Read 1-8 bits from a buffer into a byte
	/// </summary>
	static unsigned char ReadByte(unsigned char* fromBuffer, int numberOfBits, int readBitOffset)
	{
		//assert(((numberOfBits > 0) && (numberOfBits < 9)), "Read() can only read between 1 and 8 bits");

		int bytePtr = readBitOffset >> 3;
		int startReadAtIndex = readBitOffset - (bytePtr * 8); // (readBitOffset % 8);

		if (startReadAtIndex == 0 && numberOfBits == 8)
			return fromBuffer[bytePtr];

		// mask away unused bits lower than (right of) relevant bits in first byte
		unsigned char returnValue = (unsigned char)(fromBuffer[bytePtr] >> startReadAtIndex);

		int numberOfBitsInSecondByte = numberOfBits - (8 - startReadAtIndex);

		if (numberOfBitsInSecondByte < 1)
		{
			// we don't need to read from the second byte, but we DO need
			// to mask away unused bits higher than (left of) relevant bits
			return (unsigned char)(returnValue & (255 >> (8 - numberOfBits)));
		}

		unsigned char second = fromBuffer[bytePtr + 1];

		// mask away unused bits higher than (left of) relevant bits in second byte
		second &= (unsigned char)(255 >> (8 - numberOfBitsInSecondByte));

		return (unsigned char)(returnValue | (unsigned char)(second << (numberOfBits - numberOfBitsInSecondByte)));
	}

	/// <summary>
	/// Read several bytes from a buffer
	/// </summary>
	static void ReadBytes(unsigned char* fromBuffer, int numberOfBytes, int readBitOffset, unsigned char* destination, int destinationByteOffset)
	{
		int readPtr = readBitOffset >> 3;
		int startReadAtIndex = readBitOffset - (readPtr * 8); // (readBitOffset % 8);

		if (startReadAtIndex == 0)
		{
			memcpy_s(&destination[destinationByteOffset], numberOfBytes,  &fromBuffer[readPtr], numberOfBytes);
			return;
		}

		int secondPartLen = 8 - startReadAtIndex;
		int secondMask = 255 >> secondPartLen;

		for (int i = 0; i < numberOfBytes; i++)
		{
			// mask away unused bits lower than (right of) relevant bits in byte
			int b = fromBuffer[readPtr] >> startReadAtIndex;

			readPtr++;

			// mask away unused bits higher than (left of) relevant bits in second byte
			int second = fromBuffer[readPtr] & secondMask;

			destination[destinationByteOffset++] = (unsigned char)(b | (second << secondPartLen));
		}

		return;
	}

	/// <summary>
	/// Write 0-8 bits of data to buffer
	/// </summary>
	static void WriteByte(unsigned char source, int numberOfBits, unsigned char* destination, int destBitOffset)
	{
		if (numberOfBits == 0)
			return;

		//assert(((numberOfBits >= 0) && (numberOfBits <= 8)), "Must write between 0 and 8 bits!");

		// Mask out all the bits we dont want
		source = (unsigned char)(source & (0xFF >> (8 - numberOfBits)));

		int p = destBitOffset >> 3;
		int bitsUsed = destBitOffset & 0x7; // mod 8
		int bitsFree = 8 - bitsUsed;
		int bitsLeft = bitsFree - numberOfBits;

		// Fast path, everything fits in the first byte
		if (bitsLeft >= 0)
		{
			int mask = (0xFF >> bitsFree) | (0xFF << (8 - bitsLeft));

			destination[p] = (unsigned char)(
				// Mask out lower and upper bits
				(destination[p] & mask) |

				// Insert new bits
				(source << bitsUsed)
				);

			return;
		}

		destination[p] = (unsigned char)(
			// Mask out upper bits
			(destination[p] & (0xFF >> bitsFree)) |

			// Write the lower bits to the upper bits in the first byte
			(source << bitsUsed)
			);

		p += 1;

		destination[p] = (unsigned char)(
			// Mask out lower bits
			(destination[p] & (0xFF << (numberOfBits - bitsFree))) |

			// Write the upper bits to the lower bits of the second byte
			(source >> bitsFree)
			);
	}

	/// <summary>
	/// Write several whole bytes
	/// </summary>
	static void WriteBytes(unsigned char* source, int sourceByteOffset, int numberOfBytes, unsigned char* destination, int destBitOffset)
	{
		int dstBytePtr = destBitOffset >> 3;
		int firstPartLen = (destBitOffset % 8);

		if (firstPartLen == 0)
		{
			//Buffer.BlockCopy(source, sourceByteOffset, destination, dstBytePtr, numberOfBytes);
			memcpy_s(&destination[dstBytePtr], numberOfBytes,  &source[sourceByteOffset], numberOfBytes);

			return;
		}

		int lastPartLen = 8 - firstPartLen;

		for (int i = 0; i < numberOfBytes; i++)
		{
			unsigned char src = source[sourceByteOffset + i];

			// write last part of this byte
			destination[dstBytePtr] &= (unsigned char)(255 >> lastPartLen); // clear before writing
			destination[dstBytePtr] |= (unsigned char)(src << firstPartLen); // write first half

			dstBytePtr++;

			// write first part of next byte
			destination[dstBytePtr] &= (unsigned char)(255 << firstPartLen); // clear before writing
			destination[dstBytePtr] |= (unsigned char)(src >> lastPartLen); // write second half
		}

		return;
	}

	/// <summary>
	/// Reads an unsigned 16 bit integer
	/// </summary>
	static unsigned short ReadUInt16(unsigned char* fromBuffer, int numberOfBits, int readBitOffset)
	{
		//Debug.Assert(((numberOfBits > 0) && (numberOfBits <= 16)), "ReadUInt16() can only read between 1 and 16 bits");
		unsigned short returnValue;
		if (numberOfBits <= 8)
		{
			returnValue = ReadByte(fromBuffer, numberOfBits, readBitOffset);
			return returnValue;
		}
		returnValue = ReadByte(fromBuffer, 8, readBitOffset);
		numberOfBits -= 8;
		readBitOffset += 8;

		if (numberOfBits <= 8)
		{
			returnValue |= (unsigned short)(ReadByte(fromBuffer, numberOfBits, readBitOffset) << 8);
		}

#if BIGENDIAN
		// reorder bytes
		unsigned int retVal = returnValue;
		retVal = ((retVal & 0x0000ff00) >> 8) | ((retVal & 0x000000ff) << 8);
		return (unsigned short)retVal;
#else
		return returnValue;
#endif
	}

	/// <summary>
	/// Reads the specified number of bits into an UInt32
	/// </summary>

	static unsigned int ReadUInt32(unsigned char* fromBuffer, int numberOfBits, int readBitOffset)
	{
		//assert(((numberOfBits > 0) && (numberOfBits <= 32)), "ReadUInt32() can only read between 1 and 32 bits");
		unsigned int returnValue;
		if (numberOfBits <= 8)
		{
			returnValue = ReadByte(fromBuffer, numberOfBits, readBitOffset);
			return returnValue;
		}
		returnValue = ReadByte(fromBuffer, 8, readBitOffset);
		numberOfBits -= 8;
		readBitOffset += 8;

		if (numberOfBits <= 8)
		{
			returnValue |= (unsigned int)(ReadByte(fromBuffer, numberOfBits, readBitOffset) << 8);
			return returnValue;
		}
		returnValue |= (unsigned int)(ReadByte(fromBuffer, 8, readBitOffset) << 8);
		numberOfBits -= 8;
		readBitOffset += 8;

		if (numberOfBits <= 8)
		{
			unsigned int r = ReadByte(fromBuffer, numberOfBits, readBitOffset);
			r <<= 16;
			returnValue |= r;
			return returnValue;
		}
		returnValue |= (unsigned int)(ReadByte(fromBuffer, 8, readBitOffset) << 16);
		numberOfBits -= 8;
		readBitOffset += 8;

		returnValue |= (unsigned int)(ReadByte(fromBuffer, numberOfBits, readBitOffset) << 24);

#if BIGENDIAN
		// reorder bytes
		return
			((returnValue & 0xff000000) >> 24) |
			((returnValue & 0x00ff0000) >> 8) |
			((returnValue & 0x0000ff00) << 8) |
			((returnValue & 0x000000ff) << 24);
#else
		return returnValue;
#endif
	}

	//[CLSCompliant(false)]
	//public static unsigned long long ReadUInt64(byte* fromBuffer, int numberOfBits, int readBitOffset)

	/// <summary>
	/// Writes an unsigned 16 bit integer
	/// </summary>
	static void WriteUInt16(unsigned short source, int numberOfBits, unsigned char* destination, int destinationBitOffset)
	{
		if (numberOfBits == 0)
			return;

		//assert((numberOfBits >= 0 && numberOfBits <= 16), "numberOfBits must be between 0 and 16");
#if BIGENDIAN
		// reorder bytes
		unsigned int intSource = source;
		intSource = ((intSource & 0x0000ff00) >> 8) | ((intSource & 0x000000ff) << 8);
		source = (unsigned short)intSource;
#endif
		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)source, numberOfBits, destination, destinationBitOffset);
			return;
		}

		NetBitWriter::WriteByte((unsigned char)source, 8, destination, destinationBitOffset);

		numberOfBits -= 8;
		if (numberOfBits > 0)
			NetBitWriter::WriteByte((unsigned char)(source >> 8), numberOfBits, destination, destinationBitOffset + 8);
	}

	/// <summary>
	/// Writes the specified number of bits into a byte array
	/// </summary>
	static int WriteUInt32(unsigned int source, int numberOfBits, unsigned char* destination, int destinationBitOffset)
	{
#if BIGENDIAN
		// reorder bytes
		source = ((source & 0xff000000) >> 24) |
			((source & 0x00ff0000) >> 8) |
			((source & 0x0000ff00) << 8) |
			((source & 0x000000ff) << 24);
#endif

		int returnValue = destinationBitOffset + numberOfBits;
		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)source, numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)source, 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 8), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 8), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 16), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 16), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		NetBitWriter::WriteByte((unsigned char)(source >> 24), numberOfBits, destination, destinationBitOffset);
		return returnValue;
	}

	/// <summary>
	/// Writes the specified number of bits into a byte array
	/// </summary>
	static int WriteUInt64(unsigned long long source, int numberOfBits, unsigned char* destination, int destinationBitOffset)
	{
#if BIGENDIAN
		source = ((source & 0xff00000000000000L) >> 56) |
			((source & 0x00ff000000000000L) >> 40) |
			((source & 0x0000ff0000000000L) >> 24) |
			((source & 0x000000ff00000000L) >> 8) |
			((source & 0x00000000ff000000L) << 8) |
			((source & 0x0000000000ff0000L) << 24) |
			((source & 0x000000000000ff00L) << 40) |
			((source & 0x00000000000000ffL) << 56);
#endif

		int returnValue = destinationBitOffset + numberOfBits;
		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)source, numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)source, 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 8), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 8), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 16), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 16), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 24), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 24), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 32), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 32), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 40), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 40), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 48), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 48), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		if (numberOfBits <= 8)
		{
			NetBitWriter::WriteByte((unsigned char)(source >> 56), numberOfBits, destination, destinationBitOffset);
			return returnValue;
		}
		NetBitWriter::WriteByte((unsigned char)(source >> 56), 8, destination, destinationBitOffset);
		destinationBitOffset += 8;
		numberOfBits -= 8;

		return returnValue;
	}

	//
	// Variable size
	//

	/// <summary>
	/// Write Base128 encoded variable sized unsigned integer
	/// </summary>
	/// <returns>number of bytes written</returns>
	static int WriteVariableUInt32(unsigned char* intoBuffer, int offset, unsigned int value)
	{
		int retval = 0;
		unsigned int num1 = (unsigned int)value;
		while (num1 >= 0x80)
		{
			intoBuffer[offset + retval] = (unsigned char)(num1 | 0x80);
			num1 = num1 >> 7;
			retval++;
		}
		intoBuffer[offset + retval] = (unsigned char)num1;
		return retval + 1;
	}

	/// <summary>
	/// Reads a UInt32 written using WriteUnsignedVarInt(); will increment offset!
	/// </summary>
	static unsigned int ReadVariableUInt32(unsigned char* buffer, int & offset)
	{
		int num1 = 0;
		int num2 = 0;
		while (true)
		{
			//assert(num2 != 0x23, "Bad 7-bit encoded integer");

			unsigned char num3 = buffer[offset++];
			num1 |= (num3 & 0x7f) << (num2 & 0x1f);
			num2 += 7;
			if ((num3 & 0x80) == 0)
				return (unsigned int)num1;
		}
	}

};