
#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
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

void clockGetTime() {
    struct timespec start, end;
    int dummy_data = 0;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(int i = 0; i < 1000000000; i++) {
        dummy_data++; 
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    int dumdum = 0;
    printf("clockGetTime took: some time\n");
}

void useClock() {
    int dummy_data = 0;
    double start = (double) clock();
    for(int i = 0; i < 1000000000; i++) {
        dummy_data++; 
    }
    double end = (double) clock();
    double timeDiff = (end - start) / CLOCKS_PER_SEC;
    printf("clockGetTime took: some time\n");
}

void printAffinity(cpu_set_t *mask) {
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    printf("sched_getaffinity = ");
    for(int i = 0; i < nproc; i++) {
        printf("%d ", CPU_ISSET(i, mask));
    }
    printf("\n");
}

int main() {
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

    clockGetTime();
    useClock();
    return EXIT_SUCCESS;
}