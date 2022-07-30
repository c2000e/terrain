#include "memory.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>

struct MemoryStats {
    u64 allocated;
    u64 allocated_tagged[MEMORY_TAG_MAX_TAGS];
};

static struct MemoryStats stats;

void *s_alloc(u64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
    {
        LOGW("s_alloc MEMORY_TAG_UNKNOWN");
    }

    stats.allocated += size;
    stats.allocated_tagged[tag] += size;

    void *mem = malloc(size);

    s_zeroMemory(mem, size);

    return mem;
}

void s_free(void *mem, u64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
    {
        LOGW("s_free MEMORY_TAG_UNKNOWN");
    }

    stats.allocated -= size;
    stats.allocated_tagged[tag] -= size;

    free(mem);
}

void s_zeroMemory(void *mem, u64 size)
{
    memset(mem, 0, size);
}

