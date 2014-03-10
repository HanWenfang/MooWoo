
#include "UniqueServerQueue.h"


void UniqueServerQueue::push_back(UniqueServer us)
{
	usQueue.push_back(us);
}

UniqueServer UniqueServerQueue::next()
{
	UniqueServer tmp = usQueue[counter];
	++counter;
	if(counter >= usQueue.size())
	{
		counter = 0;
	}

	return tmp;
}

UniqueServer UniqueServerQueue::get(int index)
{
	return usQueue[index];
}

