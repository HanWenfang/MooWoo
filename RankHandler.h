
#ifndef RANKHANDLER_MUMAN_H
#define RANKHANDLER_MUMAN_H
#include "UniqueServer.h"
#include <vector>

using namespace std;

class RankHandler
{

public:
	int rank;
	vector<UniqueServer> ranks;
	bool running_flag;

	RankHandler(int rank_id, vector<UniqueServer> rank_set, int flag)\
	:rank(rank_id), ranks(rank_set), running_flag(flag)
	{

	}
	
	virtual int callback() {return 0;}
};






#endif

