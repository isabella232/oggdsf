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
using namespace std;

//Local Include Files
#include <libCMMLTags/C_HumReadCMMLTag.h>
#include <libCMMLTags/C_ParamTagList.h>
#include <libCMMLTags/C_CMMLTime.h>

class LIBCMMLTAGS_API C_ImportTag
	//Derived Classes
	: public C_HumReadCMMLTag
{
public:
	//Constructors
	C_ImportTag(void);
	virtual ~C_ImportTag(void);

	/// Returns the granule rate for this imported media
	wstring granuleRate();

	/// Returns the content type (MIME type) of this imported media.
	wstring contentType();

	/// Returns the uri of the imported media.
	wstring src();

	/// Returns the start time for this imported media
	wstring start();

	/// Returns the end time for this imported media.
	wstring end();

	/// Returns the title of this imported media.
	wstring title();

	/// Returns an internal poitner to the param list. Can modify but don't delete.
	C_ParamTagList* paramList();

	/// Sets the granule rate for this imported media
	void setGranuleRate(wstring inGranuleRate);

	/// Sets the content type (MIME type) of this imported media.
	void setContentType(wstring inContentType);

	/// Sets the uri of the imported media.
	void setSrc(wstring inSrc);

	/// Sets the start time for this imported media
	void setStart(wstring inStart);

	/// Sets the end time for this imported media.
	void setEnd(wstring inEnd);

	/// Sets the title of this imported media.
	void setTitle(wstring inTitle);

	/// Sets the internal poitner to the param list. You give away your pointer.
	void setParamList(C_ParamTagList* inParamList);

	/// Returns an xml representation of this tag.
	virtual wstring toString();

	/// Performs a deep copy and returns a pointer you can keep.
	C_ImportTag* clone();

	/// Performs a deep copy and returns a pointer to the base class you can keep.
	virtual C_CMMLTag* genericClone();

protected:
	//Property Data

	//FIX ::: Change this to appropriate integer type ??
	wstring mGranuleRate;
	wstring mContentType;
	wstring mSrc;
	wstring mStart;
	wstring mEnd;
	wstring mTitle;
	C_ParamTagList* mParamList;

	//Protected Helper Methods
	virtual void privateClone(C_CMMLTag* outTag);


};


	//Original structure
	//
	//typedef struct {
	//	char *id;               /**< id of import tag */
	//	char *lang;             /**< language code of import tag */
	//	char *dir;              /**< directionality of lang (ltr/rtl) */
	//	char *granulerate;      /**< base temporal resolution in Hz */
	//	char *contenttype;      /**< content type of the import bitstream */
	//	char *src;              /**< URI to import document */
	//	CMML_Time * start_time; /**< insertion time in annodex bitstream */
	//	CMML_Time * end_time;   /**< end time of this logical  bitstream */
	//	char *title;            /**< comment on the import bitstream */
	//	CMML_List * param;      /**< list of optional further nam-value
	//		     metadata for the import bitstreams */
	//} CMML_ImportElement;
	//
