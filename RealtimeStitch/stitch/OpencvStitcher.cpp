#include "stdafx.h"
#include "OpencvStitcher.h"

static Stitcher stitcher = Stitcher::createDefault(false);

OpencvStitcher::OpencvStitcher()
{
	is_estimate = false;
}

OpencvStitcher::~OpencvStitcher()
{
}

bool OpencvStitcher::init(InputArray images)
{
	std::cout << "OpencvStitcher::init\n";
	Stitcher::Status rst = stitcher.estimateTransform(images);
	if (Stitcher::OK == rst)
	{
		is_estimate = true;
		return true;
	}
	else
	{
		return false;
	}
}

Stitcher::Status OpencvStitcher::stitch(InputArray images, OutputArray pano)
{
	std::cout << "OpencvStitcher::stitch\n";
	if (!is_estimate)
	{
		std::cout << "OpencvStitcher::stitch fail\n";
		return Stitcher::ERR_NEED_MORE_IMGS;
	}
	else
	{
		std::cout << "OpencvStitcher::stitch succeed\n";
		return stitcher.composePanorama(images,pano);
	}
}