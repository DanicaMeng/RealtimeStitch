#pragma once
#include "MatStitcher.h"

typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}four_corners_t;


class SurfStitcher :
	public MatStitcher
{
public:
	SurfStitcher();
	~SurfStitcher();
	Stitcher::Status stitch(InputArray images, OutputArray pano);
private:
	Stitcher::Status mapping(InputArray images, OutputArray pano);

	bool is_mapping;
	four_corners_t corners;
	Mat projection_matrix;
};

four_corners_t CalcCorners(const Mat& H, const Mat& src);
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst, four_corners_t corners);
