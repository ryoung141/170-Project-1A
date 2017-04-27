#include "rwlock.h"

RWLock:: RWLock(){
#ifdef RWLOCK
  pthread_mutex_init(&read_lock, NULL);
  pthread_mutex_init(&write_lock, NULL);
#else
  pthread_mutex_init(&lock, NULL);
#endif
}

RWLock:: ~RWLock(){
#ifdef RWLOCK
  pthread_mutex_destroy(&read_lock);
  pthread_mutex_destroy(&write_lock);
#else
  pthread_mutex_destroy(&lock);
#endif
}

void
RWLock:: startRead(){
#ifdef RWLOCK
  pthread_mutex_lock(&read_lock);
#else
  pthread_mutex_lock(&lock);
#endif
}

void
RWLock:: doneRead(){
#ifdef RWLOCK
  pthread_mutex_unlock(&read_lock);
#else
  pthread_mutex_unlock(&lock); 
#endif
}

void
RWLock:: startWrite(){
#ifdef RWLOCK
  pthread_mutex_lock(&write_lock);
#else
  pthread_mutex_lock(&lock);
#endif
}

void
RWLock:: doneWrite(){
#ifdef RWLOCK
  pthread_mutex_unlock(&write_lock);
#else
  pthread_mutex_unlock(&lock);
#endif
}

