
#ifndef UNIQUE_SERVER_QUEUE_H
#define UNIQUE_SERVER_QUEUE_H
#include "UniqueServer.h"
#include <vector>

using namespace std;

class UniqueServerQueue
{
public:
	UniqueServerQueue():counter(0) { }
	void push_back(UniqueServer us);
	UniqueServer next();
	UniqueServer get(int index);

private:
	vector<UniqueServer> usQueue;
	int counter;
};





#endif 

