#include "stdafx.h"
#include "RTSPCapture.h"


RTSPCapture::RTSPCapture(char* rtspURL)
{
	url = rtspURL;
	cap = cv::VideoCapture(rtspURL);
	readThread = CreateThread(NULL, 0, readThreadFunc, this, 0, NULL);
}


RTSPCapture::~RTSPCapture()
{
	CloseHandle(readThread);
	cap.release();
}

bool RTSPCapture::getFramePushQueue()
{
	if (!cap.isOpened()) return false;
	//std::cout << "read begin...\n";
	cv::Mat frame, frameLast, frameTemp;
	if (!cap.read(frame)) {
		std::cout << "[Error] Cannot get frames!" << std::endl;
		return false;
	}
	if (frame.rows <= 0 || frame.cols <= 0)
	{
		std::cout << "[Error] frame is empty!" << std::endl;
		return false;
	}
	/*
	cv::imshow(url, frame);
	cv::waitKey(10);*/

	pushFrame(frame);
	
	/*else
	{
		//que.push(frame);
		//检查视频帧差别，差别小时，直接丢弃
		frameLast = que.back();
		cv::absdiff(frame, frameLast, frameTemp);
		double sum_m = cv::sum(frameTemp)[0];
		double dm = sum_m / (frameTemp.cols * frameTemp.rows);
		std::cout << "dm  " << dm <<"\n";
		if (dm < 0.2)// you can choose threshold 
		{
			
			return false;
		}
		que.push(frame);
	}*/
	return true;
}


DWORD WINAPI readThreadFunc(LPVOID lpParameter)
{
	RTSPCapture* rCap = (RTSPCapture *)lpParameter;
	while (1)
	{
		rCap->getFramePushQueue();
		// 读取本地视频时，需要间隔
		Sleep(30);
	}
	return 0;
}