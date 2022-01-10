#ifndef POULE_POOL_H
#define POULE_POOL_H

#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "poule/queue.h"

typedef void *(*pool_func)(void *data);

typedef struct
{
    pl_queue_t queue_work;
    pl_queue_t queue_done;
    pthread_t *threads;
    size_t     threads_len;
    pool_func  func;
    bool       shutdown;
} pl_pool_t;

enum pl_result_state
{
    RESULT_PENDING,
    RESULT_RUNNING,
    RESULT_DONE,
    RESULT_CANCELLED,
};

typedef struct
{
    void                *data;
    enum pl_result_state state;
    pthread_mutex_t      mutex_finish;
} pl_result_t;

int
pl_pool_init(pl_pool_t *pool, size_t workers_num, pool_func func);
int
pl_pool_shutdown(pl_pool_t *pool);
pl_result_t *
pl_pool_submit(pl_pool_t *pool, void *data);
int
pl_pool_map(pl_pool_t *pool, void **src, void **dest, size_t len);
void *
pl_result_wait(pl_result_t *result);
void **
pl_pool_drain(pl_pool_t *pool);

#endif
