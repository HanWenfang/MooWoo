
#include "Connect.h"
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int Connect::spawnSocket()
{
	int nSndBuf = 1024*32;
	int nRcvBuf = 1024*32;
	int noTCPDelay = 1;

	int reqSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//set socket options
	setsockopt(reqSocket, IPPROTO_TCP, TCP_NODELAY, (const void *)&noTCPDelay, sizeof(noTCPDelay));
	setsockopt(reqSocket, SOL_SOCKET, SO_SNDBUF, (const void *)&nSndBuf, sizeof(nSndBuf));
	setsockopt(reqSocket, SOL_SOCKET, SO_SNDBUF, (const void *)&nRcvBuf, sizeof(nRcvBuf));
	
	return reqSocket;
}

void Connect::socketAddress(struct sockaddr_in &sockAddr, in_port_t serverPort, in_addr_t serverIp)
{
	sockAddr.sin_family = AF_INET; //IPv4
	sockAddr.sin_port = serverPort;
	sockAddr.sin_addr.s_addr = serverIp;
}

int Connect::connect(string &ip, int port, int &socket)
{
	socket = spawnSocket();
	if (socket < 0) {
		cout << "spawn socket error" << endl;
		return -1;
	}
	
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	socketAddress(server_address, htons(port), inet_addr(ip.c_str()));

	return ::connect(socket, (struct sockaddr *)&server_address, sizeof(server_address));
}


int Connect::connectTimeout(string &ip, int port, int &socket, struct timeval time_value)
{
	socket = spawnSocket();
	if (socket < 0) {
		cout << "spawn socket error" << endl;
		return -1;
	}
	
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	socketAddress(server_address, htons(port), inet_addr(ip.c_str()));

	//set socket non-blocked
	int flags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, flags|O_NONBLOCK);

	struct timeval tmp = time_value;

	int rc = ::connect(socket, (struct sockaddr *)&server_address, sizeof(server_address));
	if(rc != 0)
	{
		if(errno != EINPROGRESS || errno != EWOULDBLOCK || errno != EAGAIN)
		{
			cout << "connect timeout error!" << endl;
			close(socket);
			return -1;
		}

		fd_set writable;
		FD_ZERO(&writable);
		FD_SET(socket, &writable);

		while(true)
		{
			int result = ::select(socket+1, NULL, &writable, NULL, &tmp);
		
			if (result > 0)
			{
				int serrno = 0;
				socklen_t len = sizeof(serrno);
				getsockopt(socket, SOL_SOCKET, SO_ERROR, &serrno, &len);
				
				if(serrno == 0) break;
				else 
				{
					cout << "connect sock error! serrno: " << serrno << endl;
					close(socket);
					return -1;
				}
			}

			if(errno == EINTR) tmp = time_value;
			else
			{
				close(socket);
				return -1;
			}
		}
	}

	//restore socket blocked 
	fcntl(socket, F_SETFL, flags);
	return rc;
}

int Connect::configSocket(int option, int &socket)
{
	int on = 1;
	return setsockopt(socket, SOL_SOCKET, option, &on, sizeof(on));
}


