/*
 * HAHSThreadpool.h
 *
 *  Created on: 2015-4-9
 *      Author: mark
 */

#ifndef HAHSTHREADPOOL_H_
#define HAHSTHREADPOOL_H_

#include <Mutex.h>
#include <Condition.h>
#include <list>
#include <vector>

const int DEFAULT_HAHSPTHREADPOOL_SIZE = 10;

enum HAHSTHREADPOOLSTATUS
{
	STARTED = 0,
	STOPPED = 1,
};


// 任务类
class HAHSTask
{
public:
	HAHSTask(void (*funcPtr)(void*), void *arg);
	~HAHSTask();
	void operator()();
	void run();

private:
	void (*mFuncPtr)(void*);
	void *mArg;
};

class HAHSThreadpool
{

public:
	HAHSThreadpool();
	HAHSThreadpool(int poolSize);
	~HAHSThreadpool();

public:
	int initializeHAHSThreadpool();
	int destroyHAHSThreadpool();
	void* executeHAHSThread();
	int addHAHSTask(HAHSTask *task);

private:
	Mutex mTaskMutex;
	Condition mTaskCondition;
	std::vector<pthread_t> mThreads;
	std::list<HAHSTask*> mTasks;
	volatile int mPoolState;
	int mPoolSize;
};

#endif /* HAHSTHREADPOOL_H_ */
