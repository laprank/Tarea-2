#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "map.h"

typedef struct Map PQueue;

PQueue *pqueue_create(PQueue *queue);
void pqueue_insert(PQueue *queue, int priority, void *data);
void *pqueue_remove(PQueue *queue);
void *pqueue_front(PQueue *queue);
void pqueue_clean(PQueue *queue);

#endif /* PRIORITY_QUEUE_H */
