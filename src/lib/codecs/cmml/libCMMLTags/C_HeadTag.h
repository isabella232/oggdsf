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
#include <libCMMLTags/C_TitleTag.h>
#include <libCMMLTags/C_BaseTag.h>
#include <libCMMLTags/C_MetaTagList.h>

class LIBCMMLTAGS_API C_HeadTag 
	//Derived Classes
	: public C_HumReadCMMLTag
{
public:
	//Constructors
	C_HeadTag(void);
	virtual ~C_HeadTag(void);

	/// Returns uri for profile used in meta tags (eg Dublin Core)
	wstring profile();

	/// Returns an internal pointer to the title element. Can modify but don't delete.
	C_TitleTag* title();

	/// Returns an internal pointer to the base element. Can modify but don't delete.
	C_BaseTag* base();

	/// Returns an internal pointer to the meta tag list. Can modify but don't delete.
	C_MetaTagList* metaList();

	/// Sets the uri for the profile used in meta tags.
	void setProfile(wstring inProfile);

	/// Sets the title tag for this element. You give away your pointer.
	void setTitle(C_TitleTag* inTitle);

	/// Sets the base tag for this element. You give away your pointer.
	void setBase(C_BaseTag* inBase);

	/// Sets the metalist used for this element. You give away your pointer.
	void setMetaList(C_MetaTagList* inMetaList);

	/// Returns an xml representation of this tag.
	virtual wstring toString();

	/// Performs a deep copy and returns a pointer you can keep.
	C_HeadTag* clone();

	/// Performs a deep copy and returns a pointer to the base class you can keep.
	virtual C_CMMLTag* genericClone();
protected:
	//Property Data
	wstring mProfile;
	C_TitleTag* mTitle;
	C_BaseTag* mBase;
	C_MetaTagList* mMetaList;

	//Protected Helper Methods
	virtual void privateClone(C_CMMLTag* outTag);


};

	//typedef struct {
	//	char *head_id;    /**< id attribute of header */
	//	char *lang;       /**< language of header */
	//	char *dir;        /**< directionality of lang */
	//	char *profile;    /**< profile of header */
	//	char *title;      /**< title element of header */
	//	char *title_id;   /**< id attribute of title element */
	//	char *title_lang; /**< language of title */
	//	char *title_dir;  /**< directionality of title_lang */
	//	char *base_id;    /**< id attribute of base element */
	//	char *base_href;  /**< href attribute of base element */
	//	CMML_List * meta; /**< list of meta elements */
	//} CMML_Head;
