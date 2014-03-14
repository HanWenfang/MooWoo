

#ifndef MASTER_MOOWOO_H
#define MASTER_MOOWOO_H
#include "TaskManager.h"
#include <Poco/Timer.h>

using Poco::Timer;

// Master function core 
class Master
{
public:
	Master(TaskManager &taskM):taskManager(taskM) {} 
	void run(Timer &timer)
	{
		if (taskManager.addTask() < 0)
			cout << " Task is not finished!" << endl;
		else
			cout << "Add new Task!" << endl;
	}

private:
	TaskManager &taskManager;
};






#endif

