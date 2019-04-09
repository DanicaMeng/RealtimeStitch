#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/stitching/stitcher.hpp"

using namespace cv;
using namespace std;

class MatStitcher
{
public:
	virtual Stitcher::Status stitch(InputArray images, OutputArray pano)=0;
	virtual ~MatStitcher(){};
};

class TestStitcher: public MatStitcher
{
public:
	Stitcher::Status stitch(InputArray images, OutputArray pano);
};