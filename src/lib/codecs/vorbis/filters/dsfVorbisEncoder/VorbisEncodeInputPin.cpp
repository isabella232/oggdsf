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

#include "stdafx.h"
#include "VorbisEncodeInputPin.h"

VorbisEncodeInputPin::VorbisEncodeInputPin(     AbstractTransformFilter* inParentFilter
                                            ,   CCritSec* inFilterLock
                                            ,   AbstractTransformOutputPin* inOutputPin
                                            ,   vector<CMediaType*> inAcceptableMediaTypes)
	:	AbstractTransformInputPin(      inParentFilter
                                    ,   inFilterLock
                                    ,   inOutputPin
                                    ,   NAME("VorbisEncodeInputPin")
                                    ,   L"PCM In"
                                    ,   inAcceptableMediaTypes)
    ,   mBegun(false)
	,	mWaveFormat(NULL)
	,	mUptoFrame(0)
{
#ifdef OGGCODECS_LOGGING
	debugLog.open("G:\\logs\\vorbisenc.logs", ios_base::out);
#endif
}

VorbisEncodeInputPin::~VorbisEncodeInputPin(void)
{
	debugLog.close();
	DestroyCodec();
}


//PURE VIRTUALS
HRESULT VorbisEncodeInputPin::TransformData(unsigned char* inBuf, long inNumBytes) 
{

    HRESULT locHR = S_OK;
    vector<StampedOggPacket*> locPackets;
    if (!mBegun) {
        locPackets = mVorbisEncoder.setupCodec(mEncoderSettings);
        if (locPackets.size() != VorbisEncoder::NUM_VORBIS_HEADERS) {
            //Is this really what we want to return?
            return E_FAIL;
        }
        locHR = sendPackets(locPackets);
        deletePacketsAndEmptyVector(locPackets);
        if (locHR != S_OK) {
            return locHR;
        }
        mBegun = true;
    }

    unsigned long locNumSamplesPerChannel = bufferBytesToSampleCount(inNumBytes);
    locPackets = mVorbisEncoder.encodeVorbis((const short* const)inBuf, locNumSamplesPerChannel);

    locHR = sendPackets(locPackets);
    deletePacketsAndEmptyVector(locPackets);
    return locHR;

}

bool VorbisEncodeInputPin::ConstructCodec() 
{
    mEncoderSettings.setAudioParameters(mWaveFormat->nChannels, mWaveFormat->nSamplesPerSec);
    mUptoFrame = 0;
    return true;
}
void VorbisEncodeInputPin::DestroyCodec() 
{

}


HRESULT VorbisEncodeInputPin::SetMediaType(const CMediaType* inMediaType) 
{
	if (	(inMediaType->subtype == MEDIASUBTYPE_PCM) &&
			(inMediaType->formattype == FORMAT_WaveFormatEx)) {

                //Is this really safe?
		mWaveFormat = (WAVEFORMATEX*)inMediaType->pbFormat;
	} else {
		//Failed... should never be here !
		throw 0;
	}
	//This is here and not the constructor because we need audio params from the
	// input pin to construct properly.
	ConstructCodec();

	return CBaseInputPin::SetMediaType(inMediaType);

}

HRESULT VorbisEncodeInputPin::EndOfStream()
{
    CAutoLock locLock(mStreamLock);

    vector<StampedOggPacket*> locPackets = mVorbisEncoder.flush();

    HRESULT locHR = sendPackets(locPackets);
    deletePacketsAndEmptyVector(locPackets);
    return AbstractTransformInputPin::EndOfStream();

}

void VorbisEncodeInputPin::deletePacketsAndEmptyVector(vector<StampedOggPacket*>& inPackets)
{
    for (size_t i = 0; i < inPackets.size(); i++) {
        delete inPackets[i];
    }
    inPackets.clear();
}

unsigned long VorbisEncodeInputPin::bufferBytesToSampleCount(long inByteCount)
{
    if (mEncoderSettings.mNumChannels == 0) {
        return 0;
    }
    //TODO::: Needs a bytes per sample thingy
    const long SIZE_OF_SHORT = sizeof(short);
    return (inByteCount / mEncoderSettings.mNumChannels) / SIZE_OF_SHORT;
}

HRESULT VorbisEncodeInputPin::sendPackets(const vector<StampedOggPacket*>& inPackets)
{
  
	LONGLONG locFrameStart;
    LONGLONG locFrameEnd;
    IMediaSample* locSample = NULL;
    BYTE* locBuffer = NULL;

    for (size_t pack = 0; pack < inPackets.size(); pack++) {
        locFrameStart = mUptoFrame;
        locFrameEnd = inPackets[pack]->endTime();
        mUptoFrame = locFrameEnd;

        HRESULT locHR = mOutputPin->GetDeliveryBuffer(     &locSample
                                                        ,   &locFrameStart
                                                        ,   &locFrameEnd
                                                        ,   NULL);
        if (FAILED(locHR)) {
		    //We get here when the application goes into stop mode usually.
		    return locHR;
	    }	

        //TODO::: Should we be checking this return?
        locSample->GetPointer(&locBuffer);




	    if (locSample->GetSize() >= inPackets[pack]->packetSize()) {


		    memcpy((void*)locBuffer, (const void*)inPackets[pack]->packetData(), inPackets[pack]->packetSize());
    		
		    //Set the sample parameters.
		    SetSampleParams(locSample, inPackets[pack]->packetSize(), &locFrameStart, &locFrameEnd);

		    locHR = ((VorbisEncodeOutputPin*)mOutputPin)->mDataQueue->Receive(locSample);
            if (locHR != S_OK) {
                return locHR;
            }

	    } else {
		    throw 0;
	    }
    }

    return S_OK;
}
