// RealtimeStitch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "opencv2/opencv.hpp"

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")


#ifdef _DEBUG  
#pragma comment(lib, "opencv_core249d.lib")   
#pragma comment(lib, "opencv_imgproc249d.lib")   //MAT processing  
#pragma comment(lib, "opencv_highgui249d.lib")  
//need by cv::Stitcher
#pragma comment(lib, "opencv_stitching249d.lib")
//need by surf stitcher
#pragma comment(lib, "opencv_flann249d.lib")    
#pragma comment(lib, "opencv_features2d249d.lib")  
#pragma comment(lib, "opencv_nonfree249d.lib")
#pragma comment(lib, "opencv_legacy249d.lib")
#pragma comment(lib, "opencv_calib3d249d.lib")

#else
#pragma comment(lib, "opencv_core249.lib")  
#pragma comment(lib, "opencv_imgproc249.lib")  
#pragma comment(lib, "opencv_highgui249.lib")  
//need by cv::Stitcher
#pragma comment(lib, "opencv_stitching249.lib")
//need by surf stitcher
#pragma comment(lib, "opencv_flann249.lib")  
#pragma comment(lib, "opencv_features2d249.lib")  
#pragma comment(lib, "opencv_nonfree249.lib")  
#pragma comment(lib, "opencv_legacy249.lib")
#pragma comment(lib, "opencv_calib3d249.lib")
#endif 

#include "frame\CaptureManager.h"
#include "stitch\StitchManager.h"
#include "control\ComposeManager.h"


#include "control\ComposeManager.h"

int main(int argc, char* argv[])
{
	std::cout << "Starting...\n";
	bool r = CaptureManager::getInstance()->initCaptures();
	
	vector<cv::Mat> * pV = CaptureManager::getInstance()->getFrames();
	
	StitchManager * p = StitchManager::getInstance();
	bool b = p->initStitchObject(*pV);
	if (!b)
	{
		delete pV;
		std::cout << "StitchManager initStitchObject ERROR\n";
		cv::waitKey(0);
		return 1;
	}
	else
	{
		delete pV;
	}
	std::cout << "Start Compose\n";
	ComposeManager comMng(5);

	cv::Mat pano, image;

	while (1)
	{
		comMng.getNextCaptureFrame(pano);
		CV_Assert(pano.rows > 0 && pano.cols > 0);
		if (!(pano.rows > 0 && pano.cols > 0))
		{
			continue;
		}
		//resize(pano, image, Size(pano.cols*0.5, pano.rows*0.5));
		cv::imshow("pano", pano);
		cv::waitKey(10);
	}
	std::cout << "...End\n";
	
	cv::waitKey(0);
	return 0;
}