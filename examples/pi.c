#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "poule/pool.h"

void *point_in_circle(void *_)
{
    double x = drand48() - 0.5;
    double y = drand48() - 0.5;
    bool is_in = x * x + y * y < 0.25;
    return (void*)is_in;
}

const char *rand_filepath = "/dev/random";
const size_t points_count = 1000000;

int main(void)
{
    const size_t worker_count = sysconf(_SC_NPROCESSORS_ONLN);
    FILE *file = fopen(rand_filepath, "r");
    if (file == NULL)
        exit(1);
    long seed;
    fread(&seed, 1, sizeof(seed), file);
    fclose(file);
    srand48(seed);

    pl_pool_t pool;
    pl_pool_init(&pool, worker_count, point_in_circle);

    for (int i = 0; i < points_count; i++)
        pl_pool_submit(&pool, NULL);


    void **datum = pl_pool_drain(&pool);
    size_t points_inside_count = 0;
    for (size_t i = 0; i < points_count; i++)
        if ((bool)datum[i])
            points_inside_count++;

    double pi_approx = 4.0 * ((double)points_inside_count / (double)points_count);
    printf("pi approx: %f\n", pi_approx);
    printf("pi       : %f\n", M_PI);
    printf("diff     : %f\n", fabs(M_PI - pi_approx));

    pl_pool_shutdown(&pool);
    return 0;
}
