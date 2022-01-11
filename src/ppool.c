#include "poule/ppool.h"

static void *
manager_routine(void *arg)
{
    pl_ppool_t *pool = arg;
    return NULL;
}

int
pl_ppool_init(pl_ppool_t *pool, size_t workers_num)
{
    pl_queue_init(&pool->queue_work);
    pl_queue_init(&pool->queue_done);
    pool->pids_len = workers_num;
    pool->pids = malloc(sizeof(pid_t) * workers_num);
    if (pool->pids == NULL)
        return -1;
    if (pthread_create(&pool->manager, NULL, manager_routine, pool) != 0)
    {
        free(pool->pids);
        return -1;
    }
    return 0;
}

int
pl_ppool_shutdown(pl_ppool_t *pool)
{
    for (size_t i = 0; i < pool->pids_len; i++)
        kill(pool->pids[i], SIGTERM);
    free(pool->pids);
    pl_queue_deinit(&pool->queue_work, free);
    pl_queue_deinit(&pool->queue_done, free);
    return 1;
}
