#include <cstdio>
#include <time.h>

int current_time = time(0);
int seed_count = 0;
int seed = current_time * 10000 + seed_count;

int new_seed()
{
    if (time(0) != current_time)
    {
        current_time = time(0);
        seed_count = 0;
        seed = current_time * 10000 + seed_count;
    }
    else
    {
        ++seed_count;
        seed = current_time * 10000 + seed_count;
    }
    return seed;
}