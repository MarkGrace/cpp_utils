/*
 * Condition.h
 *
 *  Created on: 2015年4月7日
 *      Author: mark
 */

#ifndef THREADPOOL_CONDITION_H_
#define THREADPOOL_CONDITION_H_

#include <pthread.h>

class Condition
{
	Condition() { pthread_cond_init(&mCondition, NULL); }
	~Condition() { pthread_cond_destroy(&mCondition); }
	void wait(pthread_mutex_t *mutex) { pthread_cond_wait(&mCondition, mutex); }
	void signal() { pthread_cond_signal(&mCondition); }
	void broadcast() { pthread_cond_broadcast(&mCondition); }
private:
	pthread_cond_t mCondition;
};



#endif /* THREADPOOL_CONDITION_H_ */
