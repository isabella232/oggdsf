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
//#ifdef DSFABSTRACOGGAUDIODECODER_EXPORTS
//#pragma message("----> Exporting from Abstract Library...")
//#define ABS_AUDIO_DEC_API __declspec(dllexport)
//#else
//#pragma message("<---- Importing from Abstract Library...")
//#define ABS_AUDIO_DEC_API __declspec(dllimport)
//#endif

struct sFLACFormatBlock {
	unsigned short numChannels;
	unsigned long numBitsPerSample;
	unsigned long samplesPerSec;
};

#include "AbstractTransformFilter.h"
#include "AbstractTransformInputPin.h"
#include "AbstractTransformOutputPin.h"
#include "FLACDecodeFilter.h"
#include "FLACDecodeInputPin.h"
#include "FLACDecodeOutputPin.h"


// {3913F0AB-E7ED-41c4-979B-1D1FDD983C07}
DEFINE_GUID(MEDIASUBTYPE_FLAC, 
0x3913f0ab, 0xe7ed, 0x41c4, 0x97, 0x9b, 0x1d, 0x1f, 0xdd, 0x98, 0x3c, 0x7);

// {3376086C-D6F9-4ce4-8B89-33CD570106B5}
DEFINE_GUID(CLSID_FLACDecodeFilter, 
0x3376086c, 0xd6f9, 0x4ce4, 0x8b, 0x89, 0x33, 0xcd, 0x57, 0x1, 0x6, 0xb5);

// {1CDC48AC-4C24-4b8b-982B-7007A29D83C4}
DEFINE_GUID(FORMAT_FLAC, 
0x1cdc48ac, 0x4c24, 0x4b8b, 0x98, 0x2b, 0x70, 0x7, 0xa2, 0x9d, 0x83, 0xc4);


const REGPINTYPES FLACDecodeOutputTypes = {
    &MEDIATYPE_Audio,
	&MEDIASUBTYPE_PCM
};

const REGPINTYPES FLACDecodeInputTypes = {
	&MEDIATYPE_Audio,
	&MEDIASUBTYPE_FLAC
};
const REGFILTERPINS FLACDecodePinReg[] = {
	{
    L"FLAC Input",						//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	FALSE,								//Not an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//upport two media type
	&FLACDecodeInputTypes				//Pointer to media type (Audio/Vorbis or Audio/Speex)
	} ,

	{
	L"PCM Output",						//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	TRUE,								//Is an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//Only support one media type
	&FLACDecodeOutputTypes					//Pointer to media type (Audio/PCM)

	}
};



const REGFILTER2 FLACDecodeFilterReg = {
		1,
		MERIT_NORMAL,
		2,
        FLACDecodePinReg
		
};
