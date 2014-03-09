#include "MooWoo.h"
#include "UniqueServer.h"
#include "ArgumentsParser.h"
#include "ComputeCore.h"



int main(int argc, char const *argv[])
{
	UniqueServer rank0("127.0.0.1", 16501);
	UniqueServer rank1("127.0.0.1", 17501);
	UniqueServer rank2("127.0.0.1", 18501);

	vector<UniqueServer> ranks;
	ranks.push_back(rank0);
	ranks.push_back(rank1);
	ranks.push_back(rank2);

	int rank = -1;
	int master_mode = -1;
	int slave_mode = -1;

	ArgumentsParser::parse(argc, argv, &rank, &master_mode, &slave_mode);

	ComputeCore computerCore(ranks, rank, master_mode, slave_mode);

	RankZeroHandler rankZeroHandler(0, ranks, true);
	computerCore.registerRankHandler(rankZeroHandler);

	cout << "start running..." << endl;
	computerCore.run();


	return 0;
}










