#pragma once
/*
线程池
1）不采用任务队列；有任务时，找空闲线程处理，没有空闲线程就等待

*/
#include <vector>
#include "ExecuteThread.h"
#include "ThreadPoolTask.h"

class ThreadPool
{
public:
	ThreadPool(int threadNum);
	~ThreadPool();
	bool initThreadPool();
	//阻塞调用，直至等到空闲线程去执行任务
	bool addJob(ThreadPoolTask * pTask);
protected:
	//阻塞调用
	ExecuteThread * getIdleThread();
private:
	int m_threadNum; //池中总线程数
	std::vector<ExecuteThread *> m_vecThread;

	HANDLE m_hEvent;//线程空闲事件，池中线程执行完任务后，设置信号
	int m_curIndex; //下一次查找空闲线程从该索引位置开始
};

