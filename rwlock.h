//#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#ifndef RWLOCK_H
#define RWLOCK_H

class RWLock{
private:
#ifdef RWLOCK
  
  pthread_cond_t read;
  pthread_cond_t write;
  
  int reading;
  int read_wait;
  int writing;
  int write_wait;
  
#endif
  pthread_mutex_t lock;
  
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
