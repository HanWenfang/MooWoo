
#ifndef TREACTOR_MUMAN_H
#define TREACTOR_MUMAN_H
#include <arpa/inet.h>
#include <list>
#include <vector>
#include <iostream>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Mutex.h>
#include <Poco/NotificationQueue.h>
#include <Poco/NumberParser.h>
#include "Protocol.h"
#include "MooWoo.h"
#include "SocketEvent.h"


using namespace std;

class ComputeCore;

class TReactor: public Poco::Runnable
{
private:
	bool isAlive;
	bool isClosed;

	vector<Message> inbox;
	vector<Message> outbox;
	ComputeCore *computeCore;
	list<int> descriptors;
	Poco::Mutex mutex;
	Poco::NotificationQueue &event_queue;
	int id;

public:
	TReactor(ComputeCore *computeC, Poco::NotificationQueue &equeue, int identity):\
	isAlive(true),isClosed(false),computeCore(computeC), event_queue(equeue), id(identity)
	{
		
	}

	//atomic operation
	int addDescriptor(int sock);
	void removeDescriptor(int sock);
	unsigned int copyDescriptorSet(list<int> &dset);
	int max(list<int> &dset);

	int start();
	virtual void run();
};











#endif
