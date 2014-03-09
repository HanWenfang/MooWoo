
#ifndef RANKHANDLER_MUMAN_H
#define RANKHANDLER_MUMAN_H
#include "UniqueServerQueue.h"
#include <vector>

using namespace std;

class RankHandler
{

public:
	int rank;
	vector<UniqueServerQueue> ranks;
	bool running_flag;

	RankHandler(int rank_id, vector<UniqueServerQueue> rank_set, int flag)\
	:rank(rank_id), ranks(rank_set), running_flag(flag)
	{

	}
	
	virtual int callback() {return 0;}
};






#endif

