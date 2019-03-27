#pragma once
#include <vector>
#include "FrameCapture.h"
#include "RS_define.h"
/*
单例抓取对象管理类
*/
class CaptureManager
{
	SINGLETON_DECLARE(CaptureManager)
public:
	bool initCaptures();

	/*获取 视频抓取对象的列表*/
	const std::vector<FrameCapture*>* getFrameCaptureVecter(){ return &m_frameCptVec; };

	/*获取 抓取到的视频帧：每次返回的向量中，包含着从m_frameCptVec中的每个对象获取一帧
		阻塞调用，获取不到时，会一直等待;*/
	std::vector<cv::Mat>* getFrames();
	

protected:
	CaptureManager();
	~CaptureManager();

private:

	std::vector<FrameCapture*>  m_frameCptVec;

};

