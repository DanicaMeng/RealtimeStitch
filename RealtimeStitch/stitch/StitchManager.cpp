#include "stdafx.h"
#include "StitchManager.h"
#include "OpencvStitcher.h"
#include "SurfStitcher.h"


//单例实现
SINGLETON_IMPLEMENT(StitchManager)

StitchManager::StitchManager()
{
	pMS = nullptr;
}


StitchManager::~StitchManager()
{
	if (pMS != nullptr)
	{
		delete pMS;
		pMS = nullptr;
	}
}
bool StitchManager::initStitchObject(InputArray images)
{
	cout << "StitchManager::initStitchObject" << endl;
	//*
	pMS = new  TestStitcher(); //TestStitcher();// SurfStitcher(); //
	/*/
	OpencvStitcher *p = new OpencvStitcher();
	if (p->init(images))
	{
		pMS = p;
	}
	else
	{
		//error
		return false;
	}
	//*/
	return true;
}
// 执行合并
Stitcher::Status StitchManager::stitch(InputArray images, OutputArray pano)
{
	if (pMS != nullptr)
	{
		return pMS->stitch(images, pano);
	}
	return Stitcher::Status::ERR_NEED_MORE_IMGS;
}
