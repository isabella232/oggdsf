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

#include "StdAfx.h"
#include "Vorbisdecodeoutputpin.h"

VorbisDecodeOutputPin::VorbisDecodeOutputPin(VorbisDecodeFilter* inParentFilter, CCritSec* inFilterLock, vector<CMediaType*> inAcceptableMediaTypes)
	: AbstractTransformOutputPin(inParentFilter, inFilterLock,NAME("VorbisDecodeOutputPin"), L"PCM Out", 65536, 20, inAcceptableMediaTypes)
{

		
}
VorbisDecodeOutputPin::~VorbisDecodeOutputPin(void)
{
	
	
}

STDMETHODIMP VorbisDecodeOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IMediaSeeking) {
		*ppv = (IMediaSeeking*)this;
		((IUnknown*)*ppv)->AddRef();
		return NOERROR;
	}

	return CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv); 
}

HRESULT VorbisDecodeOutputPin::CreateAndFillFormatBuffer(CMediaType* outMediaType, int inPosition)
{
	if (inPosition == 0) {
		WAVEFORMATEX* locWaveFormat = (WAVEFORMATEX*)outMediaType->AllocFormatBuffer(sizeof(WAVEFORMATEX));
		//TODO::: Check for null ?

		locWaveFormat->wFormatTag = WAVE_FORMAT_PCM;
		locWaveFormat->nChannels = ((VorbisDecodeFilter*)m_pFilter)->mVorbisFormatInfo->numChannels;
		locWaveFormat->nSamplesPerSec =  ((VorbisDecodeFilter*)m_pFilter)->mVorbisFormatInfo->samplesPerSec;
		locWaveFormat->wBitsPerSample = 16;
		locWaveFormat->nBlockAlign = (locWaveFormat->nChannels) * (locWaveFormat->wBitsPerSample >> 3);
		locWaveFormat->nAvgBytesPerSec = ((locWaveFormat->nChannels) * (locWaveFormat->wBitsPerSample >> 3)) * locWaveFormat->nSamplesPerSec;
		locWaveFormat->cbSize = 0;
		return S_OK;
	} else {
        return S_FALSE;
	}
}


//Old imp
//****************************************************
//#include "StdAfx.h"
//#include "Vorbisdecodeoutputpin.h"
//
//VorbisDecodeOutputPin::VorbisDecodeOutputPin(VorbisDecodeFilter* inParentFilter, CCritSec* inFilterLock)
//	: AbstractAudioDecodeOutputPin(inParentFilter, inFilterLock,NAME("VorbisDecodeOutputPin"), L"PCM Out")
//{
//
//		
//}
//VorbisDecodeOutputPin::~VorbisDecodeOutputPin(void)
//{
//	
//	
//}
//
//STDMETHODIMP VorbisDecodeOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
//{
//	if (riid == IID_IMediaSeeking) {
//		*ppv = (IMediaSeeking*)this;
//		((IUnknown*)*ppv)->AddRef();
//		return NOERROR;
//	}
//
//	return CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv); 
//}
//
//bool VorbisDecodeOutputPin::FillWaveFormatExBuffer(WAVEFORMATEX* inFormatBuffer) {
//	inFormatBuffer->wFormatTag = WAVE_FORMAT_PCM;
//	inFormatBuffer->nChannels = ((VorbisDecodeFilter*)m_pFilter)->mVorbisFormatInfo->numChannels;
//	inFormatBuffer->nSamplesPerSec =  ((VorbisDecodeFilter*)m_pFilter)->mVorbisFormatInfo->samplesPerSec;
//	inFormatBuffer->wBitsPerSample = 16;
//	inFormatBuffer->nBlockAlign = (inFormatBuffer->nChannels) * (inFormatBuffer->wBitsPerSample >> 3);
//	inFormatBuffer->nAvgBytesPerSec = ((inFormatBuffer->nChannels) * (inFormatBuffer->wBitsPerSample >> 3)) * inFormatBuffer->nSamplesPerSec;
//	inFormatBuffer->cbSize = 0;
//	return true;
//}