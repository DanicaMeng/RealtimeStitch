#include "stdafx.h"
#include "ComposeTask.h"
#include "../stitch/StitchManager.h"


ComposeTask::ComposeTask(HANDLE* event)
{
	status = 0;
	finishEvent = event;
	imgs = nullptr;
}


ComposeTask::~ComposeTask()
{
	if (imgs != nullptr)
	{
		delete imgs;
		imgs = nullptr;
	}
}

bool ComposeTask::execute()
{
	if(StitchManager::getInstance()->stitch(*imgs, pano) == cv::Stitcher::OK)
	{
		status = 1;
		
	}
	else
	{
		status = -1;
		cout << "Error: ComposeTask stitch failed." << endl;
	}
	CV_Assert(pano.rows > 0 && pano.cols > 0);
	SetEvent(*finishEvent);
	return (status==1);
}