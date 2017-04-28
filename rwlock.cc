#include "rwlock.h"

RWLock:: RWLock(){
#ifdef RWLOCK
  reading = 0;
  read_wait = 0;
  writing = 0;
  write_wait = 0;
  
  pthread_cond_init(&read, NULL);
  pthread_cond_init(&write, NULL);
#endif
  pthread_mutex_init(&lock, NULL);
}

RWLock:: ~RWLock(){
#ifdef RWLOCK
  pthread_cond_destroy(&read);
  pthread_cond_destroy(&write);
#endif
  pthread_mutex_destroy(&lock);
}

void
RWLock:: startRead(){
  pthread_mutex_lock(&lock);
#ifdef RWLOCK
  read_wait++;
  while(write_wait!=0 || writing!=0){
    pthread_cond_wait(&read, &lock);
  }
  //once nobody is planning on writing, read!

  read_wait--;
  reading++;
  pthread_mutex_unlock(&lock);
#endif
}

void
RWLock:: doneRead(){

#ifdef RWLOCK
    pthread_mutex_lock(&lock);
    reading--;
    if(reading == 0 && write_wait > 0){
      pthread_cond_signal(&write);
    }
#endif
    pthread_mutex_unlock(&lock);
}

void
RWLock:: startWrite(){
  pthread_mutex_lock(&lock);
#ifdef RWLOCK
  write_wait++;
  while(writing!=0 || reading!=0){
    pthread_cond_wait(&write, &lock);
  }
  //once nobody is actively reading or writing, ok if people waiting to read
  //write has priority
  
  write_wait--;
  writing++;
  pthread_mutex_unlock(&lock);
#endif
}

void
RWLock:: doneWrite(){

#ifdef RWLOCK
  pthread_mutex_lock(&lock);
  writing--;
  if(writing == 0 && write_wait > 0){
    pthread_cond_signal(&write);
  }
  else{ //no writers waiting --> readers get signaled
    pthread_cond_signal(&read);
  }
#endif
  pthread_mutex_unlock(&lock);

}

