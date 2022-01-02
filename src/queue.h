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
} queue_node_t;

typedef struct
{
    queue_node_t   *start;
    queue_node_t   *end;
    pthread_mutex_t mutex;
} queue_t;

void
queue_init(queue_t *queue);
void
queue_deinit(queue_t *queue);
void *
queue_push(queue_t *queue, void *data);
void *
queue_pop(queue_t *queue);
bool
queue_empty(queue_t *queue);

void
_debug_queue_print(queue_t *queue, void (*print_func)(void *));

#endif
