#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdio.h>
#include "poule/queue.h"
#include "poule/pool.h"

void *
square(void *data)
{
    unsigned long i = (unsigned long)data;
    return (void*)(i * i);
}

#define SQUARE_ARRAY_LEN 3000

int
main(void)
{
    pl_pool_t pool;
    pl_pool_init(&pool, 10, square);
    void *src[SQUARE_ARRAY_LEN] = {0};
    void *dest[SQUARE_ARRAY_LEN] = {0};
    for (size_t i = 0; i < SQUARE_ARRAY_LEN; i++)
        src[i] = (void*)(i + 1);
    pl_pool_map(&pool, src, dest, SQUARE_ARRAY_LEN);
    for (int i = 0; i < SQUARE_ARRAY_LEN; i++)
    {
        printf("%3lu -> %5lu", (unsigned long)src[i], (unsigned long)dest[i]);
        printf("\n");
    }
    pl_pool_shutdown(&pool);
    return 0;
}
