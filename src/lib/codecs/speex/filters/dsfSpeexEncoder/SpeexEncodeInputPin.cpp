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
#include "Speexencodeinputpin.h"

SpeexEncodeInputPin::SpeexEncodeInputPin(AbstractAudioEncodeFilter* inParentFilter, CCritSec* inFilterLock, AbstractAudioEncodeOutputPin* inOutputPin)
	:	AbstractAudioEncodeInputPin(inParentFilter, inFilterLock, inOutputPin, NAME("SpeexEncodeInputPin"), L"PCM In")
	,	mFishSound(NULL)
{
	//debugLog.open("C:\\temp\\speexenc.log", ios_base::out);
}

SpeexEncodeInputPin::~SpeexEncodeInputPin(void)
{
	//debugLog.close();
	DestroyCodec();
}


//PURE VIRTUALS
long SpeexEncodeInputPin::encodeData(unsigned char* inBuf, long inNumBytes) {


	//debugLog << "encodeData receives : "<<inNumBytes<<" bytes"<<endl;
	
	float* locFloatBuf = new float[inNumBytes/2 + (inNumBytes % 2)];
	short locTempShort = 0;
	float locTempFloat = 0;

	//__int64 locGranPos = 0;
	//Removed hack for gran pos
	//fish_sound_command(mFishSound, 8, &locGranPos, sizeof(__int64));
	//
	//locGranPos = fish_sound_get_frameno(mFishSound);
	//mUptoFrame = locGranPos;
	//__int64 locTemp = ((FishSoundSpeexInfo*)mFishSound->codec_data)->vd.pcm_returned;
	for (int i = 0; i < inNumBytes; i += 2) {
		locTempShort = *((short*)(inBuf + i));
		locTempFloat = (float)locTempShort;
		locTempFloat /= 32767.0;
		locFloatBuf[i/2] = locTempFloat;;
	}
	//debugLog<<"Calling encode"<<endl;
	//FIX::: The 2 is the size of a sample ie 16 bits
	long locErr = fish_sound_encode(mFishSound, (float**)locFloatBuf, inNumBytes/(mFishInfo.channels*2));
	delete locFloatBuf;
	//FIX::: Do something here ?
	if (locErr < 0) {
		//debugLog<<"Fishsound reports error"<<endl;
	} else {
	
	}
	return locErr;
}
bool SpeexEncodeInputPin::ConstructCodec() {
	mFishInfo.channels = mWaveFormat->nChannels;
	mFishInfo.format = FISH_SOUND_SPEEX;
	mFishInfo.samplerate = mWaveFormat->nSamplesPerSec;


	
	mFishSound = fish_sound_new (FISH_SOUND_ENCODE, &mFishInfo);

	int i = 1;
	//FIX::: Use new API for interleave setting
	fish_sound_command(mFishSound, FISH_SOUND_SET_INTERLEAVE, &i, sizeof(int));

	fish_sound_set_encoded_callback (mFishSound, SpeexEncodeInputPin::SpeexEncoded, this);
	//FIX::: Proper return value
	return true;

}
void SpeexEncodeInputPin::DestroyCodec() {
	fish_sound_delete(mFishSound);
	mFishSound = NULL;
}


//Encoded callback
int SpeexEncodeInputPin::SpeexEncoded (FishSound* inFishSound, unsigned char* inPacketData, long inNumBytes, void* inThisPointer) 
{

	//For convenience we do all these cast once and for all here.
	SpeexEncodeInputPin* locThis = reinterpret_cast<SpeexEncodeInputPin*> (inThisPointer);
	SpeexEncodeFilter* locFilter = reinterpret_cast<SpeexEncodeFilter*>(locThis->m_pFilter);
	//locThis->debugLog << "SpeexEncoded called with "<<inNumBytes<< " byte of data"<<endl;

	//Time stamps are granule pos not directshow times
	LONGLONG locFrameStart = locThis->mUptoFrame;
	LONGLONG locFrameEnd	= locThis->mUptoFrame
							= fish_sound_get_frameno(locThis->mFishSound);

	
	//locThis->debugLog << "Stamping packet "<<locFrameStart<< " to "<<locFrameEnd<<endl;
	//Get a pointer to a new sample stamped with our time
	IMediaSample* locSample;
	HRESULT locHR = locThis->mOutputPin->GetDeliveryBuffer(&locSample, &locFrameStart, &locFrameEnd, NULL);

	if (FAILED(locHR)) {
		//We get here when the application goes into stop mode usually.
		//locThis->debugLog<<"Getting buffer failed"<<endl;
		return locHR;
	}	
	
	BYTE* locBuffer = NULL;

	
	//Make our pointers set to point to the samples buffer
	locSample->GetPointer(&locBuffer);

	

	if (locSample->GetSize() >= inNumBytes) {

		memcpy((void*)locBuffer, (const void*)inPacketData, inNumBytes);
		
		//Set the sample parameters.
		locThis->SetSampleParams(locSample, inNumBytes, &locFrameStart, &locFrameEnd);

		{
			CAutoLock locLock(locThis->m_pLock);

			//Add a reference so it isn't deleted en route.
			//locSample->AddRef();
			HRESULT locHR = locThis->mOutputPin->mDataQueue->Receive(locSample);						//->DownstreamFilter()->Receive(locSample);
			if (locHR != S_OK) {
				//locThis->debugLog<<"Sample rejected"<<endl;
			} else {
				//locThis->debugLog<<"Sample Delivered"<<endl;
			}
		}

		return 0;
	} else {
		throw 0;
	}
}


HRESULT SpeexEncodeInputPin::SetMediaType(const CMediaType* inMediaType) {
	AbstractAudioEncodeInputPin::SetMediaType(inMediaType);

	ConstructCodec();

	return S_OK;
	
}