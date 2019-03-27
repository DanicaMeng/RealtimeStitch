#pragma once
#include "MatStitcher.h"


class OpencvStitcher :
	public MatStitcher
{
public:
	OpencvStitcher();
	~OpencvStitcher();

	bool init(InputArray images);

	Stitcher::Status stitch(InputArray images, OutputArray pano);

private:
	bool is_estimate;

};

