#pragma once

class CircularBuffer
{
public:
	CircularBuffer(unsigned long inBufferSize);
	~CircularBuffer(void);

	unsigned long read(unsigned char* outData, unsigned long inBytesToRead);
	unsigned long write(const unsigned char* inData, unsigned long inBytesToWrite);

	unsigned long numBytesAvail();
	unsigned long spaceLeft();

protected:
	unsigned long mBufferSize;
	unsigned long mReadPtr;
	unsigned long mWritePtr;

	unsigned char* mBuffer;
};
