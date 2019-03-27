#pragma once
//#include "stdafx.h"

/*正常销毁的单例类*/

/*定义
m_garbo 定义一个内部类的静态对象         
当该对象销毁时，顺带就释放myInstance指向的堆区资源*/   


#define SINGLETON_DECLARE(className)      \
public:                                   \
	static className * getInstance();     \
private:                                  \
	static className * myInstance;        \
	/* 定义一个内部类*/                   \
	class CGarbo{                         \
	public:                               \
		CGarbo(){};                       \
		~CGarbo()                         \
		{                                 \
			if (nullptr != myInstance)    \
			{                             \
				delete myInstance;        \
				myInstance = nullptr;     \
			}                             \
		}                                 \
	};                                    \
	static CGarbo m_garbo;                


/*实现*/
#define  SINGLETON_IMPLEMENT(className)  \
className* className::myInstance = nullptr; \
className* className::getInstance()      \
{                                        \
	if (myInstance == nullptr)          \
	{                                    \
		myInstance = new className();    \
	}                                    \
	return myInstance;                   \
}                                        
