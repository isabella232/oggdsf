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
#include <streams.h>
#include <pullpin.h>
#include <initguid.h>


#ifdef DSFOGGMUX_EXPORTS
#pragma message("----> Exporting from Ogg Mux...")
#define OGG_MUX_API __declspec(dllexport)
#else
#pragma message("<---- Importing from Ogg Mux...")
#define OGG_MUX_API __declspec(dllimport)
#endif


#ifndef OGGMUX_DLL
    #define LIBOOOGG_API
#else
    #ifdef LIBOOOGG_EXPORTS
        #define LIBOOOGG_API __declspec(dllexport)
    #else
        #define LIBOOOGG_API __declspec(dllimport)
    #endif
#endif


// {90D6513C-A665-4b16-ACA7-B3D1D4EFE58D}
DEFINE_GUID(IID_IOggMuxProgress, 
0x90d6513c, 0xa665, 0x4b16, 0xac, 0xa7, 0xb3, 0xd1, 0xd4, 0xef, 0xe5, 0x8d);



// {30393ca2-c404-4744-a21e-90975700ea8f}
DEFINE_GUID(CLSID_PropsOggMux,
0x30393ca2, 0xc404, 0x4744, 0xa2, 0x1e, 0x90, 0x97, 0x57, 0x00, 0xea, 0x8f);

// {3a2cf997-0aeb-4d3f-9846-b5db2ca4c80b}
DEFINE_GUID(IID_IOggMuxSettings, 
0x3a2cf997, 0x0aeb, 0x4d3f, 0x98, 0x46, 0xb5, 0xdb, 0x2c, 0xa4, 0xc8, 0x0b);

//New section
// {31CA0186-1FF0-4181-AA38-3CA4040BD260}
DEFINE_GUID(CLSID_OggDemuxSourceFilter, 
0x31ca0186, 0x1ff0, 0x4181, 0xaa, 0x38, 0x3c, 0xa4, 0x4, 0xb, 0xd2, 0x60);

// {1F3EFFE4-0E70-47c7-9C48-05EB99E20011}
DEFINE_GUID(CLSID_OggMuxFilter, 
0x1f3effe4, 0xe70, 0x47c7, 0x9c, 0x48, 0x5, 0xeb, 0x99, 0xe2, 0x0, 0x11);

// {3913F0AB-E7ED-41c4-979B-1D1FDD983C07}
DEFINE_GUID(MEDIASUBTYPE_FLAC, 
0x3913f0ab, 0xe7ed, 0x41c4, 0x97, 0x9b, 0x1d, 0x1f, 0xdd, 0x98, 0x3c, 0x7);

// {2C409DB0-95BF-47ba-B0F5-587256F1EDCF}
DEFINE_GUID(MEDIASUBTYPE_OggFLAC_1_0, 
0x2c409db0, 0x95bf, 0x47ba, 0xb0, 0xf5, 0x58, 0x72, 0x56, 0xf1, 0xed, 0xcf);


// {8A0566AC-42B3-4ad9-ACA3-93B906DDF98A}
DEFINE_GUID(MEDIASUBTYPE_Vorbis, 
0x8a0566ac, 0x42b3, 0x4ad9, 0xac, 0xa3, 0x93, 0xb9, 0x6, 0xdd, 0xf9, 0x8a);

// {25A9729D-12F6-420e-BD53-1D631DC217DF}
DEFINE_GUID(MEDIASUBTYPE_Speex, 
0x25a9729d, 0x12f6, 0x420e, 0xbd, 0x53, 0x1d, 0x63, 0x1d, 0xc2, 0x17, 0xdf);

// {44E04F43-58B3-4de1-9BAA-8901F852DAE4}
DEFINE_GUID(FORMAT_Vorbis, 
0x44e04f43, 0x58b3, 0x4de1, 0x9b, 0xaa, 0x89, 0x1, 0xf8, 0x52, 0xda, 0xe4);

// {78701A27-EFB5-4157-9553-38A7854E3E81}
DEFINE_GUID(FORMAT_Speex, 
0x78701a27, 0xefb5, 0x4157, 0x95, 0x53, 0x38, 0xa7, 0x85, 0x4e, 0x3e, 0x81);

// {1CDC48AC-4C24-4b8b-982B-7007A29D83C4}
DEFINE_GUID(FORMAT_FLAC, 
0x1cdc48ac, 0x4c24, 0x4b8b, 0x98, 0x2b, 0x70, 0x7, 0xa2, 0x9d, 0x83, 0xc4);


// {05187161-5C36-4324-A734-22BF37509F2D}
DEFINE_GUID(CLSID_TheoraDecodeFilter, 
0x5187161, 0x5c36, 0x4324, 0xa7, 0x34, 0x22, 0xbf, 0x37, 0x50, 0x9f, 0x2d);

// {D124B2B1-8968-4ae8-B288-FE16EA34B0CE}
DEFINE_GUID(MEDIASUBTYPE_Theora, 
0xd124b2b1, 0x8968, 0x4ae8, 0xb2, 0x88, 0xfe, 0x16, 0xea, 0x34, 0xb0, 0xce);

// {A99F116C-DFFA-412c-95DE-725F99874826}
DEFINE_GUID(FORMAT_Theora, 
0xa99f116c, 0xdffa, 0x412c, 0x95, 0xde, 0x72, 0x5f, 0x99, 0x87, 0x48, 0x26);

// {BBCD12AC-0000-0010-8000-00aa00389b71}
DEFINE_GUID(MEDIASUBTYPE_Schroedinger, 
0xBBCD12AC, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

// {BBCD12AC-c356-11ce-bf01-00aa0055595a}
DEFINE_GUID(FORMAT_Schroedinger,
0xBBCD12AC, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a);

//This structure defines the type of input we accept on the input pin... Stream/Annodex

// {53696C76-6961-40b2-B136-436F6E726164}
DEFINE_GUID(FORMAT_CMML, 
0x53696c76, 0x6961, 0x40b2, 0xb1, 0x36, 0x43, 0x6f, 0x6e, 0x72, 0x61, 0x64);


// {5A656E74-6172-6F26-B79C-D6416E647282}
DEFINE_GUID(MEDIASUBTYPE_CMML, 
0x5a656e74, 0x6172, 0x6f26, 0xb7, 0x9c, 0xd6, 0x41, 0x6e, 0x64, 0x72, 0x82);
//Structure defining the registration details of the filter

// {37535B3C-F068-4f93-9763-E7208277D71F}
DEFINE_GUID(MEDIASUBTYPE_RawOggAudio, 
0x37535b3c, 0xf068, 0x4f93, 0x97, 0x63, 0xe7, 0x20, 0x82, 0x77, 0xd7, 0x1f);

// {232D3C8F-16BF-404b-99AE-296F3DBB77EE}
DEFINE_GUID(FORMAT_RawOggAudio, 
0x232d3c8f, 0x16bf, 0x404b, 0x99, 0xae, 0x29, 0x6f, 0x3d, 0xbb, 0x77, 0xee);
const REGPINTYPES OggMuxInputTypes[] = {
	{	
		&MEDIATYPE_Audio,
		&MEDIASUBTYPE_Speex
	},
	{
		&MEDIATYPE_Audio,
		&MEDIASUBTYPE_Vorbis
	},
	{
		&MEDIATYPE_Audio,
		&MEDIASUBTYPE_OggFLAC_1_0
	},
	{
		&MEDIATYPE_Video,
		&MEDIASUBTYPE_Theora
	},
	{
		&MEDIATYPE_Audio,
		&MEDIASUBTYPE_FLAC
	},
	{
		&MEDIATYPE_Audio,
		&MEDIASUBTYPE_RawOggAudio
	},
	{
		&MEDIATYPE_Text,
		&MEDIASUBTYPE_CMML
	}

};
const REGFILTERPINS OggMuxPinReg = {
	
    L"Ogg Packet Input",				//Name (obsoleted)
	TRUE,								//Renders from this pin ?? Not sure about this.
	FALSE,								//Not an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	7,									//upport two media type
	OggMuxInputTypes					//Pointer to media type (Audio/Vorbis or Audio/Speex)
};

const REGFILTER2 OggMuxFilterReg = {
		1,
		MERIT_DO_NOT_USE,
		1,
        &OggMuxPinReg
		
};



struct sVorbisFormatBlock {
	unsigned long vorbisVersion;
	unsigned long samplesPerSec;
	unsigned long minBitsPerSec;
	unsigned long avgBitsPerSec;
	unsigned long maxBitsPerSec;
	unsigned char numChannels;
};

struct sSpeexFormatBlock {
	unsigned long speexVersion;
	unsigned long samplesPerSec;
	unsigned long minBitsPerSec;
	unsigned long avgBitsPerSec;
	unsigned long maxBitsPerSec;
	unsigned long numChannels;

};

struct sFLACFormatBlock {
	unsigned short numChannels;
	unsigned long numBitsPerSample;
	unsigned long samplesPerSec;

};

struct sTheoraFormatBlock {
	unsigned long theoraVersion;
	unsigned long outerFrameWidth;
	unsigned long outerFrameHeight;
	unsigned long pictureWidth;
	unsigned long pictureHeight;
	unsigned long frameRateNumerator;
	unsigned long frameRateDenominator;
	unsigned long aspectNumerator;
	unsigned long aspectDenominator;
	unsigned long maxKeyframeInterval;
	unsigned long targetBitrate;
	unsigned char targetQuality;
	unsigned char xOffset;
	unsigned char yOffset;
	unsigned char colourSpace;
    unsigned char pixelFormat;
};
struct sOggRawAudioFormatBlock {
	unsigned long samplesPerSec;
	unsigned long numHeaders;
	unsigned long numChannels;
	unsigned long bitsPerSample;
	unsigned long maxFramesPerPacket;


};
struct sCMMLFormatBlock {
	__int64 granuleNumerator;
	__int64 granuleDenominator;
	unsigned short granuleSplitBits;
};