#include <pthread.h>
#include<stdio.h>
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>

int t1Set = -1;

void* threadMethod(void* arg) {
    t1Set = 1;
    return NULL;
}

int main() {

    int ret;
    struct sched_param schedParam = 
    {
        .sched_priority =1,
    };
    //setting main thread to use FIFO
    ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &schedParam);
    if(ret != 0) {
        fprintf(stderr, "setting scheduling param for main thread failed: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }

    pthread_attr_t tattr;
    pthread_t t1;
    int policy;
    int schedpolicyInherit;
    struct sched_param priority;
    ret = pthread_attr_init(&tattr);
    if(ret != 0) {
        fprintf(stderr, "initializing failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }



    ret = pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
    if(ret != 0) {
        fprintf(stderr, "setting schedpolicy failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE); 
    }
    ret = pthread_attr_getschedpolicy(&tattr, &policy);
    if(ret != 0) {
        fprintf(stderr, "getting sched policy failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE); 
    }
    printf("The schedpolicy value is: %d\n", policy);

    //before setting inheritSchedPolicy 
    ret = pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    if(ret != 0) {
        fprintf(stderr, "getting tattr inherited sched failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE); 
    }
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);


    ret = pthread_attr_setinheritsched(&tattr, PTHREAD_INHERIT_SCHED);
    if(ret != 0) {
        fprintf(stderr, "setting tattr inherit sched failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }

    //After setting inheritSchedPolicy 
    ret = pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    if(ret != 0) {
        fprintf(stderr, ": %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);

    //get schedparam
    ret = pthread_attr_getschedparam(&tattr, &priority);
    if(ret != 0) {
        fprintf(stderr, "getting tattr schedparam failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }

    printf("The scheduling priority of thread is: %d\n", priority.sched_priority);

    //Change main thread scheduling priority
    pthread_t mainThread = pthread_self();
    int mainPolicy;
    struct sched_param mainPriority;
    ret = pthread_getschedparam(mainThread, &mainPolicy, &mainPriority);
    if(ret != 0) {
        fprintf(stderr, "getting main thread schedparam failed: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }
    printf("The mainThread has\n" 
            "ID      : %lu\n"
            "policy  : %d\n"
            "priority: %d\n",
             mainThread, mainPolicy, mainPriority.sched_priority);

    //checking for privileges of setting a thread's scheduling policy
    ret = pthread_create(&t1, &tattr, threadMethod, NULL);
    fprintf(stderr, "t1 creation failure: %s\n", strerror(ret));

    ret = pthread_getschedparam(t1, &policy, &schedParam);
    if(ret != 0) {
        fprintf(stderr, "t1 schedparam failure: %s\n", strerror(ret)); 
        exit(EXIT_FAILURE);
    }
    printf("t1 has\n"
            "policy: %d\n"
            "priority: %d\n", policy, schedParam.sched_priority);

    ret = pthread_join(t1, NULL);
    if(ret != 0) {
        fprintf(stderr, "thread1 join failed: %s", strerror(ret));
        exit(EXIT_FAILURE);
    }
    printf("t1 join return value: %d\n", ret);

    printf("t1Set is: %d\n", t1Set);

//EINVAL Invalid settings in attr. 22
//EPERM  No permission to set the scheduling policy and parameters specified in attr. 1
//EAGAIN Insufficient resources to create another thread. 11
}