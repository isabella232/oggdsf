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
#include "httpfilesource.h"

HTTPFileSource::HTTPFileSource(void)
	:	mWasError(false)
	,	mIsEOF(false)
	,	mIsOpen(false)
	,	mSeenResponse(false)
{
	debugLog.open("G:\\httpdebug.log", ios_base::out);
	WORD locWinsockVersion = MAKEWORD(1,1);
	WSADATA locWinsockData;
	int locRet= 0;

	locRet = WSAStartup(locWinsockVersion, &locWinsockData);
	if ((locRet != 0) || (locWinsockData.wVersion != locWinsockVersion)) {
		//Failed to setup.
		debugLog<<"Failed to start winsock V "<<locWinsockData.wVersion<<endl;
		WSACleanup();
		throw 0;
	}

	debugLog<<"Winsock started"<<endl;



}

HTTPFileSource::~HTTPFileSource(void)
{
	debugLog<<"Winsock ended"<<endl;
	WSACleanup();
}

void HTTPFileSource::DataProcessLoop() {
	debugLog<<"DataProcessLoop: "<<endl;
	int locNumRead = 0;
	char* locBuff = NULL;
	const unsigned long RECV_BUFF_SIZE = 1024;
	locBuff = new char[RECV_BUFF_SIZE];
	while(true) {

		locNumRead = recv(mSocket, locBuff, RECV_BUFF_SIZE, 0);
		if (locNumRead == SOCKET_ERROR) {
			int locErr = WSAGetLastError();
			debugLog<<"Socket error receiving - Err No = "<<locErr<<endl;
			mWasError = true;
			break;
		}

		if (locNumRead == 0) {
			debugLog<<"Read last bytes..."<<endl;
			mIsEOF = true;
			break;
		}

		if (mSeenResponse) {
			//Add to buffer
			mStreamBuffer.write(locBuff, locNumRead);
			debugLog<<"Added to buffer "<<locNumRead<<" bytes."<<endl;
		} else {
			string locTemp = locBuff;
			size_t locPos = locTemp.find("\n\n");
			if (locPos != string::npos) {
				//Found the break
				mSeenResponse = true;
				mLastResponse = locTemp.substr(0, locPos);
				mStreamBuffer.write(locBuff + locPos + 2, locNumRead - (locPos + 2));
				debugLog<<"Added to Buffer "<<locNumRead - (locPos+2)<<" bytes... first after response."<<endl;
			}
		}
	}

	delete locBuff;


}

bool HTTPFileSource::setupSocket(string inSourceLocation) {
	
	debugLog<<"Setup Socket:"<<endl;
	IN_ADDR locAddress;  //iaHost
	LPHOSTENT locHostData;;  //lpHost

	bool locValidURL = splitURL(inSourceLocation);

	locAddress.S_un.S_addr = inet_addr(mServerName.c_str());
	

	if (locAddress.S_un.S_addr == INADDR_NONE) {
		locHostData = gethostbyname(mServerName.c_str());
	} else {
		locHostData = gethostbyaddr((const char*)&locAddress, sizeof(struct in_addr), AF_INET);
	}



	if (locHostData == NULL) {
		debugLog<<"LocHostData is NULL"<<endl;
		//Failed
		return false;
	}

	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET) {
		debugLog<<"Socket Invalid"<<endl;
		//Failed
		return false;
	}


	LPSERVENT locServiceData; //lpServEnt
	SOCKADDR_IN locServiceSocketAddr; //saServer

	locServiceData = getservbyname("http", "tcp");
	if (locServiceData == NULL) {
		locServiceSocketAddr.sin_port = htons(80);
	} else {
		locServiceSocketAddr.sin_port = locServiceData->s_port;
	}



	locServiceSocketAddr.sin_family = AF_INET;
	locServiceSocketAddr.sin_addr = *((LPIN_ADDR)*locHostData->h_addr_list);


	int locRetVal = 0;
	locRetVal = connect(mSocket, (LPSOCKADDR)&locServiceSocketAddr, sizeof(SOCKADDR_IN));
	if (locRetVal == SOCKET_ERROR) {
		debugLog<<"Failed to connect..."<<endl;
		closesocket(mSocket);
		return false;
	}

	return true;


}

string HTTPFileSource::assembleRequest(string inFilePath) {
	string retRequest;
	retRequest = "GET " + inFilePath+ " HTTP/1.1\n" + "Host: " + mServerName+ "\n\n";
	debugLog<<"Assembled Req : "<<endl<<retRequest<<endl;
	return retRequest;
}

bool HTTPFileSource::httpRequest(string inRequest) {
	debugLog<<"Http Request:"<<endl;
	int locRetVal = send(mSocket, inRequest.c_str(), inRequest.length(), 0);

	if (locRetVal == SOCKET_ERROR) {
		debugLog<<"Socket error on send"<<endl;
		closesocket(mSocket);
		return false;
	}
	return true;
}
DWORD HTTPFileSource::ThreadProc(void) {
	debugLog<<"ThreadProc:"<<endl;
	while(true) {
		DWORD locThreadCommand = GetRequest();
		
		switch(locThreadCommand) {
			case THREAD_EXIT:
				
				Reply(S_OK);
				return S_OK;



			case THREAD_RUN:
				
				Reply(S_OK);
				DataProcessLoop();
				break;
		}
	
	
	}
	return S_OK;
}
unsigned long HTTPFileSource::seek(unsigned long inPos) {
	//Close the socket down
	//Open up a new one to the same place.
	//Make the partial content request.
	debugLog<<"Seek ::::: EOROR NOT IMPL"<<endl;
	return 0;
}

bool HTTPFileSource::splitURL(string inURL) {
	debugLog<<"Split url:"<<endl;
	string locProtocol;
	string locServerName;
	string locPath;
	string locTemp;
	size_t locPos2;
	size_t locPos = inURL.find(':');
	if (locPos == string::npos) {
		//No colon... not a url or file... failure.
		return false;
	} else {
		locProtocol = inURL.substr(0, locPos);
		locTemp = inURL.substr(locPos+1);
		locPos = locTemp.find("//");
		if ((locPos == string::npos) || (locPos != 0)) {
			return false;
		} else {
            locTemp = locTemp.substr(locPos+2);
			locPos = locTemp.find('/');
			if (locPos == string::npos) {
				return false;
			} else {
				locServerName = locTemp.substr(0, locPos);
				locPath = locTemp.substr(locPos);
			}
		}
		
	}

	mServerName = locServerName;
	mFileName = locPath;
	debugLog<<"Proto : "<<locProtocol<<endl<<"Server : "<<locServerName<<endl<<"Path : "<<mFileName<<endl;
	return true;

}
void HTTPFileSource::closeSocket() {
	debugLog<<"Close Socket:"<<endl;
	closesocket(mSocket);
}
void HTTPFileSource::close() {
	//Close the socket down.
	closeSocket();
}

bool HTTPFileSource::startThread() {
	if (ThreadExists() == FALSE) {
		Create();
	}
	CallWorker(THREAD_RUN);
	return true;
}
bool HTTPFileSource::open(string inSourceLocation) {
	//Open network connection and start feeding data into a buffer
	//
	mSeenResponse = false;
	mLastResponse = "";
	debugLog<<"Open:"<<endl;
	bool locIsOK = setupSocket(inSourceLocation);

	if (!locIsOK) {
		debugLog<<"Setup socket FAILED"<<endl;
		closeSocket();
		return false;
	}

	debugLog<<"Sending request..."<<endl;
	httpRequest(assembleRequest(mFileName));
	debugLog<<"Socket ok... starting thread"<<endl;
	locIsOK = startThread();


	return locIsOK;
}
void HTTPFileSource::clear() {
	//Reset flags.
	mIsEOF = false;
	mWasError = false;
}
bool HTTPFileSource::isEOF() {
	
	if ((mStreamBuffer.tellp() - mStreamBuffer.tellg() == 0) && mIsEOF) {
		debugLog<<"It is EOF"<<endl;
		return true;
	} else {
		debugLog<<"It's not EOF"<<endl;
		return false;
	}
}
unsigned long HTTPFileSource::read(char* outBuffer, unsigned long inNumBytes) {
	//Reads from the buffer, will return 0 if nothing in buffer.
	// If it returns 0 check the isEOF flag to see if it was the end of file or the network is just slow.
	debugLog<<"Read:"<<endl;
	if(isEOF() || mWasError) {
		debugLog<<"Can't read is error or eof"<<endl;
		return 0;
	} else {
		debugLog<<"Reading from buffer"<<endl;
		mStreamBuffer.read(outBuffer, inNumBytes);
		unsigned long locNumRead = mStreamBuffer.gcount();
		debugLog<<locNumRead<<" bytes read from buffer"<<endl;
		return locNumRead;
	}

}
