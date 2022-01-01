#include <stdio.h>
#include "queue.h"

void print_pointer(void *p)
{
    printf("%d", p);
}

int main(void)
{
    queue_t queue;
    queue_init(&queue);

    _debug_queue_print(&queue, print_pointer);
    queue_push(&queue, (void*)1);
    _debug_queue_print(&queue, print_pointer);
    queue_push(&queue, (void*)42);
    _debug_queue_print(&queue, print_pointer);
    queue_push(&queue, (void*)3212);
    _debug_queue_print(&queue, print_pointer);

    void *d = queue_pop(&queue);
    printf("%d\n", d);
    _debug_queue_print(&queue, print_pointer);

    d = queue_pop(&queue);
    printf("%d\n", d);
    _debug_queue_print(&queue, print_pointer);

    d = queue_pop(&queue);
    printf("%d\n", d);
    _debug_queue_print(&queue, print_pointer);

    return 0;
}
