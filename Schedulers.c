#include <pthread.h>
#include<stdio.h>
#include <errno.h>

int t1Set = -1;

void* threadMethod(void* arg) {
    t1Set = 1;
    return NULL;
}

int main() {
    
    pthread_attr_t tattr;
    pthread_t t1;
    int policy;
    int schedpolicyInherit;
    struct sched_param priority;
    pthread_attr_init(&tattr);

    pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
    pthread_attr_getschedpolicy(&tattr, &policy);
    printf("The schedpolicy value is: %d\n", policy);

    //before setting inheritSchedPolicy 
    pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);
    printf("The schedpolicy value is: %d\n", policy);

    pthread_attr_setinheritsched(&tattr, PTHREAD_INHERIT_SCHED);

    //After setting inheritSchedPolicy 
    pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);
    printf("The schedpolicy value is: %d\n", policy);

    //get schedparam
    pthread_attr_getschedparam(&tattr, &priority);

    printf("The scheduling priority of thread is: %d\n", priority.sched_priority);

    //Change main thread scheduling priority
    pthread_t mainThread = pthread_self();
    int mainPolicy;
    struct sched_param mainPriority;
    pthread_getschedparam(mainThread, &mainPolicy, &mainPriority);
    printf("The mainThread has\n" 
            "ID      : %lu\n"
            "policy  : %d\n"
            "priority: %d\n",
             mainThread, mainPolicy, mainPriority.sched_priority);

    //checking for privileges of setting a thread's scheduling policy
    int ret = pthread_create(&t1, &tattr, threadMethod, NULL);
    fprintf(stderr, "t1 creation return value: %d\n", ret);
    ret = pthread_join(t1, NULL);
    printf("t1 join return value: %d\n", ret);

    printf("t1Set is: %d\n", t1Set);

//EINVAL Invalid settings in attr. 22
//EPERM  No permission to set the scheduling policy and parameters specified in attr. 1
//EAGAIN Insufficient resources to create another thread. 11
}