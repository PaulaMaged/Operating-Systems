// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>

void * ThreadFunction(void *arguments)
{
  pthread_t tid = pthread_self();
  int policy;
  struct sched_param priority;

  for(int i = 1; i <= 5; i++) {
    pthread_getschedparam(tid, &policy, &priority);
    printf("%lu Thread Running for the %d/5\tThe thread's priority is: %d\n", tid, i, priority.sched_priority);
  }
  return NULL;
}

int main() {

// variables that will hold the ID of the threads created
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  pthread_t thread4;

  //using attr features of pthread
  pthread_attr_t tattr;
  pthread_attr_init(&tattr);
  pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
  // pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
  printf("before creating threads\n");

  // Creating the threads 
  pthread_create(&thread1, &tattr, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread1);
  pthread_create(&thread2, &tattr, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread2);
  pthread_create(&thread3, &tattr, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread3);
  pthread_create(&thread4, &tattr, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread4);

// waiting for threads to finish their functions before ending Main thread
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);


  return 0;
}