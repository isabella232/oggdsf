//===========================================================================
//Copyright (C) 2003, 2004 Zentaro Kavanagh
//
//Copyright (C) 2004 Commonwealth Scientific and Industrial Research
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

// AnxCutter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libOOOgg.h"
#include "dllstuff.h"

#include <iostream>
#include <fstream>

typedef pair<unsigned long, unsigned long> tSerial_HeadCountPair;

unsigned long bytePos;

bool gotAllHeaders;

fstream outputFile;
vector<tSerial_HeadCountPair> theStreams;

enum eDemuxState {
	SEEN_NOTHING,
	SEEN_ANNODEX_BOS,
	SEEN_ANNODEX_EOS,
	SEEN_ALL_CODEC_HEADERS,
	INVALID = 100
};

eDemuxState demuxState;

unsigned long annodexSerialNo;

bool writePageToOutputFile(OggPage* inOggPage) {
	outputFile.write((char*)inOggPage->createRawPageData(), inOggPage->pageSize());
	return true;
}

unsigned long headerCount(OggPacket* inPacket) {
	const unsigned short NUM_SEC_HEADERS_OFFSET = 24;
	return iLE_Math::charArrToULong(inPacket->packetData() + NUM_SEC_HEADERS_OFFSET);

}
//This will be called by the callback
bool pageCB(OggPage* inOggPage) {

	bool allEmpty = true;

	switch (demuxState) {

		case SEEN_NOTHING:
			if (		(inOggPage->numPackets() == 1)
					&&	(inOggPage->header()->isBOS())
					&&	(strncmp((char*)inOggPage->getPacket(0)->packetData(), "Annodex\0", 8) == 0)) {
				
				//Advance the state
				demuxState = SEEN_ANNODEX_BOS;

				//Remember the annodex streams serial no
				annodexSerialNo = inOggPage->header()->StreamSerialNo();

				//Write out the page.
				writePageToOutputFile(inOggPage);
			} else {

				demuxState = INVALID;
			}

			break;
		case SEEN_ANNODEX_BOS:
			if (		(inOggPage->numPackets() == 1)
					&&	(inOggPage->header()->isBOS())
					&&	(strncmp((char*)inOggPage->getPacket(0)->packetData(), "AnxData\0", 8) == 0)) {
				

				//Create an association of serial no and num headers
				tSerial_HeadCountPair locMap;
				locMap.first = inOggPage->header()->StreamSerialNo();
				locMap.second = headerCount(inOggPage->getPacket(0));
				
				//Add the association to the list
				theStreams.push_back(locMap);

				//Write the page out to the output file.
				writePageToOutputFile(inOggPage);
			} else if (			(inOggPage->header()->isEOS())
							&&	(inOggPage->header()->StreamSerialNo() == annodexSerialNo)) {

				//It's the Annodex EOS.
				demuxState = SEEN_ANNODEX_EOS;
			} else {
				demuxState = INVALID;
			}
			break;
		case SEEN_ANNODEX_EOS:
			for (int i = 0; i < theStreams.size(); i++) {
				if (theStreams[i].first == inOggPage->header()->StreamSerialNo()) {
					if (theStreams[i].second >= 1) {
						theStreams[i].second--;
					} else {
						demuxState = INVALID;
					}
				}
			}

			
			for (int i = 0; i < theStreams.size(); i++) {
				if (theStreams[i].second != 0) {
					allEmpty = false;
				}
			}

			if (allEmpty) {

				demuxState = SEEN_ALL_CODEC_HEADERS;
			}
			break;
		case SEEN_ALL_CODEC_HEADERS:
			break;
		case INVALID:
			break;
		default:
			break;
	}


	if (demuxState == INVALID) {
		cout << "Invalid file. Broken = very yes."<<endl;
	}
	delete inOggPage;

	return true;
}


int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	demuxState = SEEN_NOTHING;

	bytePos = 0;
	gotAllHeaders = false;

	int x;
	cin>>x;
	if (argc < 2) {
		cout<<"Usage : AnxCutter <input_filename> <output_filename> <start_time> <end_time>"<<endl;
	} else {
		OggDataBuffer testOggBuff;
		
		testOggBuff.registerStaticCallback(&pageCB);

		fstream inputFile;
		
		inputFile.open(argv[1], ios_base::in | ios_base::binary);
		outputFile.open(argv[2], ios_base::out | ios_base::binary);
		
		const unsigned short BUFF_SIZE = 8092;
		char* locBuff = new char[BUFF_SIZE];
		while (demuxState < SEEN_ALL_CODEC_HEADERS) {
			inputFile.read(locBuff, BUFF_SIZE);
			unsigned long locBytesRead = inputFile.gcount();
    		testOggBuff.feed((const unsigned char*)locBuff, locBytesRead);
		}


		inputFile.close();
		outputFile.close();
		delete[] locBuff;
	}

	return 0;
}

