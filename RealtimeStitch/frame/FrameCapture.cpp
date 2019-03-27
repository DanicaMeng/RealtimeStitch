#include "stdafx.h"
#include "FrameCapture.h"

FrameCapture::FrameCapture()
{
	InitializeCriticalSection(&m_queCS);
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

FrameCapture::~FrameCapture()
{
	DeleteCriticalSection(&m_queCS);
	CloseHandle(m_hEvent);
}

bool FrameCapture::next(cv::Mat & frame)
{
	//从队列中获取，队列中没有时等待
	while (m_matQue.empty())
	{
		ResetEvent(m_hEvent);
		WaitForSingleObject(m_hEvent, INFINITE);//等待
		//cout << "FrameCapture::next after WaitForSingleObject, m_matQue size:" << que.size()<<endl;
	}
	//cout << "FrameCapture::next end while\n";
	{
		EnterCriticalSection(&m_queCS);
		frame = m_matQue.front();
		m_matQue.pop();
		LeaveCriticalSection(&m_queCS);
		CV_Assert(frame.rows > 0 && frame.cols > 0);
		return true;
	}
}

bool FrameCapture::pushFrame(cv::Mat & frame)
{
	CV_Assert(frame.rows > 0 && frame.cols > 0);

	EnterCriticalSection(&m_queCS);
	//限制队列的大小
	if (m_matQue.size() > FrameQueueMaxSize)
	{
		m_matQue.pop();
	}
	m_matQue.push(frame);
	LeaveCriticalSection(&m_queCS);
	SetEvent(m_hEvent);
	return true;
}