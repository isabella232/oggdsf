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
#pragma once

//STL Include Files
#include <string>

#include <libCMMLTags/config.h>

using namespace std;

//DESCRIPTION:
//	This is the base class for all tags. This is an abstract class.

//PURE VIRTUAL FUNCTIONS
//	wstring toString();					//Turn the tag to a string

class LIBCMMLTAGS_API C_CMMLTag
{
public:
	//Constructors
	C_CMMLTag(void);
	virtual ~C_CMMLTag(void);

	virtual C_CMMLTag* genericClone() = 0;
	

	//Enumerations
	enum eTagType {
		UNKNOWN,
		IMPORT,
		STREAM,
		META,
		HEAD,
		TITLE,
		BASE,
		CLIP,
		ANCHOR,
		IMAGE,
		DESC,
		PARAM,
		BAD_TAG = 1000
	};
	
	/// Returns the id attribute for this tag.
	wstring id();

	/// Sets the id attribute for this tag.
	void setId(wstring inId);

	/// Returns the type of tag this really is.
	eTagType tagType();

	/// Pure Virtual : Returns an xml string representing this tag.
	virtual wstring toString() = 0;
protected:
	//Property Data
	wstring mId;
	eTagType mTagType;

	//Protected Helper Methods
	virtual void privateClone(C_CMMLTag* outTag);

	/// Makes an attribute of the form name="content" and returns a string.
	wstring makeAttribute(wstring inElemName, wstring inElemContent);

	wstring makeRequiredAttribute(wstring inElemName, wstring inElemContent);

	/// Escapes out & " < > ' and turns it into an entity reference
	wstring escapeEntities(wstring inString);

	/// Implements the missing functionality from an stl string... ie replace
	wstring replaceAll(wstring inOriginal, wchar_t inReplaceThis, wstring inWithThis);
	
};
