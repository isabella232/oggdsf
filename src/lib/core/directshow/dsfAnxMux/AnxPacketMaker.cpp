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
#include "StdAfx.h"
#include ".\anxpacketmaker.h"

AnxPacketMaker::AnxPacketMaker(void)
{
}

AnxPacketMaker::~AnxPacketMaker(void)
{
}

OggPage* AnxPacketMaker::makeAnnodexBOS_2_0	(					unsigned long inSerialNo
														,	unsigned short inVersionMajor
														,	unsigned short inVersionMinor
														,	unsigned __int64 inTimebaseNum
														,	unsigned __int64 inTimebaseDenom
														,	const unsigned char* inUTC
														
													)
{
	unsigned char* locBuff = NULL;
	StampedOggPacket* locPack = NULL;
	OggPage* retPage = NULL;
	unsigned char* locSegTable = NULL;
	switch (inVersionMajor) {
		case 2:
			locBuff = new unsigned char[ANX_2_0_ANNODEX_BOS_SIZE];

			//	0	-	7		Annodex\0
			//  8	-   9		Version Major
			// 10	-  11		Version Minor
			// 12	-  19		Timebase Num
			// 20	-  27		Timebase Denom
			// 28	=  48		UTC
			locBuff[0] = 'A';
			locBuff[1] = 'n';
			locBuff[2] = 'n';
			locBuff[3] = 'o';
			locBuff[4] = 'd';
			locBuff[5] = 'e';
			locBuff[6] = 'x';
			locBuff[7] = 0;
			iLE_Math::UShortToCharArr(inVersionMajor, locBuff + 8);
			iLE_Math::UShortToCharArr(inVersionMinor, locBuff + 10);
			iLE_Math::Int64ToCharArr(inTimebaseNum, locBuff + 12);
			iLE_Math::Int64ToCharArr(inTimebaseDenom, locBuff + 20);
			for (int i = 0; i < 20; i++) {
				locBuff[28 + i] = inUTC[i];
			}
			
			locPack = new StampedOggPacket(locBuff, ANX_2_0_ANNODEX_BOS_SIZE, false, false, 0, 0, StampedOggPacket::OGG_END_ONLY);

			retPage = new OggPage;
			retPage->header()->setHeaderFlags(2);
			retPage->header()->setGranulePos((__int64)0);
			retPage->header()->setNumPageSegments( 1);
			locSegTable = new unsigned char[1];
			locSegTable[0] = ANX_2_0_ANNODEX_BOS_SIZE;
			retPage->header()->setSegmentTable(locSegTable, 1);
			retPage->header()->setHeaderSize(28);
			retPage->header()->setDataSize(ANX_2_0_ANNODEX_BOS_SIZE);

			retPage->header()->setStreamSerialNo(inSerialNo);
			retPage->addPacket(locPack);
			
			return retPage;

		default:
			return NULL;


	}
}

StampedOggPacket* AnxPacketMaker::makeAnxData_2_0	(			unsigned short inVersionMajor
														,	unsigned short inVersionMinor
														,	unsigned __int64 inGranuleRateNum
														,	unsigned __int64 inGranuleRateDenom
														,	unsigned long inNumSecHeaders
														,	vector<string> inMessageHeaders
												)
{

	StampedOggPacket* locPack = NULL;
	//OggPage* retPage = NULL;
	unsigned char* locBuff = NULL;
	unsigned long locUpto = 0;
	unsigned long locPacketSize = 0;
	switch (inVersionMajor) {
		case 2:
			//ASSERT(inMessageHeaders.size() > 0)
			//THere has to be a content type field.

			locPacketSize = 28;  //Base header size
			for (int i = 0; i < inMessageHeaders.size(); i++) {
				//2 is the crlf
				locPacketSize += inMessageHeaders[i].size() + 2;
			}

			//terminating crlf
			locPacketSize += 2;

			locBuff = new unsigned char[locPacketSize];

			locBuff[0] = 'A';
			locBuff[1] = 'n';
			locBuff[2] = 'x';
			locBuff[3] = 'D';
			locBuff[4] = 'a';
			locBuff[5] = 't';
			locBuff[6] = 'a';
			locBuff[7] = 0;

			iLE_Math::Int64ToCharArr(inGranuleRateNum, locBuff + 8);
			iLE_Math::Int64ToCharArr(inGranuleRateDenom, locBuff + 16);
			iLE_Math::ULongToCharArr(inNumSecHeaders, locBuff + 24);
			locUpto = 28;
			for (int i = 0; i < inMessageHeaders.size(); i++) {
				memcpy((void*)(locBuff + locUpto), (const void*)(inMessageHeaders[i].c_str()), inMessageHeaders[i].size());
				locUpto += inMessageHeaders[i].size();
				//TODO::: How terminated ??
				locBuff[locUpto++] = '\r';	
				locBuff[locUpto++] = '\n';
			}

			locBuff[locUpto++] = '\r';
			locBuff[locUpto++] = '\n';
			
			locPack = new StampedOggPacket(locBuff, locPacketSize, false, false, 0, 0, StampedOggPacket::OGG_END_ONLY);
			return locPack;

			


		default:
			return NULL;
	}
}

//StampedOggPacket* AnxPacketMaker::makeAnxData_2_0 (OggMuxStream* inMuxStream, OggPaginator* inPaginator) 
//{
//	return makeAnxData_2_0(		2
//							,	0
//							,	inMuxStream->granuleNumerator()
//							,	inMuxStream->granuleDenominator()
//							,	inPaginator->numHeaders()
//							,	makeMessageHeaders(inMuxStream));
//}

//StreamHeaders::eCodecType AnxPacketMaker::IdentifyCodec(OggPacket* inOggPacket) {
//	if (strncmp((char*)inOggPacket->packetData(), "\001vorbis", 7) == 0) {
//		return StreamHeaders::VORBIS;
//	} else if (strncmp((char*)inOggPacket->packetData(), "Speex   ", 8) == 0) {
//		return StreamHeaders::SPEEX;
//	} else if ((strncmp((char*)inOggPacket->packetData(), "fLaC", 4)) == 0) {
//		return StreamHeaders::FLAC;
//	} else if ((strncmp((char*)inOggPacket->packetData(), "\177FLAC", 5)) == 0) {
//		return StreamHeaders::OGG_FLAC_1_0;
//	} else if ((strncmp((char*)inOggPacket->packetData(), "\200theora", 7)) == 0) {
//		return StreamHeaders::THEORA;
//	} else if ((strncmp((char*)inOggPacket->packetData(), "\001video\000\000\000", 9)) == 0) {
//		return StreamHeaders::FFDSHOW_VIDEO;
//	}
//	
//	return StreamHeaders::NONE;
//	
//}


//vector<string> AnxPacketMaker::makeMessageHeaders(OggMuxStream* inMuxStream) {
//	string locTempString = "";
//	vector<string> retVector;
//	switch(IdentifyCodec(inMuxStream->peekFront()->getPacket(0))) {
//		case StreamHeaders::VORBIS:
//			locTempString = "Content-type: audio/x-vorbis";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::SPEEX:
//			locTempString = "Content-type: audio/x-speex";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::FLAC:
//			locTempString = "Content-type: audio/x-flac";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::OGG_FLAC_1_0:
//			locTempString = "Content-type: audio/x-flac_1_0";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::THEORA:
//			locTempString = "Content-type: video/x-theora";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::FFDSHOW_VIDEO:
//			locTempString = "Content-type: video/x-ogm";
//			retVector.push_back(locTempString);
//			break;
//		case StreamHeaders::NONE:
//		default:
//			break;
//	}
//
//	return retVector;
//}


vector<string> AnxPacketMaker::makeMessageHeaders(StreamHeaders::eCodecType inCodecType) {
	string locTempString = "";
	vector<string> retVector;
	switch(inCodecType) {
		case StreamHeaders::VORBIS:
			locTempString = "Content-type: audio/x-vorbis";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::SPEEX:
			locTempString = "Content-type: audio/x-speex";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::FLAC:
			locTempString = "Content-type: audio/x-flac";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::OGG_FLAC_1_0:
			locTempString = "Content-type: audio/x-flac_1_0";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::THEORA:
			locTempString = "Content-type: video/x-theora";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::FFDSHOW_VIDEO:
			locTempString = "Content-type: video/x-ogm";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::CMML:
			locTempString = "Content-type: text/x-cmml";
			retVector.push_back(locTempString);
			break;
		case StreamHeaders::NONE:
		default:
			break;
	}

	return retVector;
}
