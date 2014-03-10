
#include "ComputeCore.h"

void ComputeCore::run()
{
	// rank initial operation
	for(map<int, RankHandler*>::iterator it=RankHandlerTable.begin(); it != RankHandlerTable.end(); ++it)
	{
		if(it->second->running_flag && it->second->rank == rank)
		{
			it->second->callback();
		}
	}

	if(asyncore.initialize(rank, Link, ranks) < 0)
	{
		cout << "initialize asyncore error." << endl;
		return;
	}

	startThreads();

	for(;;)
	{
		if(asyncore.select() > 0)
		{
			asyncore.acceptSocket();
		}

		cout << "rank: " << rank <<endl;
	}
}

void ComputeCore::registerMessageHandler(MessageTagHandler &messageTagHandler)
{
	MessageTagHandlerTable[messageTagHandler.message_tag] = &messageTagHandler;
}

void ComputeCore::registerRankHandler(RankHandler &rankHandler)
{
	RankHandlerTable[rankHandler.rank] = &rankHandler;
}

MessageTagHandler* ComputeCore::getMessageHandler(int message_tag)
{
	return MessageTagHandlerTable[message_tag];
}

RankHandler* ComputeCore::getRankHandler(int rank)
{
	return RankHandlerTable[rank];
}

void ComputeCore::startThreads()
{
	int i = 0;
	while( i < Poco::NumberParser::parse(Config::instance().get("ThreadNum")))
	{
		tReactors.push_back(new TReactor(this, event_queue, i));
	}

	for(vector<TReactor *>::iterator it=tReactors.begin(); it != tReactors.end(); ++it)
	{
		Poco::ThreadPool::defaultPool().start(*(*it));
	}
}


