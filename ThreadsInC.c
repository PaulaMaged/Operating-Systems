// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>

struct threadMetrics {
    pthread_t tid;
    struct timespec burstTime;
    double tat;
};

void * ThreadFunction(void *arguments)
{
    int dummy = 0;
    struct timespec burst;
    double timeStart = (double) clock();
    for(int i = 1; i <= 100; i++) {
        dummy++;
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &burst); //for calculating burst time
    double timeEnd = (double) clock(); //for calculating tat time

    //calculations
    double timeDiff = timeEnd - timeStart; 
    double timeTaken = timeDiff / CLOCKS_PER_SEC;

    //return results
    struct threadMetrics currentThread = *((struct threadMetrics*) arguments);
    currentThread.burstTime = burst;
    currentThread.tat = timeTaken;

    return NULL;
}
void createTattr(pthread_attr_t *tattr, int policy, int inherit, struct sched_param *schedParam) {
    
    int ret = 0;
    ret = pthread_attr_init(tattr);
    if(ret != 0) {
        printf("pthread Initialization: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret = pthread_attr_setschedpolicy(tattr, policy);
    if(ret != 0) {
        printf("pthread schedule policy: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret = pthread_attr_setinheritsched(tattr, inherit);
    if(ret != 0) {
        printf("pthread inherit scheduling: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    } 
    ret = pthread_attr_setschedparam(tattr, schedParam);
    if(ret != 0) {
        printf("Setting scheduling priority failure: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
}

void displayThreadMetrics(struct threadMetrics *tM) {
    struct threadMetrics thread = *tM;
    printf("%lu\t", thread.tid);
    printf("%d.%.9ld\t", (int)thread.burstTime.tv_sec, thread.burstTime.tv_nsec);
    printf("%lf\n\n", thread.tat);
}

int main() {
    //setup column headers
    printf("Thread ID\tburst time\tTurn around time\n\n");

    //setting attr features of pthread
    struct sched_param schedParam = 
        {
            .sched_priority = 1,
        };
    pthread_attr_t tattr;
    createTattr(&tattr, SCHED_FIFO, PTHREAD_EXPLICIT_SCHED, &schedParam);

    int ret;
    // Creating the threads
    struct threadMetrics thread1;
    pthread_t tid1;
    thread1.tid = tid1;
    ret = pthread_create(&tid1, &tattr, ThreadFunction, (void *) &thread1);
    if(ret != 0) {
        printf("pthread1 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    struct threadMetrics thread2;
    pthread_t tid2;
    thread2.tid = tid2;
    ret = pthread_create(&tid2, &tattr, ThreadFunction, (void *) &thread2);
    if(ret != 0) {
        printf("pthread2 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    struct threadMetrics thread3;
    pthread_t tid3;
    thread3.tid = tid3;
    ret = pthread_create(&tid3, &tattr, ThreadFunction, (void *) &thread3);
    if(ret != 0) {
        printf("pthread3 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    struct threadMetrics thread4;
    pthread_t tid4;
    thread4.tid = tid4;
    ret = pthread_create(&tid4, &tattr, ThreadFunction, (void *) &thread4);
    if(ret != 0) {
        printf("pthread4 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    // waiting for threads to finish their functions before ending Main thread
    ret = pthread_join(tid1, NULL);
    if(ret != 0){
        printf("join thread 1 failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret = pthread_join(tid2, NULL);
    if(ret != 0){
        printf("join thread 2 failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret = pthread_join(tid3, NULL);
    if(ret != 0){
        printf("join thread 3 failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    ret = pthread_join(tid4, NULL);
    if(ret != 0){
        printf("join thread 4 failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    //results
    displayThreadMetrics(&thread1);
    displayThreadMetrics(&thread2);
    displayThreadMetrics(&thread3);
    displayThreadMetrics(&thread4);

    return EXIT_SUCCESS;
}