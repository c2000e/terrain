#include "queue.h"

#include "memory.h"
#include "logger.h"

b8 Queue_init(Queue *queue, u32 stride, u32 capacity)
{
    if (!queue)
    {
        LOGE("Queue_init: Unitialized queue pointer.");
        return false;
    }

    queue->length   = 0;
    queue->stride   = stride;
    queue->capacity = capacity;
    queue->head     = 0;
    queue->tail     = -1;

    queue->data = s_alloc(capacity * stride, MEMORY_TAG_QUEUE);

    return true;
}

void Queue_free(Queue *queue)
{
    if (queue)
    {
        s_free(queue->data, queue->capacity * queue->stride, MEMORY_TAG_QUEUE);
    }
}

b8 Queue_enqueue(Queue *queue, void *value)
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

    if (queue->length == queue->capacity)
    {
        LOGE("Queue_enqueue: Can't enqueue value in full queue");
        return false;
    }

    queue->tail = (queue->tail + 1) % queue->capacity;
    s_copyMemory(
            (char *)queue->data + (queue->tail * queue->stride),
            value,
            queue->stride
    );
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

    if (queue->length == 0)
    {
        LOGE("Queue_dequeue: Can't dequeue value from empty queue");
        return false;
    }

    s_copyMemory(
            value,
            (char *)queue->data + (queue->head * queue->stride),
            queue->stride
    );
    queue->head = (queue->head + 1) % queue->capacity;
    queue->length -= 1;

    return true;
}

b8 Queue_peek(const Queue *queue, void *value)
{
    if (!queue)
    {
        LOGE("Queue_peek: Invalid queue pointer");
        return 0;
    }
    if (!value)
    {
        LOGE("Queue_peek: Invalid value pointer");
        return 0;
    }

    if (queue->length == 0)
    {
        LOGE("Queue_peek: Can't peek value from empty queue");
        return 0;
    }

    s_copyMemory(
            value,
            (char *)queue->data + (queue->head * queue->stride),
            queue->stride
    );

    return 1;
}

