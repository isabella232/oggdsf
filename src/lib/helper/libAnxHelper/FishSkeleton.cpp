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

#include "stdafx.h"
#include ".\fishskeleton.h"

FishSkeleton::FishSkeleton(void)
{
}

FishSkeleton::~FishSkeleton(void)
{
}

	//Checksum tables from libogg
static unsigned long fish_crc_lookup[256]={
  0x00000000,0x04c11db7,0x09823b6e,0x0d4326d9,
  0x130476dc,0x17c56b6b,0x1a864db2,0x1e475005,
  0x2608edb8,0x22c9f00f,0x2f8ad6d6,0x2b4bcb61,
  0x350c9b64,0x31cd86d3,0x3c8ea00a,0x384fbdbd,
  0x4c11db70,0x48d0c6c7,0x4593e01e,0x4152fda9,
  0x5f15adac,0x5bd4b01b,0x569796c2,0x52568b75,
  0x6a1936c8,0x6ed82b7f,0x639b0da6,0x675a1011,
  0x791d4014,0x7ddc5da3,0x709f7b7a,0x745e66cd,
  0x9823b6e0,0x9ce2ab57,0x91a18d8e,0x95609039,
  0x8b27c03c,0x8fe6dd8b,0x82a5fb52,0x8664e6e5,
  0xbe2b5b58,0xbaea46ef,0xb7a96036,0xb3687d81,
  0xad2f2d84,0xa9ee3033,0xa4ad16ea,0xa06c0b5d,
  0xd4326d90,0xd0f37027,0xddb056fe,0xd9714b49,
  0xc7361b4c,0xc3f706fb,0xceb42022,0xca753d95,
  0xf23a8028,0xf6fb9d9f,0xfbb8bb46,0xff79a6f1,
  0xe13ef6f4,0xe5ffeb43,0xe8bccd9a,0xec7dd02d,
  0x34867077,0x30476dc0,0x3d044b19,0x39c556ae,
  0x278206ab,0x23431b1c,0x2e003dc5,0x2ac12072,
  0x128e9dcf,0x164f8078,0x1b0ca6a1,0x1fcdbb16,
  0x018aeb13,0x054bf6a4,0x0808d07d,0x0cc9cdca,
  0x7897ab07,0x7c56b6b0,0x71159069,0x75d48dde,
  0x6b93dddb,0x6f52c06c,0x6211e6b5,0x66d0fb02,
  0x5e9f46bf,0x5a5e5b08,0x571d7dd1,0x53dc6066,
  0x4d9b3063,0x495a2dd4,0x44190b0d,0x40d816ba,
  0xaca5c697,0xa864db20,0xa527fdf9,0xa1e6e04e,
  0xbfa1b04b,0xbb60adfc,0xb6238b25,0xb2e29692,
  0x8aad2b2f,0x8e6c3698,0x832f1041,0x87ee0df6,
  0x99a95df3,0x9d684044,0x902b669d,0x94ea7b2a,
  0xe0b41de7,0xe4750050,0xe9362689,0xedf73b3e,
  0xf3b06b3b,0xf771768c,0xfa325055,0xfef34de2,
  0xc6bcf05f,0xc27dede8,0xcf3ecb31,0xcbffd686,
  0xd5b88683,0xd1799b34,0xdc3abded,0xd8fba05a,
  0x690ce0ee,0x6dcdfd59,0x608edb80,0x644fc637,
  0x7a089632,0x7ec98b85,0x738aad5c,0x774bb0eb,
  0x4f040d56,0x4bc510e1,0x46863638,0x42472b8f,
  0x5c007b8a,0x58c1663d,0x558240e4,0x51435d53,
  0x251d3b9e,0x21dc2629,0x2c9f00f0,0x285e1d47,
  0x36194d42,0x32d850f5,0x3f9b762c,0x3b5a6b9b,
  0x0315d626,0x07d4cb91,0x0a97ed48,0x0e56f0ff,
  0x1011a0fa,0x14d0bd4d,0x19939b94,0x1d528623,
  0xf12f560e,0xf5ee4bb9,0xf8ad6d60,0xfc6c70d7,
  0xe22b20d2,0xe6ea3d65,0xeba91bbc,0xef68060b,
  0xd727bbb6,0xd3e6a601,0xdea580d8,0xda649d6f,
  0xc423cd6a,0xc0e2d0dd,0xcda1f604,0xc960ebb3,
  0xbd3e8d7e,0xb9ff90c9,0xb4bcb610,0xb07daba7,
  0xae3afba2,0xaafbe615,0xa7b8c0cc,0xa379dd7b,
  0x9b3660c6,0x9ff77d71,0x92b45ba8,0x9675461f,
  0x8832161a,0x8cf30bad,0x81b02d74,0x857130c3,
  0x5d8a9099,0x594b8d2e,0x5408abf7,0x50c9b640,
  0x4e8ee645,0x4a4ffbf2,0x470cdd2b,0x43cdc09c,
  0x7b827d21,0x7f436096,0x7200464f,0x76c15bf8,
  0x68860bfd,0x6c47164a,0x61043093,0x65c52d24,
  0x119b4be9,0x155a565e,0x18197087,0x1cd86d30,
  0x029f3d35,0x065e2082,0x0b1d065b,0x0fdc1bec,
  0x3793a651,0x3352bbe6,0x3e119d3f,0x3ad08088,
  0x2497d08d,0x2056cd3a,0x2d15ebe3,0x29d4f654,
  0xc5a92679,0xc1683bce,0xcc2b1d17,0xc8ea00a0,
  0xd6ad50a5,0xd26c4d12,0xdf2f6bcb,0xdbee767c,
  0xe3a1cbc1,0xe760d676,0xea23f0af,0xeee2ed18,
  0xf0a5bd1d,0xf464a0aa,0xf9278673,0xfde69bc4,
  0x89b8fd09,0x8d79e0be,0x803ac667,0x84fbdbd0,
  0x9abc8bd5,0x9e7d9662,0x933eb0bb,0x97ffad0c,
  0xafb010b1,0xab710d06,0xa6322bdf,0xa2f33668,
  0xbcb4666d,0xb8757bda,0xb5365d03,0xb1f740b4
};

  //End libogg tables

bool FishSkeleton::setChecksum(OggPage* inOggPage) 
{
	unsigned long locChecksum = 0;
	unsigned long locTemp = 0;

	unsigned char* locHeaderBuff = new unsigned char[300];
	if (inOggPage != NULL) {
		//Set the checksum to NULL for the checksumming process.
		inOggPage->header()->setCRCChecksum((unsigned long)0);
		inOggPage->header()->rawData(locHeaderBuff, 300);

		for(unsigned long i = 0; i < inOggPage->headerSize(); i++) {
			//Create the index we use for the lookup
			locTemp = ((locChecksum >> 24) & 0xff) ^ locHeaderBuff[i];
			//XOR the lookup value with the the current checksum shifted left 8 bits.
			locChecksum=(locChecksum << 8) ^ fish_crc_lookup[locTemp];
		}



		unsigned char* locBuff = NULL;
		for(unsigned long i = 0; i < inOggPage->numPackets(); i++) {
			locBuff = inOggPage->getPacket(i)->packetData();			//View only don't delete.

			for (unsigned long j = 0; j < inOggPage->getPacket(i)->packetSize(); j++) {
				locTemp = ((locChecksum >> 24) & 0xff) ^ locBuff[j];
                locChecksum = (locChecksum << 8) ^ fish_crc_lookup[locTemp];
			}
		}

		inOggPage->header()->setCRCChecksum(locChecksum);
	}

	delete[] locHeaderBuff;
	return true;

}

StampedOggPacket* FishSkeleton::makeCMMLBOS()
{
	const unsigned short CMML_BOS_SIZE = 28;
	unsigned char* locPackBuff = new unsigned char[CMML_BOS_SIZE];
	locPackBuff[0] = 'C';
	locPackBuff[1] = 'M';
	locPackBuff[2] = 'M';
	locPackBuff[3] = 'L';
	locPackBuff[4] = 0;
	locPackBuff[5] = 0;
	locPackBuff[6] = 0;
	locPackBuff[7] = 0;
	locPackBuff[8] = 2;
	locPackBuff[9] = 0;
	locPackBuff[10] = 0;
	locPackBuff[11] = 0;
	LOOG_INT64 locInt64 = 1000;
	iLE_Math::Int64ToCharArr(locInt64, locPackBuff + 12);
	locInt64 = 1;
	iLE_Math::Int64ToCharArr(locInt64, locPackBuff + 20);



	StampedOggPacket* locPacket = new StampedOggPacket(locPackBuff, CMML_BOS_SIZE, false, false, 0, 0, StampedOggPacket::OGG_BOTH);
	return locPacket;
}

StampedOggPacket* FishSkeleton::makeCMML_XML_Thing() {
	string locXMLString = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<!DOCTYPE cmml SYSTEM \"cmml.dtd\">\n<?cmml xmlns=\"http://www.annodex.net/cmml\"?>";


	unsigned char* locPackBuff = new unsigned char[locXMLString.size()];
	memcpy((void*)locPackBuff, (const void*)locXMLString.c_str(), locXMLString.size());
	StampedOggPacket* locPacket = new StampedOggPacket(locPackBuff, locXMLString.size(), false, false, 0, 0, StampedOggPacket::OGG_BOTH);
	return locPacket;
}
OggPage* FishSkeleton::makeFishHeadBOS_3_0	(		unsigned long inSerialNo
												,	unsigned short inVersionMajor
												,	unsigned short inVersionMinor
												,	unsigned __int64 inPresentTimeNum
												,	unsigned __int64 inPresentTimeDenom
												,	unsigned __int64 inTimebaseNum
												,	unsigned __int64 inTimebaseDenom
												,	const unsigned char* inUTC
														
											)
{
	const LOOG_INT64 DEFAULT_TIMEBASE_DENOMINATOR = 1000;
	const LOOG_INT64 DEFAULT_PRESENTATION_TIME_DENOMINATOR = 1000;

	unsigned char* locBuff = NULL;
	StampedOggPacket* locPack = NULL;
	OggPage* retPage = NULL;
	unsigned char* locSegTable = NULL;

	switch (inVersionMajor) {
		case 3:
			locBuff = new unsigned char[FishSkeleton::FISHEAD_3_0_PACKET_SIZE];

			//	0	-	7		fishead\0
			//  8	-   9		Version Major
			// 10	-  11		Version Minor
			// 12	-  19		Presentation Num
			// 20	-  27		Presentation Denom
			// 28	-  35		Basetime Num
			// 36	-  43	    Basetime Denom
			// 44	-  63		UTC
			locBuff[0] = 'f';
			locBuff[1] = 'i';
			locBuff[2] = 's';
			locBuff[3] = 'h';
			locBuff[4] = 'e';
			locBuff[5] = 'a';
			locBuff[6] = 'd';
			locBuff[7] = 0;
			iLE_Math::UShortToCharArr(inVersionMajor, locBuff + 8);
			iLE_Math::UShortToCharArr(inVersionMinor, locBuff + 10);
			iLE_Math::Int64ToCharArr(inPresentTimeNum, locBuff + 12);

			// Ensure that the presentation time denominator is not zero
			if (inPresentTimeNum == 0 && inPresentTimeDenom == 0) {
				iLE_Math::Int64ToCharArr(DEFAULT_PRESENTATION_TIME_DENOMINATOR, locBuff + 20);
			} else {
				// XXX: Should we handle the case wher the numerator is not
				// zero and the denominator is zero?
				iLE_Math::Int64ToCharArr(inPresentTimeDenom, locBuff + 20);
			}

			iLE_Math::Int64ToCharArr(inTimebaseNum, locBuff + 28);

			// Ensure that the timebase denominator is not zero
			if (inTimebaseNum == 0 && inTimebaseDenom == 0) {
				iLE_Math::Int64ToCharArr(DEFAULT_TIMEBASE_DENOMINATOR, locBuff + 36);
			} else {
				// XXX: Should we handle the case wher the numerator is not
				// zero and the denominator is zero?
				iLE_Math::Int64ToCharArr(inTimebaseDenom, locBuff + 36);
			}

			
			for (int i = 0; i < 20; i++) {
				locBuff[44 + i] = inUTC[i];
			}
			
			locPack = new StampedOggPacket(locBuff, FishSkeleton::FISHEAD_3_0_PACKET_SIZE, false, false, 0, 0, StampedOggPacket::OGG_END_ONLY);

			retPage = new OggPage;
			retPage->header()->setHeaderFlags(2);
			retPage->header()->setGranulePos((__int64)0);
			retPage->header()->setNumPageSegments( 1);
			locSegTable = new unsigned char[1];
			locSegTable[0] = FishSkeleton::FISHEAD_3_0_PACKET_SIZE;
			retPage->header()->setSegmentTable(locSegTable, 1);
			retPage->header()->setHeaderSize(28);
			retPage->header()->setDataSize(FishSkeleton::FISHEAD_3_0_PACKET_SIZE);

			retPage->header()->setStreamSerialNo(inSerialNo);
			retPage->addPacket(locPack);

			setChecksum(retPage);
			
			return retPage;

		default:
			return NULL;


	}
}

OggPage* FishSkeleton::makeFishEOS (unsigned long inSerialNo) {
	OggPage* retPage = new OggPage();
	StampedOggPacket* locDudPacket = new StampedOggPacket(NULL, 0, false, false, 0, 0, StampedOggPacket::OGG_BOTH);



	retPage->header()->setNumPageSegments(1);
	unsigned char* locSegTable = new unsigned char[1];

	locSegTable[0] = 0;
	

	retPage->header()->setHeaderFlags(4);
	retPage->header()->setSegmentTable(locSegTable, 1);
	retPage->header()->setHeaderSize(28);
	retPage->header()->setDataSize(0);

	retPage->header()->setStreamSerialNo(inSerialNo);
	retPage->addPacket(locDudPacket);

	setChecksum(retPage);

	return retPage;

}
OggPage* FishSkeleton::makeFishBone_3_0_Page (StampedOggPacket* inFishBonePacket, unsigned long inFishStreamSerial)
{
			StampedOggPacket* locPack = inFishBonePacket;
			OggPage* retPage = new OggPage;
			retPage->header()->setHeaderFlags(0);
			retPage->header()->setGranulePos((__int64)0);
			
			unsigned long locDataSize = locPack->packetSize();

			

			unsigned long locNumSegs = ((locDataSize / 255) + 1);

			unsigned char locLastSeg = locDataSize % 255;

			retPage->header()->setNumPageSegments(locNumSegs);
			unsigned char* locSegTable = new unsigned char[locNumSegs];

			for (int i = 0; i < locNumSegs - 1; i++) {
				locSegTable[i] = 255;
			}

			locSegTable[locNumSegs - 1] = locLastSeg;
			
			retPage->header()->setSegmentTable(locSegTable, locNumSegs);
			retPage->header()->setHeaderSize(27 + locNumSegs);
			retPage->header()->setDataSize(locDataSize);

			retPage->header()->setStreamSerialNo(inFishStreamSerial);
			retPage->addPacket(locPack);

			setChecksum(retPage);

			
			return retPage;
}



//OggPage* FishSkeleton::makeFishBone_3_0_Page			(		unsigned __int64 inGranuleRateNum
//														,	unsigned __int64 inGranuleRateDenom
//														,	unsigned __int64 inBaseGranule
//														,	unsigned long inNumSecHeaders
//														,	unsigned long inSerialNo
//														,	unsigned short inGranuleShift
//														,	unsigned char inPreroll
//														,	vector<string> inMessageHeaders
//														,	unsigned long inFishStreamSerial
//													)
//{
//			StampedOggPacket* locPack = makeFishBone_3_0(inGranuleRateNum, inGranuleRateDenom, inBaseGranule, inNumSecHeaders, inSerialNo, inGranuleShift, inPreroll,inMessageHeaders);
//			OggPage* retPage = new OggPage;
//			retPage->header()->setHeaderFlags(0);
//			retPage->header()->setGranulePos((__int64)0);
//			
//			unsigned long locDataSize = locPack->packetSize();
//
//			
//
//			unsigned long locNumSegs = ((locDataSize / 255) + 1);
//
//			unsigned char locLastSeg = locDataSize % 255;
//
//			retPage->header()->setNumPageSegments(locNumSegs);
//			unsigned char* locSegTable = new unsigned char[locNumSegs];
//
//			for (int i = 0; i < locNumSegs - 1; i++) {
//				locSegTable[i] = 255;
//			}
//
//			locSegTable[locNumSegs - 1] = locLastSeg;
//			
//			retPage->header()->setSegmentTable(locSegTable, locNumSegs);
//			retPage->header()->setHeaderSize(27 + locNumSegs);
//			retPage->header()->setDataSize(locDataSize);
//
//			retPage->header()->setStreamSerialNo(inFishStreamSerial);
//			retPage->addPacket(locPack);
//
//			return retPage;
//}
StampedOggPacket* FishSkeleton::makeFishBone_3_0	(		unsigned __int64 inGranuleRateNum
														,	unsigned __int64 inGranuleRateDenom
														,	unsigned __int64 inBaseGranule
														,	unsigned long inNumSecHeaders
														,	unsigned long inSerialNo
														,	unsigned char inGranuleShift
														,	unsigned long inPreroll
														,	vector<string> inMessageHeaders
													) 
{

	//	0	-	7		:	fisbone/0
	//	8	-	11		:	Bytes to message headers
	//	12	-	15		:	Serial NO
	//	16	-	19		:	Num Headers
	//	20	-	27		:	Granule Numerator
	//	28	-	35		:	Granule Denominator
	//	36	-	43		:	Base granule
	//	44	-	47		:	Preroll
	//	48	-	48		:	Granule shift
	//	49	-	51		:	*** PADDING ***
	//	52	-			:	Message Headers



		unsigned long locPacketSize = 52;  //Base header size
		for (size_t i = 0; i < inMessageHeaders.size(); i++) {
			//2 is the crlf
			locPacketSize += (unsigned long)(inMessageHeaders[i].size() + 2);
		}

		//terminating crlf
		//locPacketSize += 2;

		unsigned char* locBuff = new unsigned char[locPacketSize];

		locBuff[0] = 'f';
		locBuff[1] = 'i';
		locBuff[2] = 's';
		locBuff[3] = 'b';
		locBuff[4] = 'o';
		locBuff[5] = 'n';
		locBuff[6] = 'e';
		locBuff[7] = 0;

		iLE_Math::ULongToCharArr(44, locBuff + 8);
		iLE_Math::ULongToCharArr(inSerialNo, locBuff + 12);
		iLE_Math::ULongToCharArr(inNumSecHeaders, locBuff + 16);

		iLE_Math::Int64ToCharArr(inGranuleRateNum, locBuff + 20);
		iLE_Math::Int64ToCharArr(inGranuleRateDenom, locBuff + 28);

		iLE_Math::Int64ToCharArr(inBaseGranule, locBuff + 36);
		iLE_Math::ULongToCharArr(inPreroll, locBuff + 44);

		locBuff[48] = inGranuleShift;
		locBuff[49] = 0;
		locBuff[50] = 0;
		locBuff[51] = 0;

		

		
		unsigned long locUpto = 52;
		for (size_t i = 0; i < inMessageHeaders.size(); i++) {
			memcpy((void*)(locBuff + locUpto), (const void*)(inMessageHeaders[i].c_str()), inMessageHeaders[i].size());
			locUpto += (unsigned long)(inMessageHeaders[i].size());
			//TODO::: How terminated ??
			locBuff[locUpto++] = '\r';	
			locBuff[locUpto++] = '\n';
		}

		//locBuff[locUpto++] = '\r';
		//locBuff[locUpto++] = '\n';
		
		StampedOggPacket* locPack = new StampedOggPacket(locBuff, locPacketSize, false, false, 0, 0, StampedOggPacket::OGG_END_ONLY);
		return locPack;




}