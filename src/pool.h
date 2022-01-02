#ifndef POULE_POOL_H
#define POULE_POOL_H

#include "queue.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef void *(*pool_func)(void *data);

typedef struct
{
    queue_t         queue;
    pthread_t      *threads;
    size_t          threads_num;
    pool_func       func;
    pthread_mutex_t mutex_queue;
} pool_t;

typedef struct
{
    // waitable
} pool_result_t;

#endif
