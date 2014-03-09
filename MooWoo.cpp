#include "MooWoo.h"






int main(int argc, char const *argv[])
{
	cout << Config::instance().get("socket::rTimeout") << endl;

	return 0;
}










