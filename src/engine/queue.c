#include "queue.h"

#include "logger.h"
#include "memory.h"

typedef struct QueueItem {
    struct QueueItem *next;
    struct QueueItem *prev;
    void *data;
} QueueItem;

b8 Queue_init(Queue *queue, u32 stride, u32 capacity)
{
    if (!queue)
    {
        LOGE("Queue_init: Invalid queue pointer");
        return false;
    }

    queue->length = 0;
    queue->stride = stride;
    queue->stride_padded = 2 * sizeof(QueueItem *) + stride;
    queue->capacity = capacity;
    queue->cursor = NULL;
    queue->head = NULL;
    queue->tail = NULL;
    queue->data = s_alloc(queue->stride_padded * capacity, MEMORY_TAG_QUEUE);

    return true;
}

b8 Queue_free(Queue *queue)
{
    if (queue)
    {
        s_free(
                queue->data,
                queue->stride_padded * queue->capacity,
                MEMORY_TAG_QUEUE
        );
    }
    return true;
}

b8 Queue_empty(const Queue *queue)
{
    return queue->length == 0;
}

b8 Queue_enqueue(Queue *queue, const void *value)
{
    if (!queue)
    {
        LOGE("Queue_enqueue: Invalid queue pointer");
        return false;
    }
    if (!value)
    {
        LOGE("Queue_enqueue: Invalid value pointer");
        return false;
    }

    // Check queue is not full
    if (queue->length == queue->capacity)
    {
        LOGE("Queue_enqueue: Can't enqueue value in full queue");
        return false;
    }

    // Find empty slot in queue memory
    QueueItem *empty = NULL;
    for (u32 i = 0; i < queue->capacity; i++)
    {
        if (queue->data + i * queue->stride_padded) continue;

        empty = queue->data + i * queue->stride_padded;
    }

    // Fill empty slot
    empty->next = NULL;
    empty->prev = queue->tail;
    s_copyMemory(
            empty->data,
            value,
            queue->stride
    );

    // Add to end of queue
    if (queue->length == 0)
    {
        queue->head = empty;
        queue->tail = empty;
    }
    else
    {
        queue->tail->next = empty;
        queue->tail = empty;
    }

    queue->length += 1;

    return true;
}

b8 Queue_dequeue(Queue *queue, void *value)
{
    if (!queue)
    {
        LOGE("Queue_dequeue: Invalid queue pointer");
        return false;
    }
    if (!value)
    {
        LOGE("Queue_dequeue: Invalid value pointer");
        return false;
    }

    // Check queue is not empty
    if (queue->length == 0)
    {
        LOGE("Queue_dequeue: Can't dequeue value from empty queue");
        return false;
    }

    // Set value
    s_copyMemory(value, queue->head->data, queue->stride);

    // Save reference for later
    QueueItem *removed = queue->head;

    // Remove from front of queue
    if (queue->length == 1)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        queue->head = queue->head->next;
    }
    queue->length -= 1;

    // Free memory used by item
    s_zeroMemory(removed, queue->stride_padded);

    return true;
}

b8 Queue_peek(const Queue *queue, void *value)
{
    if (!queue)
    {
        LOGE("Queue_peek: Invalid queue pointer");
        return false;
    }
    if (!value)
    {
        LOGE("Queue_peek: Invalid value pointer");
        return false;
    }

    // Check queue is not empty
    if (queue->length == 0)
    {
        LOGE("Queue_peek: Can't peek value from empty queue");
        return false;
    }

    // Set value
    s_copyMemory(value, queue->head->data, queue->stride);

    return true;
}

b8 Queue_peekNext(Queue *queue, void *value)
{
    if (!queue)
    {
        LOGE("Queue_peekNext: Invalid queue pointer");
        return false;
    }
    if (!value)
    {
        LOGE("Queue_peekNext: Invalid value pointer");
        return false;
    }

    if (queue->length == 0)
    {
        LOGE("Queue_peekNext: Can't peek value from empty queue");
        return false;
    }

    // At beginning of queue
    if (!queue->cursor)
    {
        queue->cursor = queue->head;
        s_copyMemory(value, queue->cursor->data, queue->stride);
        return true;
    }

    // In middle of queue
    if (queue->cursor->next)
    {
        queue->cursor = queue->cursor->next;
        s_copyMemory(value, queue->cursor->data, queue->stride);
        return true;
    }

    // At end of queue
    queue->cursor = NULL;
    return false;
}

b8 Queue_peekDone(Queue *queue)
{
    if (!queue)
    {
        LOGE("Queue_peekDone: Invalid queue pointer");
        return false;
    }

    queue->cursor = NULL;

    return true;
}

b8 Queue_yank(Queue *queue, void *value)
{
    if (!queue)
    {
        LOGE("Queue_yank: Invalid queue pointer");
        return false;
    }
    if (!value)
    {
        LOGE("Queue_yank: Invalid value pointer");
        return false;
    }

    // Check we're iterating through the queue
    if (!queue->cursor)
    {
        LOGE("Queue_yank: Queue cursor is not set");
        return false;
    }

    // Set value
    s_copyMemory(value, queue->cursor->data, queue->stride);

    // Remove item from queue
    if (queue->cursor->prev)
    {
        queue->cursor->prev->next = queue->cursor->next;
    }
    else
    {
        queue->head = queue->cursor->next;
    }

    if (queue->cursor->next)

    {
        queue->cursor->next->prev = queue->cursor->prev;
    }
    else
    {
        queue->tail = queue->cursor->prev;
    }
    queue->length -= 1;

    // Free memory used by item
    s_zeroMemory(queue->cursor, queue->stride_padded);

    // Move cursor forward
    queue->cursor = queue->cursor->next;

    return true;
}

