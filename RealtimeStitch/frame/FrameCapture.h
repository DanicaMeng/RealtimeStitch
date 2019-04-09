#pragma once
#include <queue>
#include<iostream>
#include "opencv2/opencv.hpp"
#include <mutex>
#include <Windows.h>

#define FrameQueueMaxSize  20

using namespace std;

class FrameCapture
{
public:
	FrameCapture();
	virtual~FrameCapture();
	//获取视频帧
	//由非阻塞调用改为阻塞调用
	bool next(cv::Mat & frame);
protected:
	bool pushFrame(cv::Mat & frame);

private:
	std::queue<cv::Mat> m_matQue;
	//std::mutex mutexQue;
	CRITICAL_SECTION m_queCS;//比mutex更高效
	HANDLE m_hEvent;//获取到帧事件
	//视频帧放入队列
	
};

