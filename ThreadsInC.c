#define _GNU_SOURCE
#define _POSIX_C_SOURCE 199309L
// Importing the Pthread Library
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


const long NANOSECONDS_IN_SECOND = 1000000000;
struct threadMetrics *threads[4];

struct threadMetrics {
    pthread_t tid;
    double burstTime;
    double tat;
};

double timespecToDouble(struct timespec start, struct timespec end) {
    double startnsToSec = (double) start.tv_nsec / NANOSECONDS_IN_SECOND;
    double endnsToSec = (double) end.tv_nsec / NANOSECONDS_IN_SECOND;
    double startSec = start.tv_sec + startnsToSec;
    double endSec = end.tv_sec + endnsToSec;
    double timeTaken = endSec - startSec;
    return timeTaken;
}

void * ThreadFunction(void *arguments)
{
    int dummy = 0;
    struct timespec start, end, burstStart, burstEnd, testStart, testEnd;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &burstStart); //for calculating burst time
    for(int i = 1; i <= NANOSECONDS_IN_SECOND; i++) {
        dummy++;
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &burstEnd); //for calculating burst time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    //return results
    struct threadMetrics *currentThreadPointer = ((struct threadMetrics*) arguments);
    pthread_t tid = currentThreadPointer -> tid;
    currentThreadPointer-> burstTime = timespecToDouble(burstStart, burstEnd);
    currentThreadPointer-> tat = timespecToDouble(start, end);;
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

void displayThreadMetrics() {
    printf("Thread ID\t\tTurn around time\t\tBurst time\n\n");
    int threadsLength = sizeof(threads) / sizeof(threads[0]);
    double turnAroundTimeAvg = 0;
    double burstTimeAvg = 0;
    for(int i = 0; i < threadsLength; i++) {
        struct threadMetrics thread = *threads[i];
        printf("%lu\t\t", thread.tid);
        printf("%.4lf\t\t", thread.tat);
        printf("%.4lf\n\n", thread.burstTime);
        turnAroundTimeAvg += thread.tat;
        burstTimeAvg += thread.burstTime;
    }
    turnAroundTimeAvg /= threadsLength;
    burstTimeAvg /= threadsLength;
    printf("Average Turn Around Time: %lf\n"
            "Average Burst Time: %lf\n", turnAroundTimeAvg, burstTimeAvg);
}

void printAffinity(cpu_set_t *mask) {
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    printf("sched_getaffinity = ");
    for(int i = 0; i < nproc; i++) {
        printf("%d ", CPU_ISSET(i, mask));
    }
    printf("\n");
}

void setAffinity() {
    int result = 0;
    cpu_set_t  mask;

    result = sched_getaffinity(0, sizeof(cpu_set_t), &mask);
    if(result != 0) {
        fprintf(stderr, "Failed to set affinity: %s", strerror(errno));
    }
    printAffinity(&mask);
    printf("sched_getcpu = %d\n", sched_getcpu());

    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    result = sched_setaffinity(0, sizeof(mask), &mask);
    if(result != 0) {
        fprintf(stderr, "Failed to set affinity: %s", strerror(errno));
    }

    result = sched_getaffinity(0, sizeof(cpu_set_t), &mask);
    if(result != 0) {
        fprintf(stderr, "Failed to set affinity: %s", strerror(errno));
    }
    printAffinity(&mask);
    printf("sched_getcpu = %d\n", sched_getcpu());
}

int main() {

    setAffinity(); //set to 1 core
    //setup column headers

    //setting attr features of pthread
    struct sched_param schedParam = 
        {
            .sched_priority = 0,
        };
    pthread_attr_t tattr;
    createTattr(&tattr, SCHED_OTHER, PTHREAD_EXPLICIT_SCHED, &schedParam);

    int ret;
    // Creating the threads
    struct threadMetrics thread1;
    threads[0] = &thread1;
    pthread_t tid1;
    ret = pthread_create(&tid1, &tattr, ThreadFunction, (void *) &thread1);
    if(ret != 0) {
        printf("pthread1 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    thread1.tid = tid1;

    struct threadMetrics thread2;
    threads[1] = &thread2;
    pthread_t tid2;
    ret = pthread_create(&tid2, &tattr, ThreadFunction, (void *) &thread2);
    if(ret != 0) {
        printf("pthread2 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    thread2.tid = tid2;

    struct threadMetrics thread3;
    threads[2] = &thread3;
    pthread_t tid3;
    ret = pthread_create(&tid3, &tattr, ThreadFunction, (void *) &thread3);
    if(ret != 0) {
        printf("pthread3 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    thread3.tid = tid3;

    struct threadMetrics thread4;
    threads[3] = &thread4;
    pthread_t tid4;
    ret = pthread_create(&tid4, &tattr, ThreadFunction, (void *) &thread4);
    if(ret != 0) {
        printf("pthread4 creation: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    thread4.tid = tid4;

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
    displayThreadMetrics();
    return EXIT_SUCCESS;
}