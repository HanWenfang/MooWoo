
#include "TReactor.h"
#include "ComputeCore.h"

int TReactor::addDescriptor(int sock)
{
	if(sock < 0) return -1;

	Poco::Mutex::ScopedLock lock(mutex);

	if(descriptors.size() >= FD_SETSIZE) return -1;

	descriptors.push_front(sock);
	return 0;
}

void TReactor::removeDescriptor(int sock)
{
	Poco::Mutex::ScopedLock lock(mutex);
	list<int>::iterator it=descriptors.begin();

	for(; it != descriptors.end(); ++it)
	{
		if(*it == sock) break;
	}

	if(it != descriptors.end()) descriptors.erase(it);
}

unsigned int TReactor::copyDescriptorSet(list<int> &dset)
{
	Poco::Mutex::ScopedLock lock(mutex);
	unsigned int counter = 0;
	
	for(list<int>::iterator it=descriptors.begin(); it != descriptors.end(); ++it)
	{
		dset.push_front(*it);
		++counter;
	}

	return counter;
}

int TReactor::max(list<int> &dset)
{
	int maxA = -1;
	for(list<int>::iterator it=dset.begin(); it != dset.end(); ++it)
	{
		if (*it > maxA) maxA = *it;
	}
	return maxA;
}

int TReactor::start()
{
	fd_set file_descriptors;
	fd_set *file_descriptors_ptr = NULL;
	struct timeval time_value;

	list<int> descriptorSet;
	int maxSock;
	int receiveResult;
	int notifyTimeout = Poco::NumberParser::parse(Config::instance().get("NotifyTimeout"));
	Poco::AutoPtr<Poco::Notification> pN = NULL;

	while(isAlive)
	{
		pN = event_queue.waitDequeueNotification(notifyTimeout); // timeout
		if(pN)
		{
			SocketEvent *pHE = dynamic_cast<SocketEvent *>(pN.get());
			addDescriptor(pHE->socket);
			pN = NULL;
		}

		// Timer or IO multiplex
		if(copyDescriptorSet(descriptorSet) == 0) file_descriptors_ptr = NULL; //timer
		else file_descriptors_ptr = &file_descriptors; //timeout

		maxSock = max(descriptorSet);

		FD_ZERO(&file_descriptors);
		
		for(list<int>::iterator it=descriptorSet.begin(); it != descriptorSet.end(); ++it)
		{
			FD_SET(*it, &file_descriptors);
		}

		time_value.tv_sec = Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_ras"));
		time_value.tv_usec = Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_rams"));

		if (::select(maxSock+1, file_descriptors_ptr, NULL, NULL, &time_value) > 0)
		{
			//receiveMessage
			for(list<int>::iterator it=descriptorSet.begin(); it != descriptorSet.end(); ++it)
			{
				if(FD_ISSET(*it, file_descriptors_ptr))
				{
					receiveResult = Protocol::receiveMessage(*it, inbox);

					if ( receiveResult == 0)
					{
						close(*it);
						removeDescriptor(*it);
					}

					if( receiveResult < 0)
					{
						// communication false operation, maybe a error message queue
					}
				}
			}
		}
		cout << "inbox: " << inbox.size() << endl;
		//processData
		for(vector<Message>::iterator it=inbox.begin(); it != inbox.end(); ++it)
		{
			cout << "Rank: " << computeCore->getRank() << endl;
			computeCore->getMessageHandler(it->getMessageTag())->callback(*it);
		}
		//processMessage
		if(!inbox.empty()) inbox.clear();

		//sendMessage [use timeout to solve communication faulse]
		if(!outbox.empty()) Protocol::sendMessage(outbox);
	}

	return 0;
}

void TReactor::run()
{
	cout << "TReactor: " << id << endl; 
	start();
}

