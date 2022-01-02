#include "queue.h"

queue_node_t *
queue_node_create(void *data)
{
    queue_node_t *node = malloc(sizeof(queue_node_t));
    if (node == NULL)
        return NULL;
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    return node;
}

void
queue_init(queue_t *queue)
{
    queue->start = NULL;
    queue->end = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
}

void
queue_deinit(queue_t *queue, void (*free_func)(void *))
{
    queue_node_t *node = queue->start;
    while (node != NULL)
    {
        if (free_func != NULL)
            free_func(node->data);
        queue_node_t *curr = node;
        node = node->next;
        free(curr);
    }
    pthread_mutex_destroy(&queue->mutex);
}

static void *
queue_push_unsafe(queue_t *queue, void *data)
{
    queue_node_t *node = queue_node_create(data);
    if (node == NULL)
        return NULL;
    if (queue->start == NULL)
    {
        queue->start = node;
        queue->end = node;
        pthread_mutex_unlock(&queue->mutex);
        return node;
    }
    node->next = queue->start;
    node->next->prev = node;
    queue->start = node;
    return node->data;
}

void *
queue_push(queue_t *queue, void *data)
{
    pthread_mutex_lock(&queue->mutex);
    void *node = queue_push_unsafe(queue, data);
    pthread_mutex_unlock(&queue->mutex);
    return node;
}

static void *
queue_pop_unsafe(queue_t *queue)
{
    if (queue_empty(queue))
        return NULL;
    queue_node_t *node = queue->end;
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
queue_pop(queue_t *queue)
{
    pthread_mutex_lock(&queue->mutex);
    void *data = queue_pop_unsafe(queue);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

bool
queue_empty(queue_t *queue)
{
    return queue->start == NULL;
}

void
_debug_queue_print(queue_t *queue, void (*print_func)(void *))
{
    size_t i = 0;
    for (queue_node_t *node = queue->start; node != NULL; node = node->next, i++)
    {
        printf("[%zu=", i);
        print_func(node->data);
        printf("] ");
    }
    fputc('\n', stdout);
}
