#include <Poco/SingletonHolder.h>
#include <Poco/StringTokenizer.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/**********************************************
 How to use: 
	Config::instance().get("socket::rTimeout")

***********************************************/
class Config
{
public:
	Config()
	{
		parse();
	}

	void print()
	{
		for(map<string, string>::iterator it = pool.begin(); it != pool.end(); ++it)
		{
			cout << it->first << " : " << it->second << endl;
		}
	}

	string get(char const *name)
	{
		return pool[name];
	}

	static Config &instance()
	{
		static Poco::SingletonHolder<Config> sh;
		return *sh.get();
	}

private:
	void parse()
	{
		ifstream moowoo("./MooWoo.config");
		vector<string> configs;
		string tmp;

		while(getline(moowoo, tmp))
		{
			configs.push_back(tmp);
		}
		moowoo.close();

		for(vector<string>::iterator it = configs.begin(); it != configs.end(); ++it)
		{
			if(!(it->empty())){
				Poco::StringTokenizer tokenizer(*it, "=", Poco::StringTokenizer::TOK_TRIM);
				pool[*tokenizer.begin()] = *(tokenizer.begin() + 1);
			}
		}
	}

	map<string, string> pool;
};

