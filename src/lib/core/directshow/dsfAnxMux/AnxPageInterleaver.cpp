//===========================================================================
//Copyright (C) 2003, 2004 Zentaro Kavanagh
//
//Copyright (C) 2003, 2004 Commonwealth Scientific and Industrial Research
//   Organisation (CSIRO) Australia
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
#include ".\anxpageinterleaver.h"

AnxPageInterleaver::AnxPageInterleaver(IOggCallback* inFileWriter, INotifyComplete* inNotifier, unsigned long inVersionMajor, unsigned long inVersionMinor)
	:	OggPageInterleaver(inFileWriter, inNotifier)
	,	mVersionMajor(inVersionMajor)
	,	mVersionMinor(inVersionMinor)
	,	mIsAnxSetup(false)
{
	//This is the Annodex Logical Stream
	//mInputStreams.push_back(newStream());

	//Just make a random serial number
	mAnxSerialNo = 999;
}

AnxPageInterleaver::~AnxPageInterleaver(void)
{

}

void AnxPageInterleaver::addAnnodex_2_0_BOS() {
	//Find the CMML data
	//TODO::: Need to grab the data from CMML page

	unsigned char* locUTC = new unsigned char[20];
	memset((void*)locUTC, 0, 20);

	OggPage* locBOSPage =			AnxPacketMaker::makeAnnodexBOS_2_0(		mAnxSerialNo
																	,	2
																	,	0
																	,	0
																	,	0
																	,	locUTC);

	
	//Put the annodex BOS out to the file.
	mFileWriter->acceptOggPage(locBOSPage);
	delete locUTC;

}

void AnxPageInterleaver::addAllAnxData_2_0_BOS() {
	for (int i = 0; i < mInputStreams.size() - 1; i++) {
		mFileWriter->acceptOggPage(mInputStreams[i]->popFront());
	}

}

void AnxPageInterleaver::addAnnodexEOS() {
	//Make the EOS page and dump it
    
	OggPage* locEOSPage = new OggPage;

	locEOSPage->header()->setStreamSerialNo(mAnxSerialNo);
	locEOSPage->header()->setHeaderFlags(4);
	locEOSPage->header()->setHeaderSize(27);

	mFileWriter->acceptOggPage(locEOSPage);
}

bool AnxPageInterleaver::gotAllHeaders() {
	//TODO::: The isActive needs to be clarified so we don't start empty streams because wasany goes to true

	bool locWasAny = false;
	bool locIsOK = true;
	for (int i = 0; i < mInputStreams.size(); i++) {
		if (mInputStreams[i]->isActive()) {
			locWasAny = true;
			//if ((mInputStreams[i]->peekFront() != NULL) || (!mInputStreams[i]->isActive())) {
			if (mInputStreams[i]->numAvail() > 1) {
			
				locIsOK = locIsOK && true;
			} else {
				locIsOK = false;
			}
		}
	}

	return locWasAny && locIsOK;

	
}
void AnxPageInterleaver::processData()
{
	if ((mVersionMajor == 2) && (mVersionMinor == 0)) {
		//IF seen all headers
		//		CREATE ANNODEX BOS
		//		CREATE ANXDATA BOS's
		//		CREATE ANNODEX EOS
		//ELSE
		//		DO NOTHING
		//ENDIF
		//
	
		if (!mIsAnxSetup) {
			if (gotAllHeaders()) {
				mIsAnxSetup = true;
				addAnnodex_2_0_BOS();
				addAllAnxData_2_0_BOS();
				addAnnodexEOS();	
				//TODO::: Pump out these start pages.
			}
		} else {
			OggPageInterleaver::processData();
		}
	} else {
		throw 0;
	}
}
