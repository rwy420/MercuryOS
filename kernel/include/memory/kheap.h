#ifndef __MERCURY__MEMORY__KHEAP_H
#define __MERCURY__MEMORY__KHEAP_H

#include <core/types.h>

uint32_t kmalloc_int(uint32_t size, uint32_t align, uint32_t* address_pointer);
uint32_t kmalloc_align(uint32_t size);
uint32_t kmalloc_align_pointer(uint32_t size, uint32_t address_pointer);
uint32_t kmalloc(uint32_t size);

#endif
