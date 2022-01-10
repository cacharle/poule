#include "poule/pool.h"

static void *
worker_routine(void *arg)
{
    // printf("hello thread %p\n", pthread_self());
    pl_pool_t *pool = arg;
    while (true)
    {
        // usleep(100);
        pl_result_t *result = pl_queue_pop(&pool->queue_work);
        if (result == NULL)
        {
            if (pool->shutdown)
                return NULL;
            continue;
        }
        result->state = RESULT_RUNNING;
        result->data = pool->func(result->data);
        result->state = RESULT_DONE;
        pl_queue_push(&pool->queue_done, result);
        // printf("h1 %p %p\n", pthread_self(), &result->mutex_finish);
        pthread_mutex_unlock(&result->mutex_finish);
        // printf("h2 %p %p\n", pthread_self(), &result->mutex_finish);
    }
    return NULL;
}

int
pl_pool_init(pl_pool_t *pool, size_t workers_num, pool_func func)
{
    pl_queue_init(&pool->queue_work);
    pl_queue_init(&pool->queue_done);
    pool->func = func;
    pool->shutdown = false;
    pool->threads_len = workers_num;
    // pthread_mutex_init(&pool->mutex_queue, NULL);
    pool->threads = malloc(sizeof(pthread_t) * workers_num);
    if (pool->threads == NULL)
        return -1;
    for (size_t i = 0; i < workers_num; i++)
    {
        if (pthread_create(&pool->threads[i], NULL, worker_routine, pool) != 0)
            return -1;
    }
    return 0;
}

int
pl_pool_shutdown(pl_pool_t *pool)
{
    pool->shutdown = true;
    for (size_t i = 0; i < pool->threads_len; i++)
        pthread_join(pool->threads[i], NULL);
    pl_queue_deinit(&pool->queue_work, free);
    pl_queue_deinit(&pool->queue_done, free);
    free(pool->threads);
    return 0;
}

static pl_result_t *
_result_create(void *data)
{
    pl_result_t *result = malloc(sizeof(pl_result_t));
    if (result == NULL)
        return (NULL);
    result->state = RESULT_PENDING;
    result->data = data;
    pthread_mutex_init(&result->mutex_finish, NULL);
    pthread_mutex_lock(&result->mutex_finish);
    return result;
}

pl_result_t *
pl_pool_submit(pl_pool_t *pool, void *data)
{
    pl_result_t *result = _result_create(data);
    if (result == NULL)
        return NULL;
    if (pl_queue_push(&pool->queue_work, result) == NULL)
    {
        free(result);
        return NULL;
    }
    return result;
}

int
pl_pool_map(pl_pool_t *pool, void **src, void **dest, size_t len)
{
    pl_result_t **results = malloc(sizeof(pl_result_t *) * len);
    if (results == NULL)
        return -1;
    for (size_t i = 0; i < len; i++)
    {
        results[i] = pl_pool_submit(pool, src[i]);
        if (results[i] == NULL)
        {
            free(results);
            return -1;
        }
    }
    for (size_t i = 0; i < len; i++)
        dest[i] = pl_result_wait(results[i]);
    return 0;
}

void **
pl_pool_drain(pl_pool_t *pool)
{
    while (!pl_queue_empty(&pool->queue_work))
        ;
    size_t datum_len = pl_queue_len(&pool->queue_done);
    void **datum = malloc(sizeof(void *) * datum_len);
    if (datum == NULL)
        return NULL;
    for (size_t i = 0; i < datum_len; i++)
    {
        pl_result_t *result = pl_queue_pop(&pool->queue_done);
        datum[i] = result->data;
        free(result);
    }
    return datum;
}

void *
pl_result_wait(pl_result_t *result)
{
    // printf("YOOOOOOOOOOOOOOOO %p\n", &result->mutex_finish);
    pthread_mutex_lock(&result->mutex_finish);
    // printf("YOOOOOOOOOOOOOOOO %p\n", &result->mutex_finish);
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
