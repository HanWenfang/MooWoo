

#ifndef CONNECT_MUMAN_H
#define CONNECT_MUMAN_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

using namespace std;

class Connect
{
public:
	static int spawnSocket();
	static void socketAddress(struct sockaddr_in &sockAddr, in_port_t serverPort, in_addr_t serverIp);
	static int connect(string &ip, int port, int &socket);
	static int connectTimeout(string &ip, int port, int &socket, struct timeval time_value);
	static int configSocket(int option, int &socket);

};








#endif
