#ifndef __MERCURY__MEMORY__KHEAP_H
#define __MERCURY__MEMORY__KHEAP_H

#include <core/types.h>

struct MemoryChunk
{
	struct MemoryChunk* next;
	struct MemoryChunk* previous;
	bool allocated;
	size_t size;
};

void heap_init(uint32_t start, size_t size);
void* malloc(size_t size);
void free(void* pointer);

#endif
