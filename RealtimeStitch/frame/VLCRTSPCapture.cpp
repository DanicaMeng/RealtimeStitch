#include "stdafx.h"
#include "VLCRTSPCapture.h"
#include <iostream>

VLCRTSPCapture::VLCRTSPCapture(char* rtspURL, cv::Size size)
{
	m_url = rtspURL;
	m_size = size;

	image = new cv::Mat(size.height, size.width, CV_8UC3);
	pixels = image->data;

	frame_index = 0;
	m_lastFrameTime = 0;
	createVclObject();

	VLCCaptureOfflineCheck::addVLCRTSPCapture(this);
}

int VLCRTSPCapture::createVclObject()
{
	std::cout << "VLCRTSPCapture::createVclObject" << std::endl;
	libvlc_media_t *media;
	char const *vlc_argv[] = {
		"--no-audio",
		"--no-xlib",
	};
	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	vlcInstance = libvlc_new(vlc_argc, vlc_argv);
	media = libvlc_media_new_location(vlcInstance, m_url.c_str());
	mp = libvlc_media_player_new_from_media(media);
	libvlc_media_release(media);
	//注意：断网不会引起以上过程出错，即断网情况下，mp != NULL；
	if (mp != NULL)
	{
		libvlc_video_set_callbacks(mp, lock, unlock, display, this);
		libvlc_video_set_format(mp, "RV24", m_size.width, m_size.height, m_size.width * 3);
		libvlc_media_player_play(mp);
		m_lastFrameTime = time(NULL) + 180;//从创建到获取第一帧需要时间比较长，要留一定时间，否则容易不停创建，单位秒；
		return 0;
	}
	else
	{
		std::cout << "[Error] createVclObject: libvlc_media_player_new_from_media is error!" << std::endl;
		return -1;
	}
}
void VLCRTSPCapture::releaseVclObject()
{

	std::cout << "VLCRTSPCapture::releaseVclObject" << std::endl;
	if (vlcInstance != NULL)
	{
		libvlc_release(vlcInstance);
	}
	if (mp != NULL)
	{
		libvlc_media_player_stop(mp);
		libvlc_media_player_release(mp);
	}
}


VLCRTSPCapture::~VLCRTSPCapture()
{
	VLCCaptureOfflineCheck::removeVLCRTSPCapture(this);
	releaseVclObject();
	delete image;
}

bool VLCRTSPCapture::pushFrameToQueue()
{
	if (frame_index < 100)//开始时不完整，前100帧丢弃
	{
		frame_index++;
		return false;
	}
	if (image->rows <= 0 || image->cols <= 0)
	{
		std::cout << "[Error] pushFrameToQueue: frame is empty!" << std::endl;
		return false;
	}
	//cout << "image: " << image->rows << "  " << image->cols << endl;
	pushFrame(*image);
	//std::cout << "VLCRTSPCapture::pushFrameToQueue SetEvent, que size:" << que.size() << endl;
	return true;
}

void *lock(void *data, void**p_pixels)
{
	VLCRTSPCapture * pC = (VLCRTSPCapture *)data;
	pC->mutex.lock();
	*p_pixels = pC->pixels;
	return NULL;
}


void display(void *data, void *id)
{

}


void unlock(void *data, void *id, void *const *p_pixels)
{
	VLCRTSPCapture * pC = (VLCRTSPCapture *)data;
	pC->pushFrameToQueue();
	pC->mutex.unlock();
	pC->m_lastFrameTime = time(NULL);
}

#define TIME_OUT_VALUE  (60)  //单位秒

DWORD WINAPI offlineThreadFunc(LPVOID lpParameter)
{
	std::vector<VLCRTSPCapture* > * pVec = (std::vector<VLCRTSPCapture* > *)lpParameter;
	while (1)
	{
		time_t t = time(NULL);

		int i, count = pVec->size();
		for (i = 0; i != count; i++)
		{
			VLCRTSPCapture* pC = pVec->at(i);
			if (t - pC->m_lastFrameTime > TIME_OUT_VALUE)
			{
				pC->releaseVclObject();
				pC->createVclObject();
			}
		}
		Sleep(TIME_OUT_VALUE * 1000);
	}
	
	return 0;
}

std::vector<VLCRTSPCapture* > VLCCaptureOfflineCheck::ms_vec;
HANDLE VLCCaptureOfflineCheck::ms_offlineThread = NULL; //断网检测线程


void VLCCaptureOfflineCheck::addVLCRTSPCapture(VLCRTSPCapture* pC)
{
	ms_vec.push_back(pC);
	if (!ms_offlineThread)
	{
		//启动监控线程
		ms_offlineThread = CreateThread(NULL, 0, offlineThreadFunc, &ms_vec, 0, NULL);
	}
}
void VLCCaptureOfflineCheck::removeVLCRTSPCapture(VLCRTSPCapture* pC)
{
	std::vector<VLCRTSPCapture* >::iterator itor;
	for (itor = ms_vec.begin(); itor != ms_vec.end(); itor++)
	{
		if (*itor == pC)
		{
			ms_vec.erase(itor);
			break;
		}
	}
	if (ms_vec.size() <= 0 && !ms_offlineThread)
	{
		CloseHandle(ms_offlineThread);
		ms_offlineThread = NULL;
	}
}


