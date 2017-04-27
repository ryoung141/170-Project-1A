//#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#ifndef RWLOCK_H
#define RWLOCK_H

class RWLock{
private:
#ifdef RWLOCK
  pthread_mutex_t read_lock;
  pthread_mutex_t write_lock;
#else 
  pthread_mutex_t lock;; 
#endif
 
public:
    	RWLock();
    	~RWLock();
    //Reader
    	void startRead();
    	void doneRead();
    // Writer
    	void startWrite();
    	void  doneWrite();
};

#endif
