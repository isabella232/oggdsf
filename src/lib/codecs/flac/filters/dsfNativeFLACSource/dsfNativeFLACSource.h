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

#include "dsfNativeFLACSource.h"
#include "NativeFLACSourceFilter.h"
#include "NativeFLACSourcePin.h"
// {6DDA37BA-0553-499a-AE0D-BEBA67204548}
DEFINE_GUID(CLSID_NativeFLACSourceFilter, 
0x6dda37ba, 0x553, 0x499a, 0xae, 0xd, 0xbe, 0xba, 0x67, 0x20, 0x45, 0x48);





const REGPINTYPES FLACSourceOutputTypes = {
	&MEDIATYPE_Audio,
	&MEDIASUBTYPE_PCM
};

const REGFILTERPINS FLACSourcePinReg[] = {
	{
	L"PCM Output",						//Name (obsoleted)
	FALSE,								//Renders from this pin ?? Not sure about this.
	TRUE,								//Is an output pin
	FALSE,								//Cannot have zero instances of this pin
	FALSE,								//Cannot have more than one instance of this pin
	NULL,								//Connects to filter (obsoleted)
	NULL,								//Connects to pin (obsoleted)
	1,									//Only support one media type
	&FLACSourceOutputTypes					//Pointer to media type (Audio/PCM)

	}
};



const REGFILTER2 FLACEncodeFilterReg = {
		1,
		MERIT_DO_NOT_USE,
		0,
        FLACSourcePinReg
		
};
	   

