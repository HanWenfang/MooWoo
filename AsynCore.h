

#ifndef ASYNCORE_MUMAN_H
#define ASYNCORE_MUMAN_H
#include "UniqueServer.h"
#include "Message.h"
#include "MooWoo.h"
#include <Poco/NumberParser.h>
#include <vector>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

class AsynCore
{
private:
	int current_socket;
	int rank;
	vector<UniqueServer> ranks;
	fd_set file_descriptors;
	struct timeval time_value;
public:
	AsynCore(){}
	int initialize(int rk, vector<UniqueServer> &rank_set);
	void setRanks(int rk, vector<UniqueServer> &rank_set);
	int spawnSocket();
	int configSocket(int option); // redis
	void socketAddress(struct sockaddr_in &sockAddr, in_port_t serverPort, in_addr_t serverIp);
	int socketRankBind(struct sockaddr_in *socket_addr);
	int setListen(int num);
	int acceptSocket();
	int select();
};






#endif


