/*
   Copyright (C) 2003, 2004 Zentaro Kavanagh

   Copyright (C) 2003, 2004 Commonwealth Scientific and Industrial Research
   Organisation (CSIRO) Australia

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of CSIRO Australia nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include ".\headtag.h"



namespace illiminable {
namespace libCMMLTagsDotNET {

	HeadTag::HeadTag(void)
	{
		mBaseClass = new C_HeadTag;
	}

	HeadTag::HeadTag(C_HeadTag* inTag, bool inDeleteBase)
	{
		mBaseClass = inTag;
		mDeleteBase = inDeleteBase;
		
	}

	HeadTag::~HeadTag(void)
	{
		if (mDeleteBase) {
			delete mBaseClass;
		}
		mBaseClass = NULL;
	}

	String* HeadTag::profile() {
		return Wrappers::WStrToNetStr( getMe()->profile().c_str());
	}
	TitleTag* HeadTag::title() {
		//Title is gauranteed to exist.
		return new TitleTag(getMe()->title()->clone());
	}
	BaseTag* HeadTag::base() {
		if (getMe()->base() != NULL) {
			return new BaseTag(getMe()->base(), false);
		} else {
			return NULL;
		}
	}
	MetaTagList* HeadTag::metaList() {
		return new MetaTagList(getMe()->metaList(), false);


	}

			//Mutators
	void HeadTag::setProfile(String* inProfile) {
		wchar_t* tc = Wrappers::netStrToWStr( inProfile );
		wstring locStr = tc;
		getMe()->setProfile( locStr );
		Wrappers::releaseWStr( tc );

	}	
	void HeadTag::setTitle(TitleTag* inTitle){
		//TODO::: Maybe this needs code to handle a null pointer being passed in.
		getMe()->setTitle(inTitle->getMe()->clone());
	}
	void HeadTag::setBase(BaseTag* inBase) {
		getMe()->setBase(inBase->getMe()->clone());
	}

			//Other
	String* HeadTag::toString() {

		return Wrappers::WStrToNetStr( getMe()->toString().c_str());
		

	}

	C_HeadTag* HeadTag::getMe() {
		return (C_HeadTag*)mBaseClass;
	}

}
}
