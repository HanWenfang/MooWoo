
#include "ComputeCore.h"
#include "Reactor.h"

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

	if(asyncore.initialize(rank, ranks) < 0)
	{
		cout << "initialize asyncore error." << endl;
		return;
	}

	Reactor reactor(this);

	for(;;)
	{
		if(asyncore.select() > 0)
		{
			reactor.start(asyncore.acceptSocket()); // reactor--must be asynchronous
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

