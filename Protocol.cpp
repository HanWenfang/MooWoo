
#include "Protocol.h"
#include <unistd.h>
#include <string>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <errno.h>
#include <sys/socket.h>
#include "MooWoo.h"

int Protocol::receiveMessage(int sock, vector<Message> &inbox)
{
	int message_tag = -1;
	int data_length = -1;
	int source = -1;
	int destination = -1;
	int globalReadCounter = 0;

	int counter = 0;
	char character;
	string message_stream;
	bool line_one = true;
	bool line_two = false;

	while(true)
	{
		if(read(sock,&character, 1) != 1)
		{
			if(errno == EINTR) continue;
			else
				break;
		}
		
		++globalReadCounter;
		if(character == ':' && line_one)
		{
			source = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
		}
		else if(character == '\n' && line_one)
		{
			destination = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
			line_one = false;
			line_two = true;
		}
		else if(character == ':' && line_two)
		{
			message_tag = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
		}
		else if(character == '\n' && line_two)
		{
			data_length = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
			line_two=false;
			break;
		}
		else
		{
			message_stream.push_back(character);
		}
	}

	// context is not zero
	while(data_length > 0)
	{
		if(read(sock,&character, 1) != 1)
		{
			if(errno == EINTR) continue;
			else break;
		}
		
		++globalReadCounter;
		message_stream.push_back(character);
		++counter;
		if(counter == data_length) break;
	}
	
	if(globalReadCounter == 0) return 0;

	//receive error [ break condition ] checksum?
	if( data_length != message_stream.size() || source==-1 || destination==-1 || message_tag==-1 ) return -1;

	Message message(source, destination, message_tag, message_stream);
	message.setSocket(sock);

	inbox.push_back(message);

	return 1; // got one Message
}

void Protocol::sendMessage(int sock, vector<Message> &outbox)
{
	string stream;
	int left;
	int written;
	vector<Message> exchange;

	for(vector<Message>::iterator it = outbox.begin(); it != outbox.end(); ++it)
	{
		// ignore the blank message
		if(it->getMessageTag() == BLANK_MESSAGE)
		{
			continue;
		}

		stream = messageToStream(*it);
		left = stream.size(); 
		char const *temp = stream.c_str();

		while(left>0)
		{
			if((written = write(sock, temp, left)) <= 0)
			{
				if(errno == EINTR) continue;
				else break;
			}

			left -= written;
			temp += written;
		}

		// send error [ break condition ]
		if(left != 0)
		{
			exchange.push_back(*it);
		}
	}

	outbox = exchange; // fault-tolerance
}

string Protocol::messageToStream(Message &message)
{
	string stream;
	Poco::NumberFormatter::append(stream, message.getRankSource());
	stream.append(":");
	Poco::NumberFormatter::append(stream, message.getRankDestination());
	stream.append("\n");
	Poco::NumberFormatter::append(stream, message.getMessageTag());
	stream.append(":");
	Poco::NumberFormatter::append(stream, message.getContextSize());
	stream.append("\n");
	stream.append(message.getContext());

	return stream;
}

void Protocol::sendMessage(vector<Message> &outbox)
{
	string stream;
	int left;
	int written;
	vector<Message> exchange;

	for(vector<Message>::iterator it=outbox.begin(); it != outbox.end(); ++it)
	{
		stream = messageToStream(*it);
		left = stream.size(); 
		char const *temp = stream.c_str();

		while(left>0)
		{
			if((written = write(it->getSocket(), temp, left)) <= 0)
			{
				if(errno == EINTR) continue;
				else break;
			}

			left -= written;
			temp += written;
		}
		// send error [ break condition ]
		if(left != 0)
		{
			exchange.push_back(*it);
		}
	}

	outbox = exchange; // fault-tolerance
}


int Protocol::receiveMessageTimeout(int sock, vector<Message> &inbox)
{
	struct timeval tv;
	tv.tv_sec = Poco::NumberParser::parse(Config::instance().get("socket::rTimeout_s"));
	tv.tv_usec = Poco::NumberParser::parse(Config::instance().get("socket::rTimeout_ms"));

	if(::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
	{
		cout << "set timeout error" << endl;
		return -1;
	}

	int message_tag = -1;
	int data_length = -1;
	int source = -1;
	int destination = -1;
	int globalReadCounter = 0;

	int counter = 0;
	char character;
	string message_stream;
	bool line_one = true;
	bool line_two = false;

	int timeoutRetry = 0;

	while(true)
	{
		if(read(sock,&character, 1) != 1)
		{
			if(errno == EINTR) continue;
			else if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				++timeoutRetry;
				if(timeoutRetry > Poco::NumberParser::parse(Config::instance().get("socket::TimeoutRetry")))
				{
					break;
				} 
			}
			else
				break;
		}
		
		++globalReadCounter;
		if(character == ':' && line_one)
		{
			source = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
		}
		else if(character == '\n' && line_one)
		{
			destination = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
			line_one = false;
			line_two = true;
		}
		else if(character == ':' && line_two)
		{
			message_tag = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
		}
		else if(character == '\n' && line_two)
		{
			data_length = Poco::NumberParser::parse(message_stream);
			message_stream.clear();
			line_two=false;
			break;
		}
		else
		{
			message_stream.push_back(character);
		}
	}

	// context is not zero
	while(data_length > 0)
	{
		if(read(sock,&character, 1) != 1)
		{
			if(errno == EINTR) continue;
			else break;
		}
		
		++globalReadCounter;
		message_stream.push_back(character);
		++counter;
		if(counter == data_length) break;
	}
	
	if(globalReadCounter == 0) return 0;

	//receive error [ break condition ] checksum?
	if( data_length != message_stream.size() || source==-1 || destination==-1 || message_tag==-1 ) return -1;

	Message message(source, destination, message_tag, message_stream);
	message.setSocket(sock);

	inbox.push_back(message);

	return 1; // got one Message
}


int Protocol::sendMessageTimeout(int sock, vector<Message> &outbox)
{
	struct timeval tv;
	tv.tv_sec = Poco::NumberParser::parse(Config::instance().get("socket::wTimeout_s"));
	tv.tv_usec = Poco::NumberParser::parse(Config::instance().get("socket::wTimeout_ms"));

	if(::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
	{
		cout << "set timeout error" << endl;
		return -1;
	}

	string stream;
	int left;
	int written;
	vector<Message> exchange;

	for(vector<Message>::iterator it = outbox.begin(); it != outbox.end(); ++it)
	{
		// ignore the blank message
		if(it->getMessageTag() == BLANK_MESSAGE)
		{
			continue;
		}

		stream = messageToStream(*it);
		left = stream.size(); 
		char const *temp = stream.c_str();
		int timeoutRetry = 0;

		while(left>0)
		{
			if((written = write(sock, temp, left)) <= 0)
			{
				if(errno == EINTR) continue;
				else if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					++timeoutRetry;
					if(timeoutRetry > Poco::NumberParser::parse(Config::instance().get("socket::TimeoutRetry")))
					{
						break;
					} 
				}
				else break;
			}

			left -= written;
			temp += written;
		}

		// send error [ break condition ]
		if(left != 0)
		{
			exchange.push_back(*it);
		}
	}

	outbox = exchange; // fault-tolerance

	return 0;
}
