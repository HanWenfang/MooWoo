
#ifndef SOCKET_EVENT_MOOWOO_H
#define SOCKET_EVENT_MOOWOO_H
#include <Poco/Notification.h>

class SocketEvent: public Poco::Notification 
{
public:
	SocketEvent(int sock):socket(sock) {}
	~SocketEvent() {}
	

	int socket;
};





#endif



