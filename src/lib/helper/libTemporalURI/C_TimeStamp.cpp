#include "StdAfx.h"
#include "C_TimeStamp.h"

C_TimeStamp::C_TimeStamp(void)
	:	mSecs(0)
	,	mHuns(0)
	,	mStampType(TS_NONE)
{
}


C_TimeStamp::~C_TimeStamp(void)
{
}

bool C_TimeStamp::parseNPT(string inTimeStamp, sFourPartTime* inFPT) {

	string locLeftOver = "";

	bool locWasOK = parseThreePartTime(inTimeStamp, inFPT, &locLeftOver);

	if (locWasOK) {
		if ( (locLeftOver.find(".") == 0) || (locLeftOver == "")) {
			if (locLeftOver != "" ) {
				locLeftOver = locLeftOver.substr(1);
			}

			if (locLeftOver == "") {
				inFPT->partials = 0;
				return true;
			} else {
				LOOG_UINT64 locSubSec = StringHelper::stringToFractNum(locLeftOver);
				
				if (locSubSec >= 0) {
					inFPT->partials = locSubSec;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool C_TimeStamp::parseSMPT(string inTimeStamp, sFourPartTime* inFPT) {
	string locLeftOver = "";

	bool locWasOK = parseThreePartTime(inTimeStamp, inFPT, &locLeftOver);

	if (locWasOK) {
		if ( (locLeftOver.find(":") == 0) || (locLeftOver == "")) {
			locLeftOver = locLeftOver.substr(1);
			if (locLeftOver == "") {
				inFPT->partials = 0;
				return true;
			} else {
				long locSubSec = StringHelper::stringToNum(locLeftOver);
				
				if (locSubSec >= 0) {
					//TODO::: Verify frames < numframes for type.
					inFPT->partials = locSubSec;
				}
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool C_TimeStamp::parseThreePartTime(string inTimeStamp, sFourPartTime* inFPT, string* outLeftOver) {
	//This is *H:MM:SS
	size_t locColonPos = inTimeStamp.find(":");
	
	string locHours;
	string locMins;
	string locSecs;

	long locHH = 0;
	long locMM = 0;
	long locSS = 0;

	if (locColonPos != string::npos) {
		locHours = inTimeStamp.substr(0, locColonPos);
	} else {
		return false;
	}

	inTimeStamp = inTimeStamp.substr(locColonPos + 1);
	locColonPos = inTimeStamp.find(":");

	if (locColonPos != string::npos) {
		locMins = inTimeStamp.substr(0, locColonPos);	
	} else {
		return false;
	}

	inTimeStamp = inTimeStamp.substr(locColonPos + 1);
	locColonPos = inTimeStamp.find(":");

	if (locColonPos != string::npos) {
		locSecs = inTimeStamp.substr(0, locColonPos);	
	} else {
		locColonPos = inTimeStamp.find(".");

		if (locColonPos != string::npos) {
			locSecs = inTimeStamp.substr(0, locColonPos);
		} else {
			locSecs = inTimeStamp.substr(0);
		}
	}

	if (locColonPos == string::npos) {
		inTimeStamp = "";
	} else {
		inTimeStamp = inTimeStamp.substr(locColonPos);
	}

	locHH = StringHelper::stringToNum(locHours);
	locMM = StringHelper::stringToNum(locMins);
	locSS = StringHelper::stringToNum(locSecs);

	if (		(locHH >= 0) && (locMM >= 0) && (locSS >= 0) && (locMM <= 59) && (locSS <=59) ) {
		//The upper bound on hh is not checked here... it's upto the smpt time parser to ensure it <= 99

		//Other wise, everything is ok.
		inFPT->hours = locHH;
		inFPT->minutes = locMM;
		inFPT->seconds = locSS;
		inFPT->partials = 0;
		*outLeftOver = inTimeStamp;
		return true;

	} else {
		return false;
	}


	

}

bool C_TimeStamp::parseSecsOnly(string inTimeStamp) {
	
	string locSecs;
	string locHuns;
	LOOG_UINT64 locSS;
	LOOG_UINT64 locNN;

	size_t locDotPos = inTimeStamp.find(".");

	if (locDotPos == string::npos) {
		//No dot here
		locSS = StringHelper::stringToNum(inTimeStamp);
		locNN = 0;
	} else {
		//Dotted time

		locSecs = inTimeStamp.substr(0, locDotPos);
		locHuns = inTimeStamp.substr(locDotPos + 1);

		locSS = StringHelper::stringToNum(locSecs);

		if (locHuns == "") {
			locNN = 0;
		} else {
			locNN = StringHelper::stringToFractNum(locHuns);
		}

		if (	(locSS >= 0) && (locNN >= 0) && (locSS <= 59) ) {
			mSecs = locSS;
			mHuns = locNN;
			mStampType = TS_NPT_SECS;
			return true;
		} else {
			return false;
		}
	}
}

LOOG_INT64 C_TimeStamp::toHunNanos() {
	switch (mStampType) {
		case TS_NPT_SECS:
			return (mSecs * 10000000) + (mHuns);
		case TS_NPT_FULL:
			return (mFPT.hours * 3600 * 10000000) + (mFPT.minutes * 60 * 10000000) + (mFPT.seconds * 10000000) + (mFPT.partials);
		default:
			return  -1;

	};
}

bool C_TimeStamp::parseTimeStamp(string inTimeStamp)
{
	if (inTimeStamp.find("npt:") == 0) {
		//NPT time stamp
		inTimeStamp = inTimeStamp.substr(4);

		if (inTimeStamp.find(":") != string::npos) {
			//We have four part time
			sFourPartTime locFPT;
			if ( parseNPT(inTimeStamp, &locFPT) ) {
				mFPT = locFPT;
				mStampType = TS_NPT_FULL;
				return true;
			} else {
				return false;
			}
			
		} else {
			return parseSecsOnly(inTimeStamp);
		}

	} else if (inTimeStamp.find("smpte-") == 0) {
		//One of the smpt stamps
		inTimeStamp = inTimeStamp.substr(6);

		size_t locColonPos = inTimeStamp.find(":");

		if (locColonPos != string::npos) {
			string locFrameRate = inTimeStamp.substr(0, locColonPos);

			//TODO::: Need to set the spec type here...
			inTimeStamp = inTimeStamp.substr(locColonPos + 1);

			sFourPartTime locFPT;
			if ( parseSMPT(inTimeStamp, &locFPT) ) {
				mFPT = locFPT;
				mStampType = TS_SMPT;
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}



	} else {
		//Assume it's default numeric npt
		return parseSecsOnly(inTimeStamp);



	}

}