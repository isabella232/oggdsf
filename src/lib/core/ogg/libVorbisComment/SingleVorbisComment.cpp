//===========================================================================
//Copyright (C) 2004 Zentaro Kavanagh
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
#include "singlevorbiscomment.h"

SingleVorbisComment::SingleVorbisComment(void)
{
}

SingleVorbisComment::~SingleVorbisComment(void)
{
}


string SingleVorbisComment::key() {
	return mKey;
}
bool SingleVorbisComment::setKey(string inKey) {
	//FIX::: Need to do a check here for invalid chars
	mKey = inKey;
	return true;
}

string SingleVorbisComment::value() {
	return mValue;
}
bool SingleVorbisComment::setValue(string inValue) {
	//FIX::: Need to do a check here for invalid chars
	mValue = inValue;
	return true;
}

string SingleVorbisComment::toString() {
	string retStr = mKey+"="+mValue;
	return retStr;
}

unsigned long SingleVorbisComment::length() {
	//FIX::: This would be faster to add them manually.
	return toString().length();
}

bool SingleVorbisComment::parseComment(string inCommentString) {
	size_t pos = 0;
	size_t pos2 = 0;
	pos = inCommentString.find('=');
	if ((pos == string::npos) && (pos != 0)) {
		//FAILED - No equals sign
		return false;
	} else {
		pos2 = inCommentString.find('=', pos + 1);
		if (pos2 == string::npos) {
			//OK - no other equals signs
			mKey = inCommentString.substr(0, pos);
			mValue = inCommentString.substr(pos + 1);
		} else {
			//FAILED : Too many = signs
			return false;
		}

		return true;
	}


}