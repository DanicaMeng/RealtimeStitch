#include "stdafx.h"
#include "MatStitcher.h"


Stitcher::Status TestStitcher::stitch(InputArray images, OutputArray pano)
{
	vector<Mat> mVec;
	images.getMatVector(mVec);
	if (mVec.size() <= 0)
	{
		return Stitcher::ERR_NEED_MORE_IMGS;
	}
	Mat m = mVec[0];
	CV_Assert(m.rows > 0 && m.cols > 0);
	pano.create(m.size(), m.type());
	Mat &pano_ = pano.getMatRef();
	m.copyTo(pano_);
	CV_Assert(pano_.rows > 0 && pano_.cols > 0);
	return Stitcher::OK;
}
