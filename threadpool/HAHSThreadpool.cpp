/*
 * HAHSThreadpool.cpp
 *
 *  Created on: 2015-4-9
 *      Author: mark
 */

#include "HAHSThreadpool.h"
#include <errno.h>
#include <string>

HAHSTask::HAHSTask(void (*funcPtr)(void*), void *arg)
{

}

HAHSTask::~HAHSTask()
{

}

void HAHSTask::operator()()
{
	(*(this->mFuncPtr))(this->mArg);
	if (this->mArg != NULL)
	{
		// 任务执行结束删除参数
		delete this->mArg;
	}
}

void HAHSTask::run()
{
	(*(this->mFuncPtr))(this->mArg);
}

HAHSThreadpool::HAHSThreadpool()
{

}

HAHSThreadpool::HAHSThreadpool(int poolSize)
{
}

HAHSThreadpool::~HAHSThreadpool()
{
}

int HAHSThreadpool::initializeHAHSThreadpool()
{
}

int HAHSThreadpool::destroyHAHSThreadpool()
{
}

void* HAHSThreadpool::executeHAHSThread()
{
}

int HAHSThreadpool::addHAHSTask(HAHSTask* task)
{
}



