#pragma once

class OggStreamValidationState
{
public:
	OggStreamValidationState(void);
	~OggStreamValidationState(void);

	enum eOggStreamValidationState {
		VS_FULLY_VALID,

		VS_SEEN_NOTHING,
		VS_SEEN_BOS,
		VS_SEEN_EOS,
		VS_WAITING_FOR_CONTINUATION,
		VS_INVALID

	};
	unsigned long mSerialNo;
	__int64 mGranulePosUpto;
	unsigned long mSequenceNoUpto;

	bool mSeenAnything;
	unsigned long mSeenBOS;
	unsigned long mSeenEOS;

	unsigned long mErrorCount;
	unsigned long mWarningCount;

	eOggStreamValidationState mState;

};
