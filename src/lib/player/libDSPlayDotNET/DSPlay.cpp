#include "StdAfx.h"
#include "dsplay.h"

#pragma managed
namespace illiminable {
namespace libDSPlayDotNET {
	//class __gc DSPlayer {
	//	public:
	//		bool loadFile(String inFileName);
	//		bool play();
	//		bool pause();
	//		bool stop();
	//		bool seek(Int64 inTime);
	//		Int64 queryPosition();
	//};

DSPlay::DSPlay(void) 
	:	mGraphBuilder(NULL)
	,	mMediaControl(NULL)
	,	mMediaSeeking(NULL)
	,	mMediaEvent(NULL)
	,	mEventHandle(INVALID_HANDLE_VALUE)
	,	mDNCMMLCallbacks(NULL)
	,	mDNMediaEvent(NULL)
{
	CoInitialize(NULL);
}

bool DSPlay::checkEvents() {
	const DWORD TIMEOUT_WAIT = 0;  //Wait this many ms for handle
	long locEventCode = 0;
	long locParam1 = 0;
	long locParam2 = 0;
	HRESULT locHR = S_OK;

	if (WAIT_OBJECT_0 == WaitForSingleObject(mEventHandle, TIMEOUT_WAIT))   { 
			while (locHR = mMediaEvent->GetEvent(&locEventCode, &locParam1, &locParam2, 0), SUCCEEDED(locHR)) 
			{
	            
				//cout<<"Event : "<<evCode<<" Params : "<<param1<<", "<<param2<<endl;
				
				//This is dodgy ! param1 and 2 are actually pointers !!
				if (mDNMediaEvent != NULL) {
					mDNMediaEvent->eventNotification(locEventCode, locParam1, locParam2);
				}

				mMediaEvent->FreeEventParams(locEventCode, locParam1, locParam2);
			}
	}
	return true;
}
//IMediaEvent* locMediaEvent = NULL;
//	locHR = locGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&locMediaEvent);
//	
//	HANDLE  hEvent; 
//	long    evCode, param1, param2;
//	BOOLEAN bDone = FALSE;
//	HRESULT hr = S_OK;
//	hr = locMediaEvent->GetEventHandle((OAEVENT*)&hEvent);
//	if (FAILED(hr))
//	{
//	    /* Insert failure-handling code here. */
//	}
//	while(true) //!bDone) 
//	{
//	    if (WAIT_OBJECT_0 == WaitForSingleObject(hEvent, 100))
//	    { 
//			while (hr = locMediaEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr)) 
//			{
//	            //printf("Event code: %#04x\n Params: %d, %d\n", evCode, param1, param2);
//				cout<<"Event : "<<evCode<<" Params : "<<param1<<", "<<param2<<endl;
//				locMediaEvent->FreeEventParams(evCode, param1, param2);
//				bDone = (EC_COMPLETE == evCode);
//			}
//		}
//	} 

DSPlay::~DSPlay(void) {
	releaseInterfaces();
	CoUninitialize();
}

void DSPlay::releaseInterfaces() {
	if (mGraphBuilder != NULL) {
		mGraphBuilder->Release();
		mGraphBuilder = NULL;
	}

	if (mMediaControl != NULL) {
		mMediaControl->Release();
		mMediaControl = NULL;
	}

	if (mMediaSeeking != NULL) {
		mMediaSeeking->Release();
		mMediaSeeking = NULL;
	}

	if (mMediaEvent != NULL) {
		mMediaEvent->Release();
		mMediaEvent = NULL;
	}

	//TODO::: Release everything !
}

bool DSPlay::loadFile(String* inFileName) {

	
	releaseInterfaces();
	HRESULT locHR = S_OK;

	char* locFileName = Wrappers::netStrToCStr(inFileName);
	wstring locWFileName = illiminable::libDSPlayDotNET::toWStr(locFileName);
	
	Wrappers::releaseCStr(locFileName);
	locFileName = NULL;

	//Have to use a local pointer or taking the adress of a member function makes the second level
	// of indirection a __gc pointer.
	IGraphBuilder* locGraphBuilder = NULL;
	locHR = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&locGraphBuilder);
	mGraphBuilder = locGraphBuilder;
	if (locHR != S_OK) {
		mIsLoaded = false;
		return false;
	}
	
	
	//If it's an annodex file, then put the VMR 9 in the graph.
	if (isFileAnnodex(inFileName)) {
	
		IBaseFilter* locVMR9 = NULL;

		HRESULT locHR = S_OK;
		locHR = mGraphBuilder->FindFilterByName(L"Video Mixing Renderer 9", &locVMR9);
		if (locVMR9 == NULL) {
			locHR= CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **)&locVMR9);
			if (locHR == S_OK) {
				locHR = mGraphBuilder->AddFilter(locVMR9, L"Video Mixing Renderer 9");
				if (locHR != S_OK) {
					locVMR9->Release();
				}
			}
		}

	}

	//Build the graph
	locHR = mGraphBuilder->RenderFile(locWFileName.c_str(), NULL);

	if (locHR != S_OK) {
		mIsLoaded = false;
		return false;
	}

	//Get the media control interface
	IMediaControl* locMediaControl = NULL;
	locHR = mGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&locMediaControl);
	mMediaControl = locMediaControl;

	if (locHR != S_OK) {
		mIsLoaded = false;
		return false;
	} else {
		mIsLoaded = true;
	}

	//get the media seeking interface if its available.
	IMediaSeeking* locMediaSeeking = NULL;
	locHR = mGraphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&locMediaSeeking);
	mMediaSeeking = locMediaSeeking;

	//Get the media event interface
	IMediaEvent* locMediaEvent = NULL;
	locHR = locGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&locMediaEvent);

	if (locHR == S_OK) {
		mMediaEvent = locMediaEvent;
		HANDLE locEventHandle = INVALID_HANDLE_VALUE;
		locHR = locMediaEvent->GetEventHandle((OAEVENT*)&locEventHandle);
		mEventHandle = locEventHandle;
	}

//	if (FAILED(hr))

	return true;


}

bool DSPlay::isLoaded() {
	return mIsLoaded;
}
bool DSPlay::play() {
	if (mIsLoaded) {
		HRESULT locHR = mMediaControl->Run();
		if (locHR != S_OK) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}

bool DSPlay::pause() {
	if (mIsLoaded) {
		HRESULT locHR = mMediaControl->Pause();
		if (locHR != S_OK) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}

bool DSPlay::stop() {
	if (mIsLoaded) {
		HRESULT locHR = mMediaControl->Stop();
		if (locHR != S_OK) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}

Int64 DSPlay::seek(Int64 inTime) {
	if (mIsLoaded && (mMediaSeeking != NULL)) {
		LONGLONG locCurrent = inTime;
		LONGLONG locStop = 0;
		HRESULT locHR = mMediaSeeking->SetPositions(&locCurrent, AM_SEEKING_AbsolutePositioning | AM_SEEKING_ReturnTime, 
													&locStop, AM_SEEKING_NoPositioning);
		
		if (SUCCEEDED(locHR)) {
			return locCurrent;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
	
}

Int64 DSPlay::seekStart() {
	return 0;
}

Int64 DSPlay::queryPosition() {
	return 0;
}

Int64 DSPlay::fileSize() {
	return -1;
}
Int64 DSPlay::fileDuration() {
	if (mIsLoaded && (mMediaSeeking != NULL)) {
		LONGLONG locDuration = 0;
		HRESULT locHR = mMediaSeeking->GetDuration(&locDuration);

		if (locHR != S_OK) {
			return -1;
		} else {
			return locDuration;
		}
	} else {
		return -1;
	}
}

bool DSPlay::isFileAnnodex(String* inFilename)
{
	String* locExt = (inFilename->Substring(inFilename->Length - 4, 4))->ToUpper();
	if (locExt->Equals(".ANX")) {
		return true;
	} else {
		return false;
	}
}

bool DSPlay::setMediaEventCallback(IDNMediaEvent* inMediaEventCallback) {
	mDNMediaEvent = inMediaEventCallback;
	return true;
}
IDNMediaEvent* DSPlay::getMediaEventCallback() {
	return mDNMediaEvent;
}


} //end namespace libDSPlayDotNET
} //end namespace illiminable
