#include <memory/heap.h>
#include <core/screen.h>

uint32_t size;
struct MemoryChunk* first;

void heap_init(uint32_t start, size_t size)
{
	first = (struct MemoryChunk*) start;
	first->previous = 0;
	first->next = 0;
	first->size = size - sizeof(struct MemoryChunk);
}

void* malloc(size_t size)
{
	struct MemoryChunk* result = 0;

	for(struct MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
	{
		if(chunk->size > size && !chunk->allocated) result = chunk;
	}

	if(result->size >= size + sizeof(struct MemoryChunk) + 1)
	{
		struct MemoryChunk* temp = (struct MemoryChunk*)  ((size_t) result + sizeof(struct MemoryChunk) + size);
	
		temp->allocated = false;
		temp->size = result->size - size - sizeof(struct MemoryChunk);
		temp->previous = result;
		temp->next = result->next;

		if(temp->next != 0) temp->next->previous = temp;

		result->size = size;
		result->next = temp;
	}

	result->allocated = true;
	return (void*) (((size_t) result) + sizeof(struct MemoryChunk));
}

void free(void* pointer)
{
	struct MemoryChunk* chunk = (struct MemoryChunk*) ((size_t) pointer - sizeof(struct MemoryChunk));

	chunk->allocated = false;

	if(chunk->previous != 0 && !chunk->previous->allocated)
	{
		chunk->previous->next = chunk->next;
		chunk->previous->size += chunk->size + sizeof(struct MemoryChunk);

		if(chunk->next != 0) chunk->next->previous = chunk->previous;

		chunk = chunk->previous;
	}

	if(chunk->next != 0 && !chunk->next->allocated)
	{
		chunk->size += chunk->next->size + sizeof(struct MemoryChunk);
		chunk->next = chunk->next->next;

		if(chunk->next != 0) chunk->next->previous = chunk;
	}
}
