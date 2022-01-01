#include "queue.h"

queue_node_t *queue_node_create(void *data)
{
    queue_node_t *node = malloc(sizeof(queue_node_t));
    if (node == NULL)
        return NULL;
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    return node;
}

void queue_init(queue_t *queue)
{
    queue->start = NULL;
    queue->end = NULL;
}

void *queue_push(queue_t *queue, void *data)
{
    queue_node_t *node = queue_node_create(data);
    if (node == NULL)
        return NULL;
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

void *queue_pop(queue_t *queue)
{
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

bool queue_empty(queue_t *queue)
{
    return queue->start == NULL;
}

void _debug_queue_print(queue_t *queue, void (*print_func)(void *))
{
    size_t i = 0;
    for (
        queue_node_t *node = queue->start;
        node != NULL;
        node = node->next, i++
    )
    {
        printf("[%zu=", i);
        print_func(node->data);
        printf("] ", i);
    }
    fputc('\n', stdout);
}
