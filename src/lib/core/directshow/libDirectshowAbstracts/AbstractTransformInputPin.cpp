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
#include "AbstractTransformInputPin.h"


AbstractTransformInputPin::AbstractTransformInputPin (AbstractTransformFilter* inParentFilter, CCritSec* inFilterLock, AbstractTransformOutputPin* inOutputPin, CHAR* inObjectName, LPCWSTR inPinDisplayName, vector<CMediaType*> inAcceptMediaTypes)
	:	CBaseInputPin (inObjectName, inParentFilter, inFilterLock, &mHR, inPinDisplayName)
	,	mOutputPin (inOutputPin)
	,	mParentFilter (inParentFilter)
	
	,	mBegun (false)


{
	
	//ConstructCodec();
	//debugLog.open("g:\\logs\\aad.log", ios_base::out);

	//TODO::: Put in init list.
	mAcceptableMediaTypes = inAcceptMediaTypes;
	mStreamLock = new CCritSec;			//Deleted in destructor.

}

STDMETHODIMP AbstractTransformInputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	//TODO::: This really shouldn't be exposed on an input pin
	if (riid == IID_IMediaSeeking) {
		*ppv = (IMediaSeeking*)this;
		((IUnknown*)*ppv)->AddRef();
		return NOERROR;
	}

	return CBaseInputPin::NonDelegatingQueryInterface(riid, ppv); 
}

HRESULT AbstractTransformInputPin::BreakConnect() 
{
	CAutoLock locLock(m_pLock);
	//Release the seeking delegate
	ReleaseDelegate();
	return CBaseInputPin::BreakConnect();
}
HRESULT AbstractTransformInputPin::CompleteConnect (IPin *inReceivePin) 
{
	CAutoLock locLock(m_pLock);
	
	IMediaSeeking* locSeeker = NULL;
	inReceivePin->QueryInterface(IID_IMediaSeeking, (void**)&locSeeker);
	SetDelegate(locSeeker);
	return CBaseInputPin::CompleteConnect(inReceivePin);
}
AbstractTransformInputPin::~AbstractTransformInputPin(void)
{
	//DestroyCodec();
	//debugLog.close();
	delete mStreamLock;

}


//void AbstractTransformInputPin::ResetFrameCount() 
//{
//	mUptoFrame = 0;
//	
//}
//void AbstractTransformInputPin::ResetTimeBases() 
//{
//	mLastSeenStartGranPos = 0;
//}
bool AbstractTransformInputPin::SetSampleParams(IMediaSample* outMediaSample, unsigned long inDataSize, REFERENCE_TIME* inStartTime, REFERENCE_TIME* inEndTime) 
{
	outMediaSample->SetTime(inStartTime, inEndTime);
	outMediaSample->SetMediaTime(NULL, NULL);
	outMediaSample->SetActualDataLength(inDataSize);
	outMediaSample->SetPreroll(FALSE);
	outMediaSample->SetDiscontinuity(FALSE);
	outMediaSample->SetSyncPoint(TRUE);
	return true;
}


STDMETHODIMP AbstractTransformInputPin::Receive(IMediaSample* inSample) 
{
	CAutoLock locLock(mStreamLock);

	HRESULT locHR = CheckStreaming();

	if (locHR == S_OK) {
		BYTE* locBuff = NULL;
		locHR = inSample->GetPointer(&locBuff);

		if (locHR != S_OK) {
			//TODO::: Do a debug dump or something here with specific error info.
			return locHR;
		} else {


			////Is any of this needed ???
			////New start time hacks
			//REFERENCE_TIME locStart = 0;
			//REFERENCE_TIME locEnd = 0;

			////More work arounds for that stupid granule pos scheme in theora!
			//REFERENCE_TIME locTimeBase = 0;
			//REFERENCE_TIME locDummy = 0;
			//inSample->GetMediaTime(&locTimeBase, &locDummy);
			//mSeekTimeBase = locTimeBase;
			////

			//inSample->GetTime(&locStart, &locEnd);
			////Error chacks needed here
			////debugLog<<"Receive : Start    = "<<locStart<<endl;
			////debugLog<<"Receive : End      = "<<locEnd<<endl;
			////debugLog<<"Receive : Timebase = "<<locTimeBase<<endl;
			//
			////QUERY::: Why are we doing this ???
			////			Wouldn't it be better to only resset the frame count when we seek (ie in NewSegment)
			////			then we just use m_tStart as the time base until another seek.
			//if ((mLastSeenStartGranPos != locStart) && (locStart != -1)) {
			//	//debugLog<<"Receive : RESETTING FRAME COUNT !!"<<endl;
			//	ResetFrameCount();
			//}
			////debugLog<<endl;
			//mLastSeenStartGranPos = locStart;
			////End of additions
			
			//TODO::: Why using this convention... why not use HRESULT ???
			HRESULT locResult = TransformData(locBuff, inSample->GetActualDataLength());
			if (locResult == S_OK) {

				//aadDebug<<"Receive Decode : OK"<<endl;
				return S_OK;
			} else {
				//aadDebug<<"Receive Decode : *** FAILED *** "<<locResult<<endl;
				return S_FALSE;
			}
		}
	} else {
		//debugLog<<"NOT STREAMING.... "<<endl;
		return locHR;
	}
	
	return S_OK;
}

HRESULT AbstractTransformInputPin::CheckMediaType(const CMediaType *inMediaType) {
	//TO DO::: Neaten this up.
	for (int i = 0; i < mAcceptableMediaTypes.size(); i++) {
		if	(		(inMediaType->majortype == mAcceptableMediaTypes[i]->majortype) 
				&&	(inMediaType->subtype == mAcceptableMediaTypes[i]->subtype) 
				&&	(inMediaType->formattype == mAcceptableMediaTypes[i]->formattype)
			)
		{
			return S_OK;
		} 
	}
	//If it matched none... return false.
	return S_FALSE;
}

STDMETHODIMP AbstractTransformInputPin::EndOfStream(void) {
	CAutoLock locLock(mStreamLock);
	
	return mParentFilter->mOutputPin->DeliverEndOfStream();
}

STDMETHODIMP AbstractTransformInputPin::BeginFlush() {
	CAutoLock locLock(m_pLock);

	CBaseInputPin::BeginFlush();
	return mParentFilter->mOutputPin->DeliverBeginFlush();
}
STDMETHODIMP AbstractTransformInputPin::EndFlush() {
	CAutoLock locLock(m_pLock);

	mParentFilter->mOutputPin->DeliverEndFlush();
	return CBaseInputPin::EndFlush();
}

STDMETHODIMP AbstractTransformInputPin::NewSegment(REFERENCE_TIME inStartTime, REFERENCE_TIME inStopTime, double inRate) {
	CAutoLock locLock(mStreamLock);
	//ResetFrameCount();

	//This is called on BasePin and not BaseInputPin because the implementation is not overriden in BaseOutputPin.
	CBasePin::NewSegment(inStartTime, inStopTime, inRate);
	return mParentFilter->mOutputPin->DeliverNewSegment(inStartTime, inStopTime, inRate);
}

HRESULT AbstractTransformInputPin::GetMediaType(int inPosition, CMediaType *outMediaType) 
{
	//TODO::: Check for NULL Pointer.
	if (inPosition < 0) {
		return E_INVALIDARG;
	} else 	if (inPosition < mAcceptableMediaTypes.size()) {
		outMediaType->SetType(&(mAcceptableMediaTypes[inPosition]->majortype));
		outMediaType->SetSubtype(&(mAcceptableMediaTypes[inPosition]->subtype));
		//Don't set the format data here... its up to the connecting output pin
		// to do this, and we will verify it in CheckMediaType
		return S_OK;
	} else {
		return VFW_S_NO_MORE_ITEMS;
	}

}