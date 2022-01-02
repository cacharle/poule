#include "pool.h"

void *
pool_worker_routine(void *arg)
{
    // printf("hello thread %p\n", pthread_self());
    pool_t *pool = arg;
    while (true)
    {
        // usleep(100);
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
        // printf("h1 %p %p\n", pthread_self(), &result->mutex_finish);
        pthread_mutex_unlock(&result->mutex_finish);
        // printf("h2 %p %p\n", pthread_self(), &result->mutex_finish);
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
    pool->threads_len = workers_num;
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
    pool->shutdown = true;
    for (size_t i = 0; i < pool->threads_len; i++)
        pthread_join(pool->threads[i], NULL);
    queue_deinit(&pool->queue_work, free);
    queue_deinit(&pool->queue_done, free);
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

int
pool_map(pool_t *pool, void **src, void **dest, size_t len)
{
    pool_result_t **results = malloc(sizeof(pool_result_t *) * len);
    if (results == NULL)
        return -1;
    for (size_t i = 0; i < len; i++)
    {
        results[i] = pool_submit(pool, src[i]);
        if (results[i] == NULL)
        {
            free(results);
            return -1;
        }
    }
    for (size_t i = 0; i < len; i++)
        dest[i] = pool_result_wait(results[i]);
    return 0;
}

void *
pool_result_wait(pool_result_t *result)
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
