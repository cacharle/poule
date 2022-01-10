#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// double frand(void)
// {
//     return (double)random() / (double)RAND_MAX;
// }


void *point_in_circle(void *_)
{
    double x = drand48();
    double y = drand48();
    bool is_in = sqrt(x * x + y * y) < 0.5;
    return (void*)is_in;
}

const char *rand_filepath = "/dev/random";
const size_t worker_count = 10;
const size_t points_count = 100;

int main(void)
{
    FILE *file = fopen(rand_filepath, "r");
    if (file == NULL)
        exit(1);
    long seed;
    fread(&seed, 1, sizeof(seed), file);
    fclose(file);
    // srandom(seed);
    srand48(seed);

    pool_t pool;
    pool_init(&pool, worker_count, point_in_circle);

    for (int i = 0; i < points_count; i++)
        submit


    pool_drain();

    return 0;
}
