#include "StdAfx.h"
#include "abstractvideoencodeoutputpin.h"

AbstractVideoEncodeOutputPin::AbstractVideoEncodeOutputPin(AbstractVideoEncodeFilter* inParentFilter, CCritSec* inFilterLock, CHAR* inObjectName, LPCWSTR inPinDisplayName, CMediaType* inOutputMediaType)
	:	CBaseOutputPin(inObjectName, inParentFilter, inFilterLock, &mHR, inPinDisplayName),
		mParentFilter(inParentFilter)

	,	mDataQueue(NULL)
{
	mOutputMediaType = inOutputMediaType;
}
AbstractVideoEncodeOutputPin::~AbstractVideoEncodeOutputPin(void)
{	
	
	delete mDataQueue;
	
}


HRESULT AbstractVideoEncodeOutputPin::DecideBufferSize(IMemAllocator* inAllocator, ALLOCATOR_PROPERTIES* inPropertyRequest) {
		//FIX::: Abstract this out properly	

	HRESULT locHR = S_OK;

	//Create the structures for setproperties to use
	ALLOCATOR_PROPERTIES locReqAlloc;
	ALLOCATOR_PROPERTIES locActualAlloc;

	const unsigned long MIN_BUFFER_SIZE = 1096;			//What should this be ????
	const unsigned long DEFAULT_BUFFER_SIZE = 32192;
	const unsigned long MIN_NUM_BUFFERS = 3;
	const unsigned long DEFAULT_NUM_BUFFERS = 5;

	
	//Validate and change what we have been requested to do.
	//Allignment of data
	if (inPropertyRequest->cbAlign <= 0) {
		locReqAlloc.cbAlign = 1;
	} else {
		locReqAlloc.cbAlign = inPropertyRequest->cbAlign;
	}

	if (inPropertyRequest->cbBuffer < MIN_BUFFER_SIZE) {
		locReqAlloc.cbBuffer = DEFAULT_BUFFER_SIZE;
	} else {
		locReqAlloc.cbBuffer = inPropertyRequest->cbBuffer;
	}

	if (inPropertyRequest->cbPrefix < 0) {
			locReqAlloc.cbPrefix = 0;
	} else {
		locReqAlloc.cbPrefix = inPropertyRequest->cbPrefix;
	}

	if (inPropertyRequest->cBuffers < MIN_NUM_BUFFERS) {
		locReqAlloc.cBuffers = DEFAULT_NUM_BUFFERS;
	} else {

		locReqAlloc.cBuffers = inPropertyRequest->cBuffers;
	}

	locHR = inAllocator->SetProperties(&locReqAlloc, &locActualAlloc);

	if (locHR != S_OK)  {
		return locHR;
	}

	
	locHR = inAllocator->Commit();

	return locHR;
}
HRESULT AbstractVideoEncodeOutputPin::CheckMediaType(const CMediaType *inMediaType) {
		if	( (inMediaType->majortype == MEDIATYPE_Video) &&
			(inMediaType->subtype == mOutputMediaType->subtype) && (inMediaType->formattype == mOutputMediaType->formattype)
		)
	{
		return S_OK;
	} else {
		return S_FALSE;
	}
	
}

HRESULT AbstractVideoEncodeOutputPin::GetMediaType(int inPosition, CMediaType *outMediaType) {

	if (inPosition < 0) {
		return E_INVALIDARG;
	}

	switch (inPosition) {
		case 0:

			outMediaType->SetType(&MEDIATYPE_Video);
			outMediaType->SetSubtype(&(mOutputMediaType->subtype));
			outMediaType->SetFormatType(&(mOutputMediaType->formattype));
			return S_OK;			
		default:
			return VFW_S_NO_MORE_ITEMS;
	}
}





HRESULT AbstractVideoEncodeOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
	//mPartialPacket = NULL;
	mDataQueue->NewSegment(tStart, tStop, dRate);

	return S_OK;
}
HRESULT AbstractVideoEncodeOutputPin::DeliverEndOfStream(void)
{
	//mPartialPacket = NULL;
	mDataQueue->EOS();
    return S_OK;
}

HRESULT AbstractVideoEncodeOutputPin::DeliverEndFlush(void)
{
	mDataQueue->EndFlush();
    return S_OK;
}

HRESULT AbstractVideoEncodeOutputPin::DeliverBeginFlush(void)
{
	//mPartialPacket = NULL;
	mDataQueue->BeginFlush();
    return S_OK;
}

HRESULT AbstractVideoEncodeOutputPin::CompleteConnect (IPin *inReceivePin)
{
	HRESULT locHR = S_OK;
	//DELETE in DEStructor
	mDataQueue = new COutputQueue (inReceivePin, &locHR, FALSE, TRUE, 1, TRUE, 10);
	if (FAILED(locHR)) {
		locHR = locHR;
	}
	
	return CBaseOutputPin::CompleteConnect(inReceivePin);
}