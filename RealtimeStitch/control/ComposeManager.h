#pragma once
#include "../frame/FrameCapture.h"
#include "opencv2/stitching/stitcher.hpp"

#include <queue>
#include <vector>
#include <Windows.h>

#include "threadpool/ThreadPool.h"
#include "ComposeTask.h"

/*
管理帧合并线程
	1.从FrameCapture列表中获取有效帧列表
	2.选取空闲线程执行合并函数，控制合并后帧的存放位置
*/

class ComposeManager
{
public:
	ComposeManager(int threadNum);
	~ComposeManager();

	//阻塞调用，返回合并后的帧
	bool getNextCaptureFrame(cv::Mat & frame);

	//阻塞调用，获取视频帧向量,交给线程池执行
	bool executeCompose();
private:

	std::queue<ComposeTask*> m_ComposeQue;//合并参数队列
	int m_queMaxCount;
	HANDLE m_hEvent;//合并完成事件

	ThreadPool* m_pThreadPool;
	HANDLE m_hdlThread;
};


