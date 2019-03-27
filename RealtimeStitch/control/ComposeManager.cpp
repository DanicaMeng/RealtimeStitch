#include "stdafx.h"
#include "ComposeManager.h"
#include "../frame/CaptureManager.h"
static DWORD WINAPI threadFun(LPVOID lpParameter);

ComposeManager::ComposeManager(int threadNum)
{
	m_queMaxCount = 100;// m_queMaxCount = 3 * threadNum;

	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//手动置位

	//创建线程池
	m_pThreadPool = new ThreadPool(threadNum);
	if (!m_pThreadPool->initThreadPool())
	{
		//error
		cout << "EEROR:ComposeManager::ComposeManager  initThreadPool error\n";
	}

	//创建获取视频帧并创建任务的线程
	m_hdlThread = CreateThread(NULL, 0, threadFun, this, 0, NULL);
	if (m_hdlThread == NULL)
	{
		cout << "ERROR:CreateThread fail in ComposeManager" << endl;
	}
}

ComposeManager::~ComposeManager()
{
	CloseHandle(m_hdlThread);
	CloseHandle(m_hEvent);
	delete m_pThreadPool;
}

//阻塞调用，返回合并后的帧
bool ComposeManager::getNextCaptureFrame(cv::Mat & frame)
{
	ComposeTask* pTask = NULL;

	do
	{
		if (!m_ComposeQue.empty())
		{
			pTask = m_ComposeQue.front();
			if (pTask->status == 0) //任务完成
			{
				pTask = NULL; //继续循环等待下一个完成信号
			}
			else if(pTask->status == 1)
			{
				CV_Assert(pTask->pano.rows > 0 && pTask->pano.cols > 0);
				frame = pTask->pano;
				m_ComposeQue.pop();
				delete pTask;
				break;
			}
			else
			{
				//compose error
				m_ComposeQue.pop();
				delete pTask;
				pTask = NULL; //继续循环等待下一个完成信号
			}
		}
		ResetEvent(m_hEvent);
		//等待直至合并执行完成
		WaitForSingleObject(m_hEvent, INFINITE);//等待合并执行完成事件被触发
	}while (pTask == NULL);

	cout << "ComposeManager::getNextCaptureFrame end\n";
	return true;
}

/*
从m_composeFunc 中取出帧，调用线程池进行合并
*/
bool ComposeManager::executeCompose()
{
	ComposeTask* pTask = new ComposeTask(&m_hEvent);

	//获取视频帧
	pTask->imgs = CaptureManager::getInstance()->getFrames();
	/*
	帧获取速度 > 合并速度时，m_ComposeQue会积压的越来越大，
	另外，显示速度 < 合并速度时，m_ComposeQue会积压的越来越大，但获取实时视频帧时，不会，
	*/
	if (m_ComposeQue.size() > m_queMaxCount)//控制合并队列的大小
	{
		delete pTask;//被丢掉的是帧队列中剩余的最早的帧，保证实时性
		return false;
	}//*/
	//放入合并队列
	m_ComposeQue.push(pTask);
	//std::cout << "add a ComposeItem,que size "<< m_ComposeQue.size() << std::endl;
	//线程池异步执行任务
	return m_pThreadPool->addJob(pTask);
}

static DWORD WINAPI threadFun(LPVOID lpParameter)
{
	ComposeManager* pEctThd = (ComposeManager *)lpParameter;
	while (1)
	{
		pEctThd->executeCompose();
	}
	return 1;
}