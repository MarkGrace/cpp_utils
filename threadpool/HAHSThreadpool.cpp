/*
 * HAHSThreadpool.cpp
 *
 *  Created on: 2015-4-9
 *      Author: mark
 */

#include "HAHSThreadpool.h"
#include <errno.h>
#include <string>

extern "C"
void* startHAHSThread(void *arg)
{
	HAHSThreadpool *p = (HAHSThreadpool*)arg;
	p->executeHAHSThread();
	return NULL;
}


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
	this->mPoolSize = DEFAULT_HAHSPTHREADPOOL_SIZE;
}

HAHSThreadpool::HAHSThreadpool(int poolSize)
{
}

HAHSThreadpool::~HAHSThreadpool()
{
	if (this->mPoolState != HAHSTHREADPOOLSTATUS::STOPPED)
	{
		this->destroyHAHSThreadpool();
	}
}

int HAHSThreadpool::initializeHAHSThreadpool()
{
	this->mPoolState = HAHSTHREADPOOLSTATUS::STARTED;
	for (int i = 0; i < this->mPoolSize; i++)
	{
		pthread_t threadID;
		int ret = pthread_create(&threadID, NULL, startHAHSThread, (void*)this);
		if (ret != 0)
		{
			return -1;
		}
		this->mThreads.push_back(threadID);
	}

	return 0;
}

int HAHSThreadpool::destroyHAHSThreadpool()
{
	this->mTaskMutex.lock();
	this->mPoolState = HAHSTHREADPOOLSTATUS::STOPPED;
	this->mTaskMutex.unlock();

	this->mTaskCondition.broadcast(); // 唤醒所有线程

	for (int i = 0; i < this->mPoolSize; i++)
	{
		void *result;
		int ret = pthread_join(this->mThreads[i], &result);
		if (ret != 0)
		{
			// join出错，根据errno值做相应处理
			switch (errno)
			{
			case EDEADLK:
				// 有死锁
				break;
			case EINVAL:
				// 不是一个joinable线程
				break;
			case ESRCH:
				// 没有找到该线程
				break;
			default:
				break;
			}
		}
		this->mTaskCondition.broadcast();
	}

	return 0;
}

void* HAHSThreadpool::executeHAHSThread()
{
	HAHSTask *task = NULL;

	while (1)
	{
		this->mTaskMutex.lock();

		while ((this->mPoolState != HAHSTHREADPOOLSTATUS::STOPPED) && (this->mTasks.empty()))
		{
			this->mTaskCondition.wait(this->mTaskMutex.getMutexPtr());
		}

		if (this->mPoolState == HAHSTHREADPOOLSTATUS::STOPPED)
		{
			this->mTaskMutex.unlock();
			pthread_exit(NULL);
		}

		task = this->mTasks.front();
		this->mTasks.pop_front();
		this->mTaskMutex.unlock();
		(*task)();

		delete task;
	}
	return NULL;
}

int HAHSThreadpool::addHAHSTask(HAHSTask* task)
{
	this->mTaskMutex.lock();
	this->mTasks.push_back(task);
	this->mTaskCondition.signal();
	this->mTaskMutex.unlock();
	return 0;
}



