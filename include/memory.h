#pragma once

#include <stdlib.h>

typedef enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_RENDER,
    MEMORY_TAG_STRING,
    MEMORY_TAG_JOB,
    MEMORY_TAG_MAX_TAGS
} memory_tag;

void *s_alloc(size_t size, memory_tag tag);

void s_free(void *mem, size_t size, memory_tag tag);

void s_zeroMemory(void *mem, size_t size);

