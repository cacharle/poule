#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdio.h>
#include "queue.h"
#include "pool.h"

// void
// print_pointer(void *p)
// {
//     //printf("%d", (int)p);
// }

void *
add_one_func(void *data)
{
    unsigned long i = data;
    i += 1;
    usleep(10000);
    return i;
}

int
main(void)
{
    // queue_t queue;
    // queue_init(&queue);
    // _debug_queue_print(&queue, print_pointer);
    // queue_push(&queue, (void *)1);
    // _debug_queue_print(&queue, print_pointer);
    // queue_push(&queue, (void *)42);
    // _debug_queue_print(&queue, print_pointer);
    // queue_push(&queue, (void *)3212);
    // _debug_queue_print(&queue, print_pointer);
    // void *d = queue_pop(&queue);
    // //printf("%d\n", d);
    // _debug_queue_print(&queue, print_pointer);
    // d = queue_pop(&queue);
    // //printf("%d\n", d);
    // _debug_queue_print(&queue, print_pointer);
    // d = queue_pop(&queue);
    // //printf("%d\n", d);
    // _debug_queue_print(&queue, print_pointer);

    pool_t pool;
    // printf("yo0\n");
    pool_init(&pool, 2, add_one_func);
    // printf("yo1\n");
    pool_result_t *result36 = pool_submit(&pool, (void *)36UL);
    // printf("yo2\n");
    pool_result_t *result42 = pool_submit(&pool, (void *)42UL);
    // printf("yo3\n");

    unsigned long r36 = (unsigned long)pool_result_wait(result36);
    // printf("yo4\n");
    unsigned long r42 = (unsigned long)pool_result_wait(result42);
    // printf("yo5\n");

    printf("36=%lu 42=%lu\n", r36, r42);

    pool_shutdown(&pool);

    return 0;
}