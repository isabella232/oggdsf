//===========================================================================
//Copyright (C) 2003, 2004 Zentaro Kavanagh
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
#include "oggstream.h"

OggStream::OggStream(OggPage* inBOSPage, OggDemuxSourceFilter* inOwningFilter, bool inAllowSeek)
	:	mCodecHeaders(NULL)
	,	mOwningFilter(inOwningFilter)
	,	mAllowSeek(inAllowSeek)
	,	mSerialNo(0)
	,	mSourcePin(NULL)
	,	mNumHeadersNeeded(0)
	,	mStreamReady(false)
	//,	mPartialPacket(NULL)
	,	mFirstRun(true)
	,	mSendExcess(true)
	,	mLastEndGranulePos(0)
	,	mLastStartGranulePos(0)
	,	mStreamLock(NULL)
	,	mAllowDispatch(false)
{
	//osDebug.open("C:\\ostream.log", ios_base::out);
	//Need to do something here !
	mSerialNo = inBOSPage->header()->StreamSerialNo();
	mStreamLock = new CCritSec;
	//This may need to be moved to derived class
	//Yep, Sure did !
	//InitCodec(inBOSPage->getPacket(0));

	//Set the packetisers callback to our acceptstampedoggpacket function.
	mPacketiser.setPacketSink(this);

}

OggStream::~OggStream(void)
{
	//osDebug.close();
	delete mSourcePin;
	delete mCodecHeaders;
	//delete mPartialPacket;
	delete mStreamLock;
	//Don't try to delete owning filter !!
}

bool OggStream::streamReady() {
	return mStreamReady;

}

unsigned long OggStream::serialNo() {
	return mSerialNo;
}
void OggStream::setAllowDispatch(bool inAllowDispatch) {
	mAllowDispatch = inAllowDispatch;
}
bool OggStream::acceptStampedOggPacket(StampedOggPacket* inPacket) {
    if (!mStreamReady) {
		//Streams not ready, still headers.
		//Stream is ready when all header packets are seen.
		// Any data packets the are on the same page as the headers are excess packets. ???? Maybe not any more.
		processHeaderPacket(inPacket);

		if(mNumHeadersNeeded <= 0) {
			//Make sure we create the pin if we got everything already
			AddPin();
			//Add pin will set streamready to true if it was all good.
		}
	} else {
		//Data packets...
		if (mAllowDispatch) {
			if (mFirstRun) {
				mFirstRun = false;

				//Deliver the header data
				deliverCodecHeaders();
			}		
			
			processDataPacket(inPacket);
		} else {
			//processExcessPacket(inPacket);
			return false;
		}
	}
	
	return true;
}
//bool OggStream::processPacket(StampedOggPacket* inPacket) {
	//You always get your own copy of a packet back from this function... you must delete it !!!
	//CAutoLock locLock(mStreamLock);
	//if (inPacket->isComplete()) {
	//	if (mPartialPacket == NULL) {
	//		//CASE 1 : New packet is complete and there is no partial packet.
	//		//===============================================================
	//		
	//		//Just return a *copy* of the incoming packet
	//		return (StampedOggPacket*)inPacket->clone();
	//	} else {
	//		//CASE 2 : New packet is complete and there *is* a partial packet.
	//		//================================================================
	//		//Merge the packets together old one first and return them

	//		//Merge new packet onto old one
	//		mPartialPacket->merge(inPacket);
	//		//Make a copy of the merged packet
	//		StampedOggPacket* locPacket = (StampedOggPacket*)mPartialPacket->clone();
	//		//Delete the old one
	//		delete mPartialPacket;
	//		mPartialPacket = NULL;
	//		//Return the copy of the merged packet
	//		return locPacket;
	//	}
	//} else {
	//	if (mPartialPacket == NULL) {
	//		//CASE 3 : New packet is *incomplete* and there is no partial packet
	//		//==================================================================
	//		//Hold onto a copy of this packet and wait for the rest.

	//		//Take a copy of the packet and hold onto for later.
	//		mPartialPacket = (StampedOggPacket*)inPacket->clone();
	//		return NULL;
	//	} else {
	//		//CASE 4 : New packet is *incomlpete* and there *is* a partial packet
	//		//===================================================================
	//		//Merge the packets old first and wait for the rest

	//		//Merge the packets together old one first
	//		mPartialPacket->merge(inPacket);
	//		
	//		return NULL;

	//		//Heres the bug retard !!
	//		//OggPacket* locPacket = mPartialPacket->clone();
	//		//delete mPartialPacket;
	//		//mPartialPacket = NULL;
	//		//return locPacket;
	//	}
	//}
//}

//ANX::: Need to override here to ensure the anxdata header isn't passed through.
bool OggStream::processHeaderPacket(StampedOggPacket* inPacket) {
	//FIX::: Return values

	//We don't delete the packet... the codecheader list will delete when it's done.
	//StampedOggPacket* locPacket = processPacket(inPacket);
	if (inPacket != NULL) {
		//We got a comlpete packet
		mCodecHeaders->addPacket(inPacket);
		mNumHeadersNeeded--;
	}
	return true;
}

bool OggStream::processDataPacket(StampedOggPacket* inPacket) {
	//We must delete the packet we get back when we have finished
	//StampedOggPacket* locPacket = processPacket(inPacket);
	if (inPacket != NULL) {
		dispatchPacket(inPacket);
		//delete locPacket;
		inPacket = NULL;
	}

	return true;
}

//bool OggStream::processExcessPacket(StampedOggPacket* inPacket) {
//	//FIX::: Return values
//
//	////We don't delete the packet we get back.. the excess list will delete when it's been sent.
//	//OggPacket* locPacket = processPacket(inPacket);
//	//if (locPacket != NULL) {
//	//	//We got a complete packet
//	//	mExcessPackets.push_back(locPacket);
//	//}
//	//New modification to fix the problem where if the last excess packet is an incomplete packet it gets lost as
//	// we don't get back a complete packet... hence we don't store it. This means when we restart we only send the 
//	// full packets we saved, and then the new data. But the start of the new data is not a complete packet.
//	// This is bad !!
//
//	//New solution is to store them as partials and reassemble them whe we send.
//	mExcessPackets.push_back(inPacket);
//	return true;
//}
OggDemuxSourcePin* OggStream::getPin() {
	return mSourcePin;
}


CMediaType* OggStream::createMediaType(GUID inMajorType, GUID inSubType, GUID inFormatType, unsigned long inFormatSize, BYTE* inFormatBlock) {
	AM_MEDIA_TYPE locAMMediaType;
	locAMMediaType.majortype = inMajorType;

	locAMMediaType.subtype = inSubType;
	locAMMediaType.formattype = inFormatType;
	locAMMediaType.cbFormat = inFormatSize; //0;//sizeof(sSpeexFormatBlock);
	locAMMediaType.pbFormat = inFormatBlock; //(BYTE*)locSpeexFormatInfo;
	locAMMediaType.pUnk = NULL;
	
			
	//LEAK CHECK::: Does this get deleted ?
	return new CMediaType(locAMMediaType);
}

unsigned long OggStream::numCodecHeaders() {
	//TODO::: Check for null.
	return mCodecHeaders->numPackets();
}
void OggStream::flush() {
	CAutoLock locLock(mStreamLock);
	//delete mPartialPacket;
	//TODO::: Tell the packetiser to flush.
	//mPartialPacket = NULL;
	mPacketiser.reset();
}

void OggStream::flush(unsigned short inNumPacketsToIgnore) {
	CAutoLock locLock(mStreamLock);
	//delete mPartialPacket;
	//TODO::: Tell the packetiser to flush.
	//mPartialPacket = NULL;
	mPacketiser.reset();
	mPacketiser.setNumIgnorePackets(inNumPacketsToIgnore);
}
//ANX::: Need to override here to create anx pins
bool OggStream::AddPin() {
	createFormatBlock();
	CMediaType* locMediaType = createMediaType(	getMajorTypeGUID(),
												getSubtypeGUID(), 
												getFormatGUID(), 
												getFormatBlockSize(), 
												getFormatBlock());

	//LEAK CHECK::: Where does this get deleted ?
	OggDemuxSourcePin* locSourcePin = new OggDemuxSourcePin(	NAME("Ogg Source Pin"), 
																mOwningFilter, 
																mOwningFilter->m_pLock, 
																mCodecHeaders, 
																locMediaType, 
																getPinName(),
																mAllowSeek);
	mStreamReady = true;
	mSourcePin = locSourcePin;
	
	return true;
	
}

void OggStream::setLastEndGranPos(__int64 inGranPos) {
	//osDebug<<"*************************** ERROR ERROR ERROR **********************"<<endl;
	mLastEndGranulePos = inGranPos;
}
bool OggStream::acceptOggPage(OggPage* inOggPage) {
	//FIX::: Add proper error checking.

	//StampedOggPacket* locPacket = NULL;
	////osDebug<<"New page sets start gran to "<<mLastEndGranulePos<<endl;
	mLastStartGranulePos = mLastEndGranulePos;
	mLastEndGranulePos = inOggPage->header()->GranulePos();

	//Put the page through the packetiser.
	return mPacketiser.acceptOggPage(inOggPage);
	//if (!mStreamReady) {

	//	for (unsigned long i = 0; i < inOggPage->numPackets(); i++) {
	//		locPacket = inOggPage->getStampedPacket(i);
	//		//Add a start stamp.
	//		
	//		if (mNumHeadersNeeded > 0) {
	//			//We are still getting headers
	//			processHeaderPacket(locPacket);
	//		} else {
	//			processExcessPacket(locPacket);
	//		}
	//		locPacket = NULL;
	//	}

	//	if(mNumHeadersNeeded <= 0) {
	//		//Make sure we create the pin if we got everything already
	//		AddPin();
	//	}

	//	//Maybe need a check here to make sure the pin is added
	//} else {
	//	//First time round send the headers
	//	if (mFirstRun) {
	//		mFirstRun = false;

	//		//Deliver the header data
	//		deliverCodecHeaders();
	//	}

	//	//On the first run and after stop/restart resend excess packets.
	//	if (mSendExcess) {
	//		mSendExcess = false;
	//		unsigned long locNumExcess = (unsigned long)mExcessPackets.size();

	//		//for (unsigned long i = 0; i < locNumExcess; i++) {
	//		//	dispatchPacket(mExcessPackets[i]);
	//
	//		//}

	//		//New solution... se Send Exces packets for more info.

	//		for (unsigned long i = 0; i < locNumExcess; i++) {
	//			processDataPacket(mExcessPackets[i]);
	//		}
	//	}

	//	//Process the data packets.
	//	for (unsigned long i = 0; i < inOggPage->numPackets(); i++) {
 //           locPacket = inOggPage->getStampedPacket(i);
	//		//Timestamp hack
	//		locPacket->setStartTime(mLastStartGranulePos);
	//		//
	//		processDataPacket(locPacket);
	//		locPacket = NULL;
	//	}
	//}
	//return true;
}

void OggStream::setSendExcess(bool inSendExcess) {
	mSendExcess = inSendExcess;
}

bool OggStream::deliverCodecHeaders() {
	StampedOggPacket* locPacket = NULL;
	for (unsigned long i = 0; i < mCodecHeaders->numPackets(); i++) {
		locPacket = mCodecHeaders->getPacket(i);

		dispatchPacket(locPacket);
	}
	return true;
}

//ANX::: Maybe also needs override. ??
bool OggStream::dispatchPacket(StampedOggPacket* inPacket) {
	//osDebug<<"Ogg Stream : Packet stamps = "<<inPacket->startTime()<<" - "<<inPacket->endTime()<<endl;
	return mSourcePin->deliverOggPacket(inPacket);
}

