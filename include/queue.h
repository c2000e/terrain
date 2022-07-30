#pragma once

#include "types.h"

typedef struct Queue {
    u32 length;
    u32 stride;
    u32 capacity;
    i32 head;
    i32 tail;
    void *data;
} Queue;

b8 Queue_init(Queue *queue, u32 stride, u32 capacity);

void Queue_free(Queue *queue);

b8 Queue_enqueue(Queue *queue, void *value);
b8 Queue_dequeue(Queue *queue, void *value);
b8 Queue_peek(const Queue *queue, void *value);

