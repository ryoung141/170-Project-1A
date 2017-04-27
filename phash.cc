/**
 * Code is based on
 *http://www.algolist.net/Data_structures/Hash_table/Chaining
 *
 **/



#include <iostream>
#include "phash.h"

LinkedHashEntry:: LinkedHashEntry(int key, int value) {
            this->key = key;
            this->value = value;
            this->next = NULL;
      }

int 
LinkedHashEntry:: getKey() {
            return key;
      }
int 
LinkedHashEntry:: getValue() {
            return value;
      }
 
void 
LinkedHashEntry:: setValue(int value) {
            this->value = value;
      }
 

LinkedHashEntry * 
LinkedHashEntry:: getNext() {
            return next;
      }
 
void 
LinkedHashEntry:: setNext(LinkedHashEntry *next) {
            this->next = next;
      }


const int TABLE_SIZE = 128;
 
HashMap::HashMap() {
#ifdef RWLOCK
#else
    pthread_mutex_init(&mutex, NULL);
#endif
            table = new LinkedHashEntry*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++)
                  table[i] = NULL;
      }

int 
HashMap:: get(int key) {
#ifdef RWLOCK
  lock.startRead();
#else
  pthread_mutex_lock(&mutex);
#endif
            int hash = (key % TABLE_SIZE);
            if (table[hash] == NULL){
#ifdef RWLOCK
	      lock.doneRead();
#else
	      pthread_mutex_unlock(&mutex);
#endif
	      return -1;}
            else {
                  LinkedHashEntry *entry = table[hash];
                  while (entry != NULL && entry->getKey() != key)
                        entry = entry->getNext();
                  if (entry == NULL){
#ifdef RWLOCK
	      lock.doneRead();
#else
	      pthread_mutex_unlock(&mutex);
#endif
		    return -1;}
                  else{
#ifdef RWLOCK
	      lock.doneRead();
#else
	      pthread_mutex_unlock(&mutex);
#endif
		    return entry->getValue();}
            }
      }
 
void 
HashMap::put(int key, int value) {
#ifdef RWLOCK
  lock.startWrite();
#else
  pthread_mutex_lock(&mutex);
#endif
            int hash = (key % TABLE_SIZE);
            if (table[hash] == NULL)
                  table[hash] = new LinkedHashEntry(key, value);
            else {
                  LinkedHashEntry *entry = table[hash];
                  while (entry->getNext() != NULL)
                        entry = entry->getNext();
                  if (entry->getKey() == key)
                        entry->setValue(value);
                  else
                        entry->setNext(new LinkedHashEntry(key, value));
            }
#ifdef RWLOCK
	    lock.doneWrite();
#else
	    pthread_mutex_unlock(&mutex);
#endif
      }
 

void
HashMap:: remove(int key) {
#ifdef RWLOCK
#else
  pthread_mutex_lock(&mutex);
#endif
            int hash = (key % TABLE_SIZE);
            if (table[hash] != NULL) {
#ifdef RWLOCK
		lock.startRead();
#endif
                  LinkedHashEntry *prevEntry = NULL;
                  LinkedHashEntry *entry = table[hash];
                  while (entry->getNext() != NULL && entry->getKey() != key) {
                        prevEntry = entry;
                        entry = entry->getNext();
                  }
#ifdef RWLOCK
		  lock.doneRead();
		  lock.startWrite();
#endif
                  if (entry->getKey() == key) {
                        if (prevEntry == NULL) {
                             LinkedHashEntry *nextEntry = entry->getNext();
                             delete entry;
                             table[hash] = nextEntry;
                        } else {
                             LinkedHashEntry *next = entry->getNext();
                              delete entry;
                             prevEntry->setNext(next);
                        }
                  }
#ifdef RWLOCK
		  lock.doneWrite();
#else
		  pthread_mutex_unlock(&mutex);
#endif
            }
      }
 
HashMap:: ~HashMap() {
#ifdef RWLOCK
  lock.startRead();
  lock.startWrite();
#else
  pthread_mutex_lock(&mutex);
#endif
            for (int i = 0; i < TABLE_SIZE; i++)
                  if (table[i] != NULL) {
                        LinkedHashEntry *prevEntry = NULL;
                        LinkedHashEntry *entry = table[i];
                        while (entry != NULL) {
                             prevEntry = entry;
                             entry = entry->getNext();
                             delete prevEntry;
                        }
                  }
            delete[] table;
#ifdef RWLOCK
	    lock.doneRead();
	    lock.doneWrite();
	    delete lock;
#else
	    pthread_mutex_destroy(&mutex);
#endif
      }


