#pragma once

#include "types.h"

/*
 * A FIFO-ish queue.
 */
typedef struct Queue {

    // The current number of elements in the queue.
    u32 length;

    // The size in bytes of an element.
    u32 stride;

    // The size in bytes of a QueueItem.
    u32 stride_padded;

    // The maximum number of elements in the queue.
    u32 capacity;

    // The current element being accessed during iteration.
    struct QueueItem *cursor;

    // The front of the queue.
    struct QueueItem *head;

    // The end of the queue.
    struct QueueItem *tail;
    
    // Block of memory holding all QueueItems.
    struct QueueItem *data;

} Queue;

/*
 * Initialize a queue which can hold a maximum of `capacity` elements of
 * `stride` bytes in size.
 */
b8 Queue_init(Queue *queue, u32 stride, u32 capacity);

/*
 * Internal clean-up of a queue.
 */
b8 Queue_free(Queue *queue);

/*
 * Add an element to the end of the queue.
 */
b8 Queue_enqueue(Queue *queue, void *value);

/*
 * Remove an element from the front of the queue.
 */
b8 Queue_dequeue(Queue *queue, void *value);

/*
 * Retrieve the element at the front of the queue without removing it.
 */
b8 Queue_peek(const Queue *queue, void *value);

/*
 * Iterate through the elements of a queue. Starts at the first element in the
 * queue. Returns false when the end of the queue is reached.
 */
b8 Queue_peekNext(Queue *queue, void *value);

/*
 * Called when iteration of a queue is stopped before reaching the end.
 */
b8 Queue_peekDone(Queue *queue);

/*
 * Removes the current element of a queue during iteration.
 */
b8 Queue_yank(Queue *queue, void *value);

