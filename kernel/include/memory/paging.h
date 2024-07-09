#ifndef __MERCURY__MEMORY__PAGING_H
#define __MERCURY__MEMORY__PAGING_H

#include <core/types.h>

#define NUM_PAGES 1024
#define PAGE_FRAME_SIZE 4096

#define PRESENT 1
#define PAGE_READONLY 0
#define PAGE_RW 1
#define PAGE_USER 1
#define PAGE_KERNEL 0
#define PAGE_SIZE_4KB 0
#define PAGE_SIZE_4MB 1

typedef struct page
{
	uint32_t present	: 1;
	uint32_t rw			: 1;
	uint32_t user		: 1;
	uint32_t accessed   : 1;
	uint32_t dirty		: 1;
	uint32_t unused		: 7;
	uint32_t frame		: 20;
} page_t;

typedef struct page_table
{
	page_t pages[1024] __attribute__((aligned(4096)));
} page_table_t;

typedef struct page_directory
{
	page_table_t* tables[1024];
	uint32_t tables_pysical[1024];

	uint32_t physical_address;
} page_directory_t;

void init_paging();
static void enable_paging();
void page_fault();

void* get_physical_address(void* virtual_address);
void map_page(void* physical_address, void* virtual_address, uint32_t flags);
void flush_tlb(unsigned long address);

/*static void set_frame(uint32_t frame_address);
static void clear_frame(uint32_t frame_address);
static int32_t first_frame();
void alloc_frame_identity(page_t* page, uint32_t is_kernel, uint32_t is_writable);
void alloc_frame_virtual(page_t* page, uint32_t physical_address, uint32_t is_kernel, uint32_t is_writable);
void free_frame(page_t* page);
page_t* get_page(uint32_t address, uint8_t make, page_directory_t* directory);
void switch_page_directory(page_directory_t* directory);*/

#endif
