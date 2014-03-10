

#ifndef ASYNCORE_MUMAN_H
#define ASYNCORE_MUMAN_H
#include "UniqueServerQueue.h"
#include "Message.h"
#include "MooWoo.h"
#include "SocketEvent.h"
#include <Poco/NumberParser.h>
#include <Poco/NotificationQueue.h>
#include <vector>
#include <arpa/inet.h>
#include <iostream>


using namespace std;

class AsynCore
{
private:
	int current_socket;
	int rank;
	vector<UniqueServerQueue> ranks;
	fd_set file_descriptors;
	struct timeval time_value;
	Poco::NotificationQueue &event_queue;

public:
	AsynCore(Poco::NotificationQueue &equeue):event_queue(equeue){}
	int initialize(int rk, vector<UniqueServerQueue> &rank_set);
	void setRanks(int rk, vector<UniqueServerQueue> &rank_set);
	int spawnSocket();
	int configSocket(int option); // redis
	void socketAddress(struct sockaddr_in &sockAddr, in_port_t serverPort, in_addr_t serverIp);
	int socketRankBind(struct sockaddr_in *socket_addr);
	int setListen(int num);
	void acceptSocket();
	int select();
};






#endif


