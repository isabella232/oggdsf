#include "StdAfx.h"
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

	OggPage* locBOSPage =			AnxPacketMaker::makeAnnodexBOS(		mAnxSerialNo
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
	//TODO::: Fill this in.

	bool locWasAny = false;
	bool locIsOK = true;
	for (int i = 0; i < mInputStreams.size(); i++) {
		locWasAny = true;
		if ((mInputStreams[i]->peekFront() != NULL) || (!mInputStreams[i]->isActive())) {
			locIsOK = locIsOK && true;
		} else {
			locIsOK = false;
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
