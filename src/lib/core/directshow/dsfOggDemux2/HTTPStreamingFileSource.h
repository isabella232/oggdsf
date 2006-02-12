
//===========================================================================
//Copyright (C) 2003-2006 Zentaro Kavanagh
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//- Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//- Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//
//- Neither the name of Zentaro Kavanagh nor the names of contributors 
//  may be used to endorse or promote products derived from this software 
//  without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
//CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//===========================================================================
#pragma once
#include "oggdllstuff.h"

#include "HTTPSocket.h"
#include "IFilterDataSource.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class OGG_DEMUX2_API HTTPStreamingFileSource
	:	public IFilterDataSource
	,	public CAMThread
	,	protected HTTPSocket	
{
public:
	HTTPStreamingFileSource(void);
	virtual ~HTTPStreamingFileSource(void);

	//Consts
	static const unsigned long MEMORY_BUFFER_SIZE = 1024 * 1024 * 2;	//2 megs
	static const unsigned long MEMORY_BUFFER_LOW_TIDE = 1024 * 512 * 3; //1.5 megs
	//Thread commands
	static const int THREAD_RUN = 0;
	static const int THREAD_EXIT = 1;

	//IFilterDataSource Interface
	virtual unsigned long seek(unsigned long inPos);
	virtual void close() ;
	virtual bool open(string inSourceLocation, unsigned long inStartByte = 0);
	virtual void clear();
	virtual bool isEOF();
	virtual bool isError();
	virtual unsigned long read(char* outBuffer, unsigned long inNumBytes);
	virtual string shouldRetryAt();

	
	//CAMThread pure virtuals
	DWORD ThreadProc();



protected:
	void unChunk(unsigned char* inBuff, unsigned long inNumBytes);
	unsigned short getHTTPResponseCode(string inHTTPResponse);
	bool startThread();
	void DataProcessLoop();

	//SingleMediaFileCache mFileCache;
	CircularBuffer* mMemoryBuffer;

	bool mIsChunked;
	unsigned long mChunkRemains;

	bool mIsBufferFilling;

	bool mIsFirstChunk;
	string mRetryAt;

	fstream debugLog;
	fstream fileDump;
	fstream rawDump;

	unsigned char* mInterBuff;
	unsigned long mNumLeftovers;
	static	const unsigned long RECV_BUFF_SIZE = 1024;
	static const unsigned long STREAM_START_BUFFER_SIZE = 32 * 1024;

	//Fix for seekback on headers - since we only maintain a forward buffer. The seek back to start which occurs
	//	right after the headers are processed, will generally trigger a stream reset. But an annodex server,
	//	will serve out a file with completely different serial numbers, making it impossible to map the streams
	//	using the originally gathered information.
	//
	//Now we are going to buffer up the first part of the file into yet another buffer. Also keep track
	//	of what the absolute byte position is we have read up to so far from the stream. When we receive a seek request,
	//	we check if the current read position is within this new buffer.
	//
	//If it is, then we can set a flag, and respond to read requests from this new buffer, up until the point
	//	where the stream was before the seek, then switch back to serving out live streaming data.
	unsigned long mCurrentAbsoluteReadPosition;
	bool mFirstPass;

	unsigned char* mStreamStartBuffer;
	unsigned long mStreamStartBufferLength;

	unsigned long mApparentReadPosition;
	
	//

	__int64 mContentLength;

	CCritSec* mBufferLock;
};
