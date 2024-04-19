// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>

int dummy = 0;
struct timespec thread1Time;
struct timespec thread2Time;
struct timespec thread3Time;
struct timespec thread4Time;

void display(struct timespec timeSpec, int num_id) {
    printf("Thread%d took %d.%.9ld\n", num_id, (int)timeSpec.tv_sec, timeSpec.tv_nsec);
}

void * ThreadFunction(void *arguments)
{
  pthread_t tid = pthread_self();

  clock_gettime(CLOCK_THREAD_CPUTIME_ID, arguments);
  for(int i = 1; i <= 5; i++) {
    // printf("%lu Thread Running for the %d/5\n", tid, i);
    dummy++;
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
    .sched_priority = 0,
  };

  ret = pthread_attr_init(&tattr);
  if(ret != 0) {
    printf("pthread Initialization: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  ret = pthread_attr_setschedpolicy(&tattr, SCHED_OTHER);
  if(ret != 0) {
    printf("pthread schedule policy: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
  if(ret != 0) {
    printf("pthread inherit scheduling: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  } 
  ret = pthread_attr_setschedparam(&tattr, &schedParam);
  if(ret != 0) {
    printf("Setting scheduling priority failure: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  // printf("before creating threads\n");

  // Creating the threads 
  ret = pthread_create(&thread1, &tattr, ThreadFunction, (void *) &thread1Time);
  if(ret != 0) {
    printf("pthread1 creation: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  ret = pthread_create(&thread2, &tattr, ThreadFunction, (void *) &thread2Time);
  if(ret != 0) {
    printf("pthread2 creation: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  ret = pthread_create(&thread3, &tattr, ThreadFunction, (void *) &thread3Time);
  if(ret != 0) {
    printf("pthread3 creation: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  ret = pthread_create(&thread4, &tattr, ThreadFunction, (void *) &thread4Time);
  if(ret != 0) {
    printf("pthread4 creation: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

// waiting for threads to finish their functions before ending Main thread
  ret = pthread_join(thread1, NULL);
  if(ret != 0){
    printf("join thread 1 failed: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread2, NULL);
  if(ret != 0){
    printf("join thread 2 failed: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread3, NULL);
  if(ret != 0){
    printf("join thread 3 failed: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  ret = pthread_join(thread4, NULL);
  if(ret != 0){
    printf("join thread 4 failed: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  display(thread1Time, 1);
  display(thread2Time, 2);
  display(thread3Time, 3);
  display(thread4Time, 4);

  return EXIT_SUCCESS;
}