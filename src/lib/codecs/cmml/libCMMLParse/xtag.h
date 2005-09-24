//===========================================================================
//Copyright (C) 2005 Commonwealth Scientific and Industrial Research
//                   Organisation (CSIRO) Australia
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


#ifndef __XTAG_H__
#define __XTAG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef XTAG_INTERNAL
typedef void XTag;
#endif /* XTAG_INTERNAL */

//XTag * xtag_new_parse (const char * s, int n);
XTag * xtag_new_parse (const char * s, int n, int* ErrorOffset);

char * xtag_get_name (XTag * xtag);

char * xtag_get_pcdata (XTag * xtag);

char * xtag_get_attribute (XTag * xtag, char * attribute);

XTag * xtag_first_child (XTag * xtag, char * name);

XTag * xtag_next_child (XTag * xtag, char * name);

XTag * xtag_free (XTag * xtag);

int xtag_snprint (char * buf, int n, XTag * xtag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XTAG_H__ */
