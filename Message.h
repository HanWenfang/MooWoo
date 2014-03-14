

#ifndef MESSAGE_MUMAN_H
#define MESSAGE_MUMAN_H
#include <string>

using namespace std;

class Message
{
private:
	int rank_source;
	int rank_destination;
	
	int message_tag;
	int blockID;
	int socket; // not unique

	string context; // stream protocol

public:
	Message(int rank_src, int rank_des, int message_t, string cont)\
	:rank_source(rank_src), rank_destination(rank_des), message_tag(message_t), context(cont)
	{
		
	}

	string getContext();
	void setContext(string &c);
	int getContextSize();
	
	int getRankSource();
	void setRankSource(int rank);
	
	int getRankDestination();
	void setRankDestination(int rank);

	int getMessageTag();
	void setMessageTag(int mtag);

	void setSocket(int sock);
	int getSocket();

	void setblockID(int bID);
	int getblockID();
};






#endif

