
#include "Reactor.h"


int Reactor::start(int sock)
{
	//if(sock < 0) return -1;

	fd_set file_descriptors;
	struct timeval time_value;

	while(isAlive)
	{
		FD_ZERO(&file_descriptors);
		FD_SET(sock, &file_descriptors);
		time_value.tv_sec = Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_ras"));;
		time_value.tv_usec = Poco::NumberParser::parse(Config::instance().get("socket::listenTimeout_rams"));;

		if (::select(sock+1, &file_descriptors, NULL, NULL, &time_value) > 0)
		{
			/********************
			 receiveMessage:
			 	<0: error
			 	=0: socket closed
			*********************/
			if ( Protocol::receiveMessage(sock, inbox) <= 0) 
			{
				close(sock);
				isAlive = false;
				//need to clear out message to the socket in the outbox , if we use reactor in more than one socket
				

			}
		}

		cout << "inbox: " << inbox.size() << endl;
		//processData
		for(vector<Message>::iterator it=inbox.begin(); it != inbox.end(); ++it)
		{
			cout << "Rank: " << computeCore->getRank() << endl;
			outbox.push_back(computeCore->getMessageHandler(it->getMessageTag())->callback(*it));
		}
		//processMessage
		if(!inbox.empty()) inbox.clear();

		//sendMessage [use timeout to solve communication faulse]
		if(!outbox.empty()) Protocol::sendMessage(sock, outbox);
	}

	return 0;
}







