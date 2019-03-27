#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include "MatStitcher.h"
#include "RS_define.h"
using namespace cv;
using namespace std;

/*
单例的管理类
管理和设置拼接的对象，对外提供静态的访问方法;
*/

class StitchManager
{
	/*单例定义*/
	SINGLETON_DECLARE(StitchManager)
public:
	bool initStitchObject(InputArray images);
	// 执行合并
	Stitcher::Status stitch(InputArray images, OutputArray pano);

private:
	StitchManager();
	~StitchManager();
	//用于拼接的对象
	MatStitcher * pMS;

};

