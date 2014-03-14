
#include "Message.h"

string Message::getContext()
{
	return context;
}

void Message::setContext(string &c)
{
	context = c;
}

int Message::getContextSize()
{
	return context.size();
}

int Message::getRankSource()
{
	return rank_source;
}

void Message::setRankSource(int rank)
{
	rank_source = rank;
}

int Message::getRankDestination()
{
	return rank_destination;
}

void Message::setRankDestination(int rank)
{
	rank_destination = rank;
}

int Message::getMessageTag()
{
	return message_tag;
}

void Message::setMessageTag(int mtag)
{
	message_tag = mtag;
}

void Message::setSocket(int sock)
{
	socket = sock;
}

int Message::getSocket()
{
	return socket;
}

void Message::setblockID(int bID)
{
	blockID = bID;
}

int Message::getblockID()
{
	return bID;
}

