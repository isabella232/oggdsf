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

#include "theoraencoderdllstuff.h"
#include "ITheoraEncodeSettings.h"
#include "PropsTheoraEncoder.h"


//Forward Declarations
//struct sTheoraFormatBlock;
class TheoraEncodeInputPin;
class TheoraEncodeOutputPin;

class TheoraEncodeFilter
	//Base classes
	:	public AbstractTransformFilter
	,	public ITheoraEncodeSettings
	,	public ISpecifyPropertyPages
{
public:
	//Friends classes
	friend class TheoraEncodeInputPin;
	friend class TheoraEncodeOutputPin;

	//Constructors and Destructors
	TheoraEncodeFilter(void);
	virtual ~TheoraEncodeFilter(void);

	//COM Functions
	DECLARE_IUNKNOWN
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	//ITheoraEncodeSettings Implementation
	STDMETHODIMP_(unsigned long) targetBitrate();
	STDMETHODIMP_(unsigned char) quality();
	STDMETHODIMP_(unsigned long) keyframeFreq();

	STDMETHODIMP_(bool) setTargetBitrate(unsigned long inBitrate);
	STDMETHODIMP_(bool) setQuality(unsigned char inQuality);
	STDMETHODIMP_(bool) setKeyframeFreq(unsigned long inKeyframeFreq);
	//


	
	//SpecifyPropertyPages Implementation
	STDMETHODIMP TheoraEncodeFilter::GetPages(CAUUID* outPropPages);

protected:

	//AbstractVideoEncodeFilter pure virtuals
	virtual bool ConstructPins();

	//Member data
	sTheoraFormatBlock mTheoraFormatBlock;
	
};
