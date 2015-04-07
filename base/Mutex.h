/*
 * Mutex.h
 *
 *  Created on: 2015年4月7日
 *      Author: mark
 */

#ifndef BASE_MUTEX_H_
#define BASE_MUTEX_H_

#include <pthread.h>

class Mutex
{
public:
	Mutex()
	{
		pthread_mutex_init(&mMutex, NULL);
		isLocked = false;
	}

	~Mutex()
	{
		while (isLocked);
		unlock();
		pthread_mutex_destroy(&mMutex);
	}

	void lock()
	{
		pthread_mutex_lock(&mMutex);
		isLocked = true;
	}

	void unlock()
	{
		isLocked = false;
		pthread_mutex_unlock(&mMutex);
	}

	pthread_mutex_t* getMutexPtr()
	{
		return &mMutex;
	}

private:
	pthread_mutex_t mMutex;
	volatile bool isLocked;
};


#endif /* BASE_MUTEX_H_ */
