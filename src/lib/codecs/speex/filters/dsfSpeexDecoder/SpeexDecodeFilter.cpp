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

//Include Files
#include "StdAfx.h"
#include "SpeexDecodeFilter.h"


//COM Factory Template
CFactoryTemplate g_Templates[] = 
{
    { 
		L"Speex Decode Filter",					// Name
	    &CLSID_SpeexDecodeFilter,				// CLSID
	    SpeexDecodeFilter::CreateInstance,		// Method to create an instance of Speex Decoder
        NULL,									// Initialization function
        NULL									// Set-up information (for filters)
    }

};

// Generic way of determining the number of items in the template
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]); 

SpeexDecodeFilter::SpeexDecodeFilter()
	:	AbstractTransformFilter(NAME("Speex Audio Decoder"), CLSID_SpeexDecodeFilter)
	,	mSpeexFormatInfo(NULL)
{

	bool locWasConstructed = ConstructPins();
}

bool SpeexDecodeFilter::ConstructPins() 
{

	//Vector to hold our set of media types we want to accept.
	vector<CMediaType*> locAcceptableTypes;

	//Setup the media types for the output pin.
	CMediaType* locAcceptMediaType = new CMediaType(&MEDIATYPE_Audio);		//Deleted in pin destructor
	locAcceptMediaType->subtype = MEDIASUBTYPE_PCM;
	locAcceptMediaType->formattype = FORMAT_WaveFormatEx;
	
	locAcceptableTypes.push_back(locAcceptMediaType);

	//Output pin must be done first because it's passed to the input pin.
	mOutputPin = new SpeexDecodeOutputPin(this, m_pLock, locAcceptableTypes);			//Deleted in base class destructor

	//Clear out the vector, now we've already passed it to the output pin.
	locAcceptableTypes.clear();

	//Setup the media Types for the input pin.
	locAcceptMediaType = NULL;
	locAcceptMediaType = new CMediaType(&MEDIATYPE_Audio);			//Deleted by pin

	locAcceptMediaType->subtype = MEDIASUBTYPE_Speex;
	locAcceptMediaType->formattype = FORMAT_Speex;

	locAcceptableTypes.push_back(locAcceptMediaType);
	
	mInputPin = new SpeexDecodeInputPin(this, m_pLock, mOutputPin, locAcceptableTypes);	//Deleted in base class filter destructor.
	return true;

}

SpeexDecodeFilter::~SpeexDecodeFilter(void)
{
	delete mSpeexFormatInfo;
}

CUnknown* WINAPI SpeexDecodeFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr) 
{
	//This routine is the COM implementation to create a new Filter
	SpeexDecodeFilter *pNewObject = new SpeexDecodeFilter();
    if (pNewObject == NULL) {
        *pHr = E_OUTOFMEMORY;
    }
	return pNewObject;
} 

//QUERY::: Do we need these ? Aren't we all friedns here ??
//RESULT::: Keep them, set function must be kept... get could go... but keep for consistency
sSpeexFormatBlock* SpeexDecodeFilter::getSpeexFormatBlock() 
{
	return mSpeexFormatInfo;
}
void SpeexDecodeFilter::setSpeexFormat(sSpeexFormatBlock* inFormatBlock) 
{
	delete mSpeexFormatInfo;
	mSpeexFormatInfo = new sSpeexFormatBlock;
	*mSpeexFormatInfo = *inFormatBlock;
}