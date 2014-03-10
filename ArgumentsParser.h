

#ifndef ARGUMENTSPARSER_MUMAN_H
#define ARGUMENTSPARSER_MUMAN_H


class ArgumentsParser
{
public:
	static int getRank(int argc, char const *argv[]);
	static int parse(int argc, char const *argv[], int *rank, int *L, int *master_mode, int *slave_mode);


};












#endif
