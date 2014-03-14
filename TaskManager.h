

#ifndef TASK_MANAGER_MOOWOO_H
#define TASK_MANAGER_MOOWOO_H

#include <vector>
#include <Poco/Mutex.h>
#include <string>

using namespace std;

// thread-safe
class TaskManager
{
public:
	int addTask();
	void deleteTask();
	string getTask();
	bool empty();

private:
	string woo;
	Poco::Mutex mutex;
};





#endif

