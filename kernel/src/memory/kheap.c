#include <memory/kheap.h>

extern uint32_t kernel_physical_start;
extern uint32_t kernel_physical_end;

uint32_t kmalloc_int(uint32_t size, uint32_t align, uint32_t* address_pointer);
uint32_t kmalloc_align(uint32_t size);
uint32_t kmalloc_align_pointer(uint32_t size, uint32_t address_pointer);
uint32_t kmalloc(uint32_t size);
