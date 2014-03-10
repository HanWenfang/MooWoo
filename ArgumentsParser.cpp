
#include "ArgumentsParser.h"
#include <Poco/NumberParser.h>
#include <string.h>

int ArgumentsParser::getRank(int argc, char const *argv[])
{
	for(int i=1; i<argc; i+=2)
	{
		if(strcmp(argv[i], "-rank") == 0) return Poco::NumberParser::parse(argv[i+1]);
	}

	return -1;
}

int ArgumentsParser::parse(int argc, char const *argv[], int *rank, int *Link, int *master_mode, int *slave_mode)
{
	for(int i=1; i<argc; i+=2)
	{
		if(strcmp(argv[i], "-rank") == 0) *rank = Poco::NumberParser::parse(argv[i+1]);
		if(strcmp(argv[i], "-L") == 0) *Link = Poco::NumberParser::parse(argv[i+1]);
		if(strcmp(argv[i], "-master") == 0) *master_mode = Poco::NumberParser::parse(argv[i+1]);
		if(strcmp(argv[i], "-slave") == 0) *slave_mode = Poco::NumberParser::parse(argv[i+1]);
	}

	return 0;
}





