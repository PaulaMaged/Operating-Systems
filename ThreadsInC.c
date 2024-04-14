// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>

void * ThreadFunction(void *arguments)
{
  pthread_t tid = pthread_self();
  int policy;
  struct sched_param priority;

  for(int i = 1; i <= 5; i++) {
    pthread_getschedparam(tid, &policy, &priority);
    printf("%lu Thread Running for the %d/5\tPriority is: %d\tPolicy is: %d\n", tid, i, priority.sched_priority, policy);
  }
  return NULL;
}

int main() {

  int ret;
// variables that will hold the ID of the threads created
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  pthread_t thread4;

  //using attr features of pthread
  pthread_attr_t tattr;
  struct sched_param schedParam = 
  {
    .sched_priority = 1,
  };

  ret = pthread_attr_init(&tattr);
  if(ret != 0) {
    printf("pthread Initialization: %d\n", ret);
    exit(EXIT_FAILURE);
  }
  ret = pthread_attr_setschedpolicy(&tattr, SCHED_OTHER);
  if(ret != 0) {
    printf("pthread schedule policy: %d\n", ret);
    exit(EXIT_FAILURE);
  }
  ret = pthread_attr_setinheritsched(&tattr, PTHREAD_INHERIT_SCHED);
  if(ret != 0) {
    printf("pthread inherit scheduling: %d\n", ret);
    exit(EXIT_FAILURE);
  } 
  ret = pthread_attr_setschedparam(&tattr, &schedParam);
  if(ret != 0) {
    printf("Setting scheduling priority failure: %d\n", ret);
    exit(EXIT_FAILURE);
  }

  printf("before creating threads\n");

  // Creating the threads 
  ret = pthread_create(&thread1, &tattr, ThreadFunction, NULL);
  if(ret != 0) {
    printf("pthread1 creation: %d\n", ret);
    exit(EXIT_FAILURE);
  }
    printf("Newly created thread: %lu\n", thread1);
  ret = pthread_create(&thread2, &tattr, ThreadFunction, NULL);
  if(ret != 0) {
    printf("pthread2 creation: %d\n", ret);
    exit(EXIT_FAILURE);
  }
    printf("Newly created thread: %lu\n", thread2);
  ret = pthread_create(&thread3, &tattr, ThreadFunction, NULL);
  if(ret != 0) {
    printf("pthread3 creation: %d\n", ret);
    exit(EXIT_FAILURE);
  }
    printf("Newly created thread: %lu\n", thread3);
  ret = pthread_create(&thread4, &tattr, ThreadFunction, NULL);
  if(ret != 0) {
    printf("pthread4 creation: %d\n", ret);
    exit(EXIT_FAILURE);
  }
    printf("Newly created thread: %lu\n", thread4);

// waiting for threads to finish their functions before ending Main thread
  ret = pthread_join(thread1, NULL);
  if(ret != 0){
    printf("join thread 1 failed: %d\n", ret);
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread2, NULL);
  if(ret != 0){
    printf("join thread 2 failed: %d\n", ret);
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread3, NULL);
  if(ret != 0){
    printf("join thread 3 failed: %d\n", ret);
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread4, NULL);
  if(ret != 0){
    printf("join thread 4 failed: %d\n", ret);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}