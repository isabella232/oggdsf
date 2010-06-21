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
#include <dvdmedia.h>

#ifndef THEORAENCODER_DLL
	#define LIBOOOGG_API
#else
	#ifdef LIBOOOGG_EXPORTS
		#define LIBOOOGG_API __declspec(dllexport)
	#else
		#define LIBOOOGG_API __declspec(dllimport)
	#endif
#endif

#include "common/TheoraTypes.h"

const REGPINTYPES TheoraEncodeInputTypes = {
    &MEDIATYPE_Video,
	&MEDIASUBTYPE_YV12
};


const REGPINTYPES TheoraEncodeOutputTypes = {
	&MEDIATYPE_Video,
	&MEDIASUBTYPE_Theora
};

const REGFILTERPINS TheoraEncodePinReg[] = {
	{
    L"Theora Input",					//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	FALSE,								//Not an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//upport two media type
	&TheoraEncodeInputTypes				//Pointer to media type (Video/VY12)
	} ,

	{
	L"Theora Output",					//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	TRUE,								//Is an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//Only support one media type
	&TheoraEncodeOutputTypes					//Pointer to media type (Audio/PCM)

	}
};



const REGFILTER2 TheoraEncodeFilterReg = {
		1,
		MERIT_DO_NOT_USE,
		2,
        TheoraEncodePinReg
		
};
