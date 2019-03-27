#include "stdafx.h"
#include "ExecuteThread.h"
#include <iostream>

static DWORD WINAPI threadFun(LPVOID lpParameter);

int ExecuteThread::m_sCount = 0;

ExecuteThread::ExecuteThread(HANDLE * phIdleEvent)
{
	m_pTask = NULL;
	m_phIdleEvent = phIdleEvent;
	
	//创建控制线程的事件，初始状态无信号,在WaitForSingleObject后,系统自动清除事件信号
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	//启动线程
	m_hdlThread = CreateThread(NULL, 0, threadFun, this, 0, NULL);
	m_sCount++;
	m_index = m_sCount;
	//设置为空闲状态
	m_status = IDLE;
}


ExecuteThread::~ExecuteThread()
{
	//关闭线程句柄
	CloseHandle(m_hdlThread);

	CloseHandle(m_hEvent);
}

bool ExecuteThread::setParam(ThreadPoolTask * pTask)
{
	//设置为非空闲状态
	m_status = BUSY;
	m_pTask = pTask;
	//触发执行
	SetEvent(m_hEvent);
	return true;
}

void ExecuteThread::run()
{
	while (1)
	{
		WaitForSingleObject(m_hEvent, INFINITE);//等待
		//调用合并
		std::cout << "ExecuteThread " << m_index << " runing"<<std::endl;
		m_pTask->execute();
		m_pTask = NULL;

		//设置为空闲状态
		m_status = IDLE;
		//发送空闲事件，触发ThreadPool检索空闲线程和完成的帧
		SetEvent(*m_phIdleEvent);
	}
}

ExecuteThread::Status ExecuteThread::getStatus()
{
	return m_status;
}

static DWORD WINAPI threadFun(LPVOID lpParameter)
{
	ExecuteThread* pEctThd = (ExecuteThread *)lpParameter;
	pEctThd->run();
	return 1;
}

