#pragma once
#include <vector>
using namespace std;
class OggValidationState
{
public:
	OggValidationState(void);
	~OggValidationState(void);

	OggStreamValidationState* getStream(unsigned long inIndex);
	OggStreamValidationState* getStreamBySerialNo(unsigned long inSerialNo);
	unsigned long numStreams();
	vector<OggStreamValidationState*> mStreams;
protected:

};
