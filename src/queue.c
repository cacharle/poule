#include "queue.h"

static _pl_queue_node_t *
node_create(void *data)
{
    _pl_queue_node_t *node = malloc(sizeof(_pl_queue_node_t));
    if (node == NULL)
        return NULL;
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    return node;
}

void
pl_queue_init(pl_queue_t *queue)
{
    queue->start = NULL;
    queue->end = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
}

void
pl_queue_deinit(pl_queue_t *queue, void (*free_func)(void *))
{
    _pl_queue_node_t *node = queue->start;
    while (node != NULL)
    {
        if (free_func != NULL)
            free_func(node->data);
        _pl_queue_node_t *curr = node;
        node = node->next;
        free(curr);
    }
    pthread_mutex_destroy(&queue->mutex);
}

static void *
queue_push_unsafe(pl_queue_t *queue, void *data)
{
    _pl_queue_node_t *node = node_create(data);
    if (node == NULL)
        return NULL;
    queue->len++;
    if (queue->start == NULL)
    {
        queue->start = node;
        queue->end = node;
        return node;
    }
    node->next = queue->start;
    node->next->prev = node;
    queue->start = node;
    return node->data;
}

void *
pl_queue_push(pl_queue_t *queue, void *data)
{
    pthread_mutex_lock(&queue->mutex);
    void *node = queue_push_unsafe(queue, data);
    pthread_mutex_unlock(&queue->mutex);
    return node;
}

static void *
queue_pop_unsafe(pl_queue_t *queue)
{
    if (pl_queue_empty(queue))
        return NULL;
    queue->len--;
    _pl_queue_node_t *node = queue->end;
    if (node->prev == NULL)
    {
        queue->start = NULL;
        queue->end = NULL;
    }
    else
    {
        node->prev->next = NULL;
        queue->end = node->prev;
    }
    void *data = node->data;
    free(node);
    return data;
}

void *
pl_queue_pop(pl_queue_t *queue)
{
    pthread_mutex_lock(&queue->mutex);
    void *data = queue_pop_unsafe(queue);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

size_t
pl_queue_len(pl_queue_t *queue)
{
    return queue->len;
}

bool
pl_queue_empty(pl_queue_t *queue)
{
    return pl_queue_len(queue) == 0;
}

void
_debug_pl_queue_print(pl_queue_t *queue, void (*print_func)(void *))
{
    size_t i = 0;
    for (_pl_queue_node_t *node = queue->start; node != NULL; node = node->next, i++)
    {
        printf("[%zu=", i);
        print_func(node->data);
        printf("] ");
    }
    fputc('\n', stdout);
}
