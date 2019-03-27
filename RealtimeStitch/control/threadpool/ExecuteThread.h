#pragma once

/*
//处理过程：
1）创建线程
2）等待
3）设置参数，触发线程执行
4）执行完成，反馈给ThreadPool
5）返回2）
*/

#include <Windows.h>
#include "ThreadPoolTask.h"

class ExecuteThread
{
public:
	enum Status {
		BUSY,
		IDLE
	};
	static int m_sCount;//统计用

	/*phIdleEvent 空闲时发送的事件信号*/
	ExecuteThread(HANDLE * phIdleEvent);
	~ExecuteThread();
	//设置参数开始执行
	bool setParam(ThreadPoolTask * pTask);
	Status getStatus();
	void run();
private:
	HANDLE m_hdlThread;
	ThreadPoolTask *m_pTask;

	HANDLE m_hEvent;//执行事件
	HANDLE * m_phIdleEvent;//线程空闲事件
	Status m_status; //线程状态：空闲，忙碌
	int m_index;//线程号，方便调试区分
	
};

