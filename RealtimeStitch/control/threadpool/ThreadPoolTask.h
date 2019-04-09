#pragma once

/*线程池需要执行任务的基类
	
*/
class ThreadPoolTask
{
public:
	virtual bool execute() = 0;
	virtual ~ThreadPoolTask(){};
};

