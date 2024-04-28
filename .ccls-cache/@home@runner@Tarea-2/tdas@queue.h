#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef List Queue;

Queue *queue_create(Queue *queue);
void queue_insert(Queue *queue, void *data);
void *queue_remove(Queue *queue);
void *queue_front(Queue *queue);
void queue_clean(Queue *queue);

#endif /* QUEUE_H */
