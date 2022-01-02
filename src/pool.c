#include "pool.h"

void *
pool_worker_routine(void *arg)
{
    pool_t *pool = arg;
    while (true)
    {
        pool_result_t *result = queue_pop(&pool->queue_work);
        if (result == NULL)
        {
            if (pool->shutdown)
                return NULL;
            continue;
        }
        result->state = RESULT_RUNNING;
        result->data = pool->func(result->data);
        result->state = RESULT_DONE;
        queue_push(&pool->queue_done, result);
        pthread_mutex_unlock(&result->mutex_finish);
    }
    return NULL;
}

int
pool_init(pool_t *pool, size_t workers_num, pool_func func)
{
    queue_init(&pool->queue_work);
    queue_init(&pool->queue_done);
    pool->func = func;
    pool->shutdown = false;
    pool->threads_num = workers_num;
    // pthread_mutex_init(&pool->mutex_queue, NULL);
    pool->threads = malloc(sizeof(pthread_t) * workers_num);
    if (pool->threads == NULL)
        return -1;
    for (size_t i = 0; i < workers_num; i++)
    {
        if (pthread_create(&pool->threads[i], NULL, pool_worker_routine, pool) != 0)
            return -1;
    }
    return 0;
}

int
pool_shutdown(pool_t *pool)
{
    if (!queue_empty(&pool->queue_work))
        return -1;
    for (size_t i = 0; i < pool->threads_num; i++)
        pthread_join(pool->threads[i], NULL);
    queue_deinit(&pool->queue_work);
    queue_deinit(&pool->queue_done);
    free(pool->threads);
    return 0;
}

static pool_result_t *
_result_create(void *data)
{
    pool_result_t *result = malloc(sizeof(pool_result_t));
    if (result == NULL)
        return (NULL);
    result->state = RESULT_PENDING;
    result->data = data;
    pthread_mutex_init(&result->mutex_finish, NULL);
    pthread_mutex_lock(&result->mutex_finish);
    return result;
}

pool_result_t *
pool_submit(pool_t *pool, void *data)
{
    pool_result_t *result = _result_create(data);
    if (result == NULL)
        return NULL;
    if (queue_push(&pool->queue_work, result) == NULL)
    {
        free(result);
        return NULL;
    }
    return result;
}

void *
pool_result_wait(pool_result_t *result)
{
    pthread_mutex_lock(&result->mutex_finish);
    switch (result->state)
    {
    case RESULT_CANCELLED:
        return NULL;
    case RESULT_DONE:
        return result->data;
    default:
        abort();
    }
}
