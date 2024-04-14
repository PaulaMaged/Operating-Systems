#include <pthread.h>
#include<stdio.h>

int main() {
    
    pthread_attr_t tattr;
    pthread_t t1;
    int policy;
    int schedpolicyInherit;
    pthread_attr_init(&tattr);

    pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
    pthread_attr_getschedpolicy(&tattr, &policy);
    printf("The schedpolicy value is: %d\n", policy);

    //before setting inheritSchedPolicy 
    pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);
    printf("The schedpolicy value is: %d\n", policy);

    pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);

    //After setting inheritSchedPolicy 
    pthread_attr_getinheritsched(&tattr, &schedpolicyInherit);
    printf("The inheritedSchedPolicy is: %d\n", schedpolicyInherit);
    printf("The schedpolicy value is: %d\n", policy);
}