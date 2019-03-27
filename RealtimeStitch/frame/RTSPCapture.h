#pragma once
#include <queue>
#include "opencv2/opencv.hpp"

#include <Windows.h>
#include "FrameCapture.h"

/*使用openCV的 VideoCapture对视频读取*/

class RTSPCapture :public FrameCapture
{
public:
	RTSPCapture(char* rtspURL);
	~RTSPCapture();
	bool RTSPCapture::getFramePushQueue();
private:
	cv::VideoCapture cap;
	HANDLE readThread;
	string url;
};

DWORD WINAPI readThreadFunc(LPVOID lpParameter);

