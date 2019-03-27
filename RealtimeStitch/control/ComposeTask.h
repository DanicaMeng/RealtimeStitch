#pragma once
#include "opencv2/stitching/stitcher.hpp"
#include <vector>
#include <Windows.h>

#include "threadpool/ThreadPoolTask.h"

typedef cv::Stitcher::Status(*ComposeFunc)(cv::InputArray images, cv::OutputArray pano);



class ComposeTask :
	public ThreadPoolTask
{
public:
	ComposeTask(HANDLE* event);
	~ComposeTask();

	bool execute();

	int status; //0：未完成的，1：合并成功完成的，-1: 合并出错
	std::vector<cv::Mat>* imgs;
	cv::Mat pano;//合并结果
	HANDLE* finishEvent;
};

