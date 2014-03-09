

#ifndef COMUPTECORE_MUMAN_H
#define COMUPTECORE_MUMAN_H
#include <vector>
#include "Message.h"
#include "AsynCore.h"
#include "Reactor.h"
#include "MessageTagHandler.h"
#include "RankHandler.h"
#include "UniqueServerQueue.h"
#include <map>

using namespace std;

class ComputeCore
{
private:
	vector<UniqueServerQueue> ranks;
	int rank;
	int master;
	int slave;
	AsynCore asyncore;
	map<int, MessageTagHandler*> MessageTagHandlerTable;
	map<int, RankHandler*> RankHandlerTable;

public:
	ComputeCore(vector<UniqueServerQueue> &rank_set, int rk, int master_mode, int slave_mode)\
	:ranks(rank_set), rank(rk), master(master_mode), slave(slave_mode){ }
	void run();
	
	void registerMessageHandler(MessageTagHandler &messageTagHandler);
	MessageTagHandler* getMessageHandler(int message_tag);

	void registerRankHandler(RankHandler &rankHandler);
	RankHandler *getRankHandler(int rank);
	
	int getRank()
	{
		return rank;
	}
};




#endif
