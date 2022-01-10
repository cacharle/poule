#ifndef POULE_QUEUE_H
#define POULE_QUEUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct queue_node
{
    struct queue_node *next;
    struct queue_node *prev;
    void              *data;
} _pl_queue_node_t;

typedef struct
{
    _pl_queue_node_t *start;
    _pl_queue_node_t *end;
    pthread_mutex_t   mutex;
    size_t            len;
} pl_queue_t;

void
pl_queue_init(pl_queue_t *queue);
void
pl_queue_deinit(pl_queue_t *queue, void (*free_func)(void *));
void *
pl_queue_push(pl_queue_t *queue, void *data);
void *
pl_queue_pop(pl_queue_t *queue);
size_t
pl_queue_len(pl_queue_t *queue);
bool
pl_queue_empty(pl_queue_t *queue);

void
_debug_pl_queue_print(pl_queue_t *queue, void (*print_func)(void *));

#endif
