#ifndef POULE_POOL_H
#define POULE_POOL_H

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "queue.h"

typedef void *(*pool_func)(void *data);

typedef struct
{
    queue_t    queue_work;
    queue_t    queue_done;
    pthread_t *threads;
    size_t     threads_len;
    pool_func  func;
    bool       shutdown;
} pool_t;

enum pool_result_state
{
    RESULT_PENDING,
    RESULT_RUNNING,
    RESULT_DONE,
    RESULT_CANCELLED,
};

typedef struct
{
    void                  *data;
    enum pool_result_state state;
    pthread_mutex_t        mutex_finish;
} pool_result_t;

int
pool_init(pool_t *pool, size_t workers_num, pool_func func);
int
pool_shutdown(pool_t *pool);
pool_result_t *
pool_submit(pool_t *pool, void *data);
int
pool_map(pool_t *pool, void **src, void **dest, size_t len);
void *
pool_result_wait(pool_result_t *result);

#endif
