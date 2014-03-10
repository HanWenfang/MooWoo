

#ifndef COMUPTECORE_MUMAN_H
#define COMUPTECORE_MUMAN_H
#include <vector>
#include "Message.h"
#include "AsynCore.h"
#include "MessageTagHandler.h"
#include "RankHandler.h"
#include "UniqueServerQueue.h"
#include "MooWoo.h"
#include <map>
#include <Poco/NotificationQueue.h>
#include <Poco/ThreadPool.h>
#include "TReactor.h"

using namespace std;

class ComputeCore
{
private:
	vector<UniqueServerQueue> ranks;
	int rank;
	int Link;
	int master;
	int slave;
	Poco::NotificationQueue event_queue;
	AsynCore asyncore;
	map<int, MessageTagHandler*> MessageTagHandlerTable;
	map<int, RankHandler*> RankHandlerTable;

	vector<TReactor*> tReactors;
	void startThreads();

public:
	ComputeCore(vector<UniqueServerQueue> &rank_set, int rk, int L, int master_mode, int slave_mode)\
	:ranks(rank_set), rank(rk), Link(L), master(master_mode), slave(slave_mode), asyncore(event_queue){ }
	~ComputeCore()
	{
		for(vector<TReactor *>::iterator it=tReactors.begin(); it != tReactors.end(); ++it)
		{
			delete *it;
		}
	}

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
