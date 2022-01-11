#ifndef POULE_PPOOL_H
#define POULE_PPOOL_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "poule/queue.h"

typedef struct {
    pl_queue_t queue_work;
    pl_queue_t queue_done;
    pid_t *pids;
    size_t pids_len;
    pthread_t manager;
} pl_ppool_t;

#endif
