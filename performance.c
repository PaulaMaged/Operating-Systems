#include <stdio.h>
#include <time.h>
#include <unistd.h>

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int main(void)
{
    struct timespec start, finish, delta;
    clock_gettime(CLOCK_REALTIME, &start);
    // sleep(2);
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
    printf("%d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);
    return 0;
}