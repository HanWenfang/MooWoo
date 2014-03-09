
#ifndef PROTOCOL_MUMAN_H
#define PROTOCOL_MUMAN_H
#include "Message.h"
#include "MessageTypes.h"
#include <vector>
#include <iostream>

/******************
 stream format:
 +--------------------------------+
 |   source    : destination | \n |
 +--------------------------------+
 | message_tag : data_length | \n |
 +--------------------------------+
 |            data                |
 +--------------------------------+

*******************/

using namespace std;

class Protocol
{
private:

public:
	static int receiveMessage(int sock, vector<Message> &inbox);
	static int receiveMessageTimeout(int sock, vector<Message> &inbox);
	static void sendMessage(int sock, vector<Message> &outbox);
	static int sendMessageTimeout(int sock, vector<Message> &outbox);
	static void sendMessage(vector<Message> &outbox);
	static string messageToStream(Message &message);

};











#endif
