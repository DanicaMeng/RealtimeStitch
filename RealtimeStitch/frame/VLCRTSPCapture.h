#pragma once

#include "vlc/vlc.h"
#include "opencv2/opencv.hpp"
#include "FrameCapture.h"
#include <mutex>
#include <Windows.h>
#include <time.h>

class VLCRTSPCapture :
	public FrameCapture
{
public:
	VLCRTSPCapture(char* rtspURL,cv::Size size);
	~VLCRTSPCapture();
	int createVclObject();
	void releaseVclObject();

	bool pushFrameToQueue();


public:
	cv::Mat* image;
	uchar* pixels;
	std::mutex mutex;
	time_t m_lastFrameTime;

private:
	libvlc_instance_t *vlcInstance;
	libvlc_media_player_t *mp;
	int frame_index; 
	
	string m_url;
	cv::Size m_size;
};

void *lock(void *data, void**p_pixels);
void display(void *data, void *id);
void unlock(void *data, void *id, void *const *p_pixels);


class VLCCaptureOfflineCheck
{
public:
	static void addVLCRTSPCapture(VLCRTSPCapture* pC);
	static void removeVLCRTSPCapture(VLCRTSPCapture* pC);

	static std::vector<VLCRTSPCapture* > ms_vec;
	static HANDLE ms_offlineThread; //¶ÏÍø¼ì²âÏß³Ì
	
};