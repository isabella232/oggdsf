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
#include "abstractvideoencoderdllstuff.h"
#include "AbstractVideoEncodeFilter.h"
#include "BasicSeekable.h"
class AbstractVideoEncodeFilter;
class ABS_VIDEO_ENC_API AbstractVideoEncodeOutputPin
	:	public CBaseOutputPin
	,	public BasicSeekable
{
public:
	AbstractVideoEncodeOutputPin(AbstractVideoEncodeFilter* inParentFilter, CCritSec* inFilterLock, CHAR* inObjectName, LPCWSTR inPinDisplayName, CMediaType* inOutputMediaType);
	virtual ~AbstractVideoEncodeOutputPin(void);

	DECLARE_IUNKNOWN
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	virtual HRESULT DecideBufferSize(IMemAllocator* inAllocator, ALLOCATOR_PROPERTIES *inReqAllocProps);
	virtual HRESULT CheckMediaType(const CMediaType *inMediaType);

	//PURE VIRTUALS
	virtual bool FillFormatBuffer(BYTE* inFormatBuffer) = 0;
	virtual unsigned long FormatBufferSize() = 0;
	
	virtual HRESULT GetMediaType(int inPosition, CMediaType *outMediaType);


	//Virtuals for data queue
	virtual HRESULT CompleteConnect (IPin *inReceivePin);
	virtual HRESULT DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
	virtual HRESULT DeliverEndOfStream(void);
	virtual HRESULT DeliverEndFlush(void);
	virtual HRESULT DeliverBeginFlush(void);

	COutputQueue* mDataQueue;
protected:
	HRESULT mHR;
	AbstractVideoEncodeFilter* mParentFilter;
	void FillMediaType(CMediaType* inMediaType);

	CMediaType* mOutputMediaType;

	static const int BUFF_SIZE = 65536;	static const int BUFF_COUNT = 5;
};
