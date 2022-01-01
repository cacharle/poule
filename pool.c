#include "pool.h"

void *pool_job_routine(void *arg)
{
    pool_t *pool = arg;
    while (true)
    {
        void *data = NULL;
        pthread_mutex_lock(&pool->mutex_queue);
        if (!queue_empty(&pool->queue))
            data = queue_pop(&pool->queue);
        pthread_mutex_unlock(&pool->mutex_queue);
        if (data == NULL)
            continue;

    }
    return NULL;
}

int pool_init(pool_t *pool, size_t jobs_num, pool_func func)
{
    queue_init(&pool->queue);
    pool->func = func;
    pool->threads_num = jobs_num;
    pthread_mutex_init(&pool->mutex_queue, NULL);
    pool->threads = malloc(sizeof(pthread_t) * jobs_num);
    if (pool->threads == NULL)
        return -1;
    for (size_t i = 0; i < jobs_num; i++)
    {
        if (pthread_create(&pool->threads[i], NULL, pool_job_routine, NULL) != 0)
            return -1;
    }

    return 0;
}

int pool_close(pool_t *pool)
{
    if (!queue_empty(&pool->queue))
        return -1;
    for (size_t i = 0; i < pool->threads_num; i++)
        pthread_join(pool->threads[i], NULL);
    pthread_mutex_destroy(&pool->mutex_queue);
    free(pool->threads);
    return 0;
}

// pool_result_t *pool_submit(void *data)
// {
//
// }
