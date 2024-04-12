// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>

void * ThreadFunction(void *arguments)
{
  unsigned long tid = pthread_self();
  for(int i = 1; i <= 5; i++) {
    printf("%lu Thread Running for the %d/5\n", tid, i);
  }
  return NULL;
}

int main() {
  
// variables that will hold the ID of the threads created
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  pthread_t thread4;
  
  printf("before creating threads\n");

  // Creating the threads 
  pthread_create(&thread1, NULL, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread1);
  pthread_create(&thread2, NULL, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread2);
  pthread_create(&thread3, NULL, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread3);
  pthread_create(&thread4, NULL, ThreadFunction, NULL);
    printf("Newly created thread: %lu\n", thread4);

// waiting for threads to finish their functions before ending Main thread
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);


  return 0;
}