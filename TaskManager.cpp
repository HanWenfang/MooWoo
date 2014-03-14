#include "TaskManager.h"

int TaskManager::addTask()
{
	if (woo.empty())
	{
		Poco::Mutex::ScopedLock lock(mutex);
		if (woo.empty()) woo = "WOO";
		return 0;
	}

	return -1;
}

void TaskManager::deleteTask()
{
	if(!woo.empty())
	{
		Poco::Mutex::ScopedLock lock(mutex);
		if(!woo.empty())
		{
			woo = "";
		}
	}

	return;
}

string TaskManager::getTask()
{
	if(!woo.empty())
	{
		Poco::Mutex::ScopedLock lock(mutex);
		if(!woo.empty())
		{
			string tmp = woo;
			woo = "";
			return tmp;
		}
	}

	return string("");
}




