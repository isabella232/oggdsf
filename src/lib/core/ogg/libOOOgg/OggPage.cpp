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

#include "stdafx.h"
#include "OggPage.h"
//LEACK CHECKED::: 20041018 - OK.
OggPage::OggPage(void)
{
	mHeader = new OggPageHeader;			//Deleted in destructor.
	
	
}
OggPage::~OggPage(void)
{
	delete mHeader;
	for (unsigned long i = 0; i < mPacketList.size(); i++) {
		delete mPacketList[i];
	}
}

//Retusn a view on the header only
OggPageHeader* OggPage::header() {
	return mHeader;
}

//returns a view on the packet only.
OggPacket* OggPage::getPacket(unsigned long inPacketNo) {
	if (inPacketNo < numPackets()) {
		return mPacketList[inPacketNo];
	} else {
		return NULL;
	}
}
//Returns a view on the packet only
StampedOggPacket* OggPage::getStampedPacket(unsigned long inPacketNo) {
	if (inPacketNo < numPackets()) {
		return mPacketList[inPacketNo];
	} else {
		return NULL;
	}
}

//Gives away  a pointer to the caller.
OggPage* OggPage::clone() {
	OggPage* retClone = new OggPage;
	retClone->mHeader = mHeader->clone();
	for (size_t i = 0; i < mPacketList.size(); i++) {
		retClone->mPacketList.push_back((StampedOggPacket*)mPacketList[i]->clone());		//The cloned new packet is deleted in the destructor
	}
	
	return retClone;
}

unsigned long OggPage::numPackets() {
	return (unsigned long)mPacketList.size();
}

//Gives a pointer to the caller, it must delete it.
unsigned char* OggPage::createRawPageData() {
	unsigned char* locPage = new unsigned char[mHeader->pageSize()];		//Given to the caller
	mHeader->rawData(locPage, mHeader->pageSize());

	unsigned long locOffset = mHeader->headerSize();
	for (size_t i = 0; i < mPacketList.size(); i++) {
		OggPacket* locPack = mPacketList[i];
		memcpy((void*)(locPage + locOffset), (const void*)(locPack->packetData()), locPack->packetSize());
		locOffset += locPack->packetSize();
	}
	return locPage;
}

bool OggPage::addPacket(StampedOggPacket* inPacket) {
	if (inPacket != NULL) {
		mPacketList.push_back(inPacket);
		return true;
	} else {
		return false;
	}
}



unsigned long OggPage::pageSize()
{
	return mHeader->pageSize();
}
unsigned long OggPage::headerSize()
{
	return mHeader->headerSize();
}
unsigned long OggPage::dataSize()
{
	return mHeader->dataSize();
}
