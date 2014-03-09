

#ifndef UNIQUE_SERVER_MUMAN_H
#define UNIQUE_SERVER_MUMAN_H
#include <string>

class UniqueServer 
{

public:
	bool operator== (UniqueServer &right){
		return (ip == right.ip) && (port == right.port);
	}
	bool operator!= (UniqueServer &right){
		return !(*this == right);
	}
	UniqueServer(std::string ip_address, int port_num):ip(ip_address),port(port_num) {}
	UniqueServer(UniqueServer const &it){
		ip = it.ip;
		port = it.port;
	}

	std::string ip;
	int port;
};




#endif


