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
#include "oggpacket.h"

OggPacket::OggPacket(void)
	:	mPacketSize(0),
		mPacketData(NULL),
		mIsComplete(false)
		
{

}

OggPacket::OggPacket(unsigned char* inPackData, unsigned long inPacketSize, bool inIsComplete)
	:	mPacketSize(inPacketSize),
		mPacketData(inPackData),
		mIsComplete(inIsComplete)
		
{
}
OggPacket* OggPacket::clone() {
	//Make a new buffer for packet data
	unsigned char* locBuff = new unsigned char[mPacketSize];

	//Copy the packet data into the new buffer
	memcpy((void*)locBuff, (const void*)mPacketData, mPacketSize);

	//Create the new packet
	OggPacket* retPack = new OggPacket(locBuff, mPacketSize, mIsComplete);
	return retPack;
}


OggPacket::~OggPacket(void)
{
	//Now deletes the packetData
	delete mPacketData;

	//Should be careful about allowing this to be copied...
}

string OggPacket::toPackDumpString() {
	string retStr = "";


	///NOTE::: ShOuld be reworked.
	//Needs dataSize and data pointer

	//Put the stream in hex mode with a fill character of 0
	
	//cout << setfill('0');

	//Loop through every character of data
	for (unsigned long i = 0; i < mPacketSize; i++) {
		//If it is the end of the previous hex dump line or first line)
		if ( (i % HEX_DUMP_LINE_LENGTH == 0) ) {
			//And this is not the first line
			if ( i != 0 ) {
				//Put the characters on the end
				retStr.append(dumpNCharsToString( &mPacketData[i - HEX_DUMP_LINE_LENGTH],  HEX_DUMP_LINE_LENGTH));
			}

			//At the start of the line write out the base address in an 8 hex-digit field 
			//NOTE::: Just decimal for now.
			//cout << setw(8) << i << ": ";
			retStr.append(padField(StringHelper::numToString(i), 8, '0') + ": ");
		}

		//Write out the value of the character in a 2 hex-digit field
		//cout << setw(2) << (int)mPageData[i] << " ";
		retStr.append(StringHelper::charToHexString(mPacketData[i]) + " ");
	}

	//Find out how many leftover charcters didn't get written out.
	unsigned long locLeftovers = (mPacketSize % HEX_DUMP_LINE_LENGTH);

	locLeftovers = (locLeftovers > 0)	? (locLeftovers)	
										: (HEX_DUMP_LINE_LENGTH);


	//If there was any data in this dump
	if ( mPacketSize > 0 ) {
		//Dump the last part out
		retStr.append(dumpNCharsToString( &mPacketData[mPacketSize - locLeftovers], locLeftovers ));
	}

	retStr+= "==============================================================================\n" ;
	//Put the stream back to decimal mode
	//dec(cout);

	return retStr;
}

string OggPacket::padField(string inString, unsigned long inPadWidth, unsigned char inPadChar) {
	//NOTE::: Need check for string being  bigger than pad space
	string retStr = "";
	retStr.append(inPadWidth - inString.length(), inPadChar);
	retStr.append(inString);

	return retStr;
}
string OggPacket::dumpNCharsToString(unsigned char* inStartPoint, unsigned long inNumChars) {
	//NOTE::: Also needs reworking
	const unsigned char BELL = 7;
	//Set the fill character back to space ' '
	//cout << setfill(' ');


	//Put some space after the hex section
	unsigned long locPadding = 3 * (HEX_DUMP_LINE_LENGTH - inNumChars) + 4;
	//cout << setw(locPadding) << "    ";
	
	string retStr = padField("    ", locPadding, ' ');   
	
	//Loop through the characters
	for (unsigned long i = 0; i < inNumChars; i++) {

		//If they are *not* going to mess up the layout (\r, \n or \t or bell(7))
		if ( (inStartPoint[i] != '\n') && (inStartPoint[i] != '\r') && (inStartPoint[i] != '\t') && (inStartPoint[i] != BELL )) {
			//Write them out
			retStr += (char)inStartPoint[i];						
		} else {
			//Otherwise just write a null char
			retStr += ((char) 0);
		}
	}
	retStr += "\n";
	
	return retStr;
	
}



unsigned long OggPacket::packetSize() const {
	return mPacketSize;
}

unsigned char* OggPacket::packetData() {
	return mPacketData;
}


bool OggPacket::isComplete() const {
	return mIsComplete;
}

void OggPacket::setIsComplete(bool inIsComplete) {
	mIsComplete = inIsComplete;
}
void OggPacket::setPacketSize(unsigned long inPacketSize) {
	mPacketSize = inPacketSize;
}

void OggPacket::setPacketData(unsigned char* inPacketData) {
	mPacketData = inPacketData;
}


void OggPacket::merge(OggPacket* inMorePacket) {
	//Make a new buffer the size of both data segs together
	unsigned char* locBuff = new unsigned char[mPacketSize + inMorePacket->mPacketSize];
	//Copy this packets data to the start
	memcpy((void*)locBuff, (const void*)mPacketData, mPacketSize);
	//Copy the next packets data after it
	memcpy((void*)(locBuff + mPacketSize), (const void*)inMorePacket->mPacketData, inMorePacket->mPacketSize);
	//Delete our original packet data
	delete mPacketData;
	//Now make our data be the combined data
	mPacketData = locBuff;
	//Make the size the sum of both packets
	mPacketSize += inMorePacket->mPacketSize;

	//If the next part of the packet isn't complete then this packet is not complete.
	mIsComplete = inMorePacket->mIsComplete;
	//Should we do this ????
	//NO !!! It will munt the page it is contained in !
	//delete inMorePacket;

}