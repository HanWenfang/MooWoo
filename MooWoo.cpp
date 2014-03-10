#include "MooWoo.h"
#include "UniqueServer.h"
#include "ArgumentsParser.h"
#include "ComputeCore.h"



int main(int argc, char const *argv[])
{
	UniqueServerQueue rank0;
	rank0.push_back(UniqueServer("127.0.0.1", 16501));
	rank0.push_back(UniqueServer("127.0.0.1", 16601));
	rank0.push_back(UniqueServer("127.0.0.1", 16701));

	UniqueServerQueue rank1;
	rank1.push_back(UniqueServer("127.0.0.1", 17501));

	UniqueServerQueue rank2;
	rank2.push_back(UniqueServer("127.0.0.1", 18501));

	vector<UniqueServerQueue> ranks;
	ranks.push_back(rank0);
	ranks.push_back(rank1);
	ranks.push_back(rank2);

	int rank = -1;
	int Link = -1;
	int master_mode = -1;
	int slave_mode = -1;

	ArgumentsParser::parse(argc, argv, &rank, &Link, &master_mode, &slave_mode);

	ComputeCore computerCore(ranks, rank, Link, master_mode, slave_mode);

	RankZeroHandler rankZeroHandler(0, ranks, true);
	computerCore.registerRankHandler(rankZeroHandler);

	cout << "start running..." << endl;
	computerCore.run();


	return 0;
}










