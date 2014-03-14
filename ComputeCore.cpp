
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

	// master : for the example
	if(rank == 0)
	{
		Poco::Timer timer(100, 60000);
		timer.start(Poco::TimerCallback<Master>(masterTask, &Master::run));
	}

	// Client will create long-time connections firstly: for the example
	// You can use a thread to server the long-time connection
	int sock;
	if(rank != 0)
	{
		// Timeout.....
		string master_ip = ranks[0].next().ip;
		int master_port = ranks[0].next().port;
		if ( Connect::connect(master_ip, master_port, sock) < 0)
		{
			cout << "connect " << master_ip << ":" << master_port << " error" << endl;
			cout << strerror(errno) << endl;
		}
	}

	if(asyncore.initialize(rank, Link, ranks) < 0)
	{
		cout << "initialize asyncore error." << endl;
		return;
	}

	startThreads();

	vector<Message> outbox;
	vector<Message> inbox;

	for(;;)
	{
		if(asyncore.select() > 0)
		{
			asyncore.acceptSocket();
		}

		cout << "rank: " << rank <<endl;

		// Workers "GET" method
		// It is bad to put them here......will be out of control!
		if(rank != 0)
		{
			// one - by - one
			
			cout << "send message..." << endl;
			
			Message message(rank, 0, WOO_MESSAGE_GET, "");
			outbox.push_back(message);

			// No Timeout....
			// Get
			Protocol::sendMessage(sock, outbox);

			Protocol::receiveMessage(sock, inbox);
			
			//process
			for(vector<Message>::ietartor it = inbox.begin(); it != inbox.end(); ++it)
			{
				if(it->getMessageTag() == WOO_MESSAGE) cout << "rank: " << rank <<  "Moo " << it->getContext() << endl;
				// record the status in the record
				statusRecord[it->getblockID()] = 0;
			}

			


			close(sock);
		}
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


