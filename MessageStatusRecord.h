
#ifndef MESSAGE_STATUS_RECORD_MUMAN
#define MESSAGE_STATUS_RECORD_MUMAN
#include <Poco/Mutex.h>

class MessageStatusRecord
{
private:
	map<int , int> record;
	Poco::Mutex mutex;

public:
	
};


#endif

