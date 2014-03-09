
#include "AsynCore.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define IP_PORT_SIZE sizeof(struct sockaddr_in)

// AF_INET(IPv4 Address Format) SOCK_STREAM(TCP) IPPROTO_TCP(TCP)
int AsynCore::spawnSocket()
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	current_socket = fd;
	return fd;
}

int AsynCore::configSocket(int option)
{
	int on = 1;
	return setsockopt(current_socket, SOL_SOCKET, option, &on, sizeof(on));
}

void AsynCore::socketAddress(struct sockaddr_in &sockAddr, in_port_t serverPort, in_addr_t serverIp)
{
	sockAddr.sin_family = AF_INET; //IPv4
	sockAddr.sin_port = serverPort;
	sockAddr.sin_addr.s_addr = serverIp;
}

int AsynCore::socketRankBind(struct sockaddr_in *socket_addr)
{
	return bind(current_socket, (struct sockaddr *)socket_addr, sizeof(*socket_addr));
}

int AsynCore::setListen(int num)
{
	return listen(current_socket, num);
}

int AsynCore::acceptSocket()
{
	int return_value;
	struct sockaddr_in temp;
	socklen_t add_size = IP_PORT_SIZE;

	for(;;)
	{
		if((return_value = accept(current_socket, (struct sockaddr *)&temp, &add_size)) < 0)
		{
			if(errno == EINTR) continue;
			else break;
		}
		else break;
	}

	return return_value;
}

void AsynCore::setRanks(int rk, vector<UniqueServer> &rank_set)
{
	rank = rk;
	ranks = rank_set;
}

int AsynCore::initialize(int rk, vector<UniqueServer> &rank_set)
{
	setRanks(rk, rank_set);

	if(spawnSocket() < 0){
		cout << "spawn socket error" << endl;
		return -1;
	}
	if(configSocket(SO_REUSEADDR) < 0){
		cout << "config socket error" << endl;
		return -1;
	}

	struct sockaddr_in ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	socketAddress(ServerAddr, htons(ranks[rank].port), htonl(INADDR_ANY));

	if(socketRankBind(&ServerAddr) < 0){
		cout << "socket bind error" << endl;
		return -1;
	}
	if(setListen(511) < 0){
		cout << "set socket listen error" << endl;
		return -1;
	}

	return 0;
}

int AsynCore::select()
{
	FD_ZERO(&file_descriptors);
	FD_SET(current_socket, &file_descriptors);

	//timeout every 2 seconds
	time_value.tv_sec = Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_s"));
	time_value.tv_usec =Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_ms"));

	return ::select(current_socket+1, &file_descriptors, NULL, NULL, &time_value);
}




