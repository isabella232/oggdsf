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

struct sSpeexFormatBlock {
	unsigned long speexVersion;
	unsigned long samplesPerSec;
	unsigned long minBitsPerSec;
	unsigned long avgBitsPerSec;
	unsigned long maxBitsPerSec;
	unsigned long numChannels;

};
#include "AbstractTransformFilter.h"
#include "AbstractTransformInputPin.h"
#include "AbstractTransformOutputPin.h"
#include "SpeexEncodeInputPin.h"
#include "SpeexEncodeOutputPin.h"
#include "SpeexEncodeFilter.h"

#ifdef LIBOOOGG_EXPORTS
#define LIBOOOGG_API __declspec(dllexport)
#else
#define LIBOOOGG_API __declspec(dllimport)
#endif




// {7036C2FE-A209-464c-97AB-95B9260EDBF7}
DEFINE_GUID(CLSID_SpeexEncodeFilter, 
0x7036c2fe, 0xa209, 0x464c, 0x97, 0xab, 0x95, 0xb9, 0x26, 0xe, 0xdb, 0xf7);

// {25A9729D-12F6-420e-BD53-1D631DC217DF}
DEFINE_GUID(MEDIASUBTYPE_Speex, 
0x25a9729d, 0x12f6, 0x420e, 0xbd, 0x53, 0x1d, 0x63, 0x1d, 0xc2, 0x17, 0xdf);

// {78701A27-EFB5-4157-9553-38A7854E3E81}
DEFINE_GUID(FORMAT_Speex, 
0x78701a27, 0xefb5, 0x4157, 0x95, 0x53, 0x38, 0xa7, 0x85, 0x4e, 0x3e, 0x81);
//This structure defines the type of input we accept on the input pin... Stream/Annodex

const REGPINTYPES SpeexEncodeInputTypes = {
    &MEDIATYPE_Audio,
	&MEDIASUBTYPE_PCM
};

const REGPINTYPES SpeexEncodeOutputTypes = {
	&MEDIATYPE_Audio,
	&MEDIASUBTYPE_Speex
};

const REGFILTERPINS SpeexEncodePinReg[] = {
	{
    L"PCM Input",						//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	FALSE,								//Not an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//upport two media type
	&SpeexEncodeInputTypes				//Pointer to media type (Audio/Speex or Audio/Speex)
	} ,

	{
	L"Speex Output",						//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	TRUE,								//Is an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//Only support one media type
	&SpeexEncodeOutputTypes					//Pointer to media type (Audio/PCM)

	}
};



const REGFILTER2 SpeexEncodeFilterReg = {
		1,
		MERIT_DO_NOT_USE,
		2,
        SpeexEncodePinReg
		
};
	   

