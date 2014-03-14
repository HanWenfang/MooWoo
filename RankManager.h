
#ifndef SOCKET_MANAGER_MOOWOO_H
#define SOCKET_MANAGER_MOOWOO_H

#include <map>
#include "UniqueServer.h"

using namespace std;

// Long-time connection management for master
class RankManager
{
public:
	void addSocket();

private:
	map<int, UniqueServer> socketDB;
};





#endif
