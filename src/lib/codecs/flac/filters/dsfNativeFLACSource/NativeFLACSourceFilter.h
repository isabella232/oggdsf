//===========================================================================
//Copyright (C) 2003, 2004 Zentaro Kavanagh
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
//Local Includes
#include "dsfNativeFLACSource.h"
#include "NativeFLACSourcePin.h"

//Library Includes
#include "FLAC++/decoder.h"
#include <libilliCore/StringHelper.h>
#include <libilliCore/iBE_Math.h>
#include "FLAC/seekable_stream_decoder.h"
#include "FLAC/stream_decoder.h"
using namespace FLAC::Decoder;

//STL Includes
#include <string>
using namespace std;

//Forward Declarations
class NativeFLACSourcePin;

class NativeFLACSourceFilter
	//Base Classes
	:	public CBaseFilter
	,	public IFileSourceFilter
	,	public IAMFilterMiscFlags
	,	public IMediaSeeking
	,	public CAMThread
	,	protected FLAC::Decoder::Stream
{
public:
	//Friends
	friend class NativeFLACSourcePin;

	//Constants
	enum eThreadCommands {
		THREAD_EXIT = 0,
		THREAD_PAUSE = 1,
		THREAD_RUN = 2
	};

	//COM Stuff
	DECLARE_IUNKNOWN
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);

	//Constructors
	NativeFLACSourceFilter(void);
	virtual ~NativeFLACSourceFilter(void);

	//IBaseFilter Pure Virtuals
	virtual int GetPinCount();
	virtual CBasePin* GetPin(int inPinNo);

	//IAMFilterMiscFlags Interface
	ULONG STDMETHODCALLTYPE GetMiscFlags(void);

	//IFileSource Interface
	virtual STDMETHODIMP GetCurFile(LPOLESTR* outFileName, AM_MEDIA_TYPE* outMediaType);
	virtual STDMETHODIMP Load(LPCOLESTR inFileName, const AM_MEDIA_TYPE* inMediaType);

	//Streaming MEthods
	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Pause(void);
	STDMETHODIMP Stop(void);

	//CAMThread
	virtual DWORD ThreadProc(void);

	//FLAC Virtuals
	virtual ::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte outBuffer[], unsigned int* outNumBytes);
	virtual ::FLAC__StreamDecoderSeekStatus seek_callback(FLAC__uint64 inSeekPos);
	virtual ::FLAC__StreamDecoderTellStatus tell_callback(FLAC__uint64* outTellPos);
	virtual ::FLAC__StreamDecoderLengthStatus length_callback(FLAC__uint64* outLength);
	virtual ::FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame* outFrame,const FLAC__int32 *const outBuffer[]);
	virtual void metadata_callback(const FLAC__StreamMetadata* inMetaData);
	virtual void error_callback(FLAC__StreamDecoderErrorStatus inStatus);
	virtual bool eof_callback(void);

	//IMediaSeeking Interface
	virtual STDMETHODIMP GetCapabilities(DWORD *pCapabilities);
	virtual STDMETHODIMP CheckCapabilities(DWORD *pCapabilities);
	virtual STDMETHODIMP IsFormatSupported(const GUID *pFormat);
	virtual STDMETHODIMP QueryPreferredFormat(GUID *pFormat);
	virtual STDMETHODIMP SetTimeFormat(const GUID *pFormat);
	virtual STDMETHODIMP GetTimeFormat( GUID *pFormat);
	virtual STDMETHODIMP GetDuration(LONGLONG *pDuration);
	virtual STDMETHODIMP GetStopPosition(LONGLONG *pStop);
	virtual STDMETHODIMP GetCurrentPosition(LONGLONG *pCurrent);
	virtual STDMETHODIMP ConvertTimeFormat(LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat);
	virtual STDMETHODIMP SetPositions(LONGLONG *pCurrent,DWORD dwCurrentFlags,LONGLONG *pStop,DWORD dwStopFlags);
	virtual STDMETHODIMP GetPositions(LONGLONG *pCurrent, LONGLONG *pStop);
	virtual STDMETHODIMP GetAvailable(LONGLONG *pEarliest, LONGLONG *pLatest);
	virtual STDMETHODIMP SetRate(double dRate);
	virtual STDMETHODIMP GetRate(double *dRate);
	virtual STDMETHODIMP GetPreroll(LONGLONG *pllPreroll);
	virtual STDMETHODIMP IsUsingTimeFormat(const GUID *pFormat);

protected:
	//Helper Methods
	HRESULT DataProcessLoop();

	//Pin Class
	NativeFLACSourcePin* mFLACSourcePin;

	//Source File Members
	wstring mFileName;
	fstream mInputFile;
	unsigned long mFileSize;

	//State Variables
	bool mBegun;
	bool mJustSeeked;
	__int64 mSeekRequest;
	bool mWasEOF;
	unsigned long mUpto;

	//Stream info data.
	unsigned long mNumChannels;
	unsigned long mFrameSize;
	unsigned long mSampleRate;
	unsigned long mBitsPerSample;
    unsigned long mSignificantBitsPerSample;
	__int64 mTotalNumSamples;
};
