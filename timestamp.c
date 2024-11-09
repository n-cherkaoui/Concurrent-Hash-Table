#include <sys/time.h>
#include <stddef.h>
long long get_microsecond_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}