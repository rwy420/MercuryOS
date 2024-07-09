#include <memory/paging.h>
#include <core/screen.h>
#include <hardware/interrupts.h>

//extern void load_page_directory(uint32_t*);
//extern void enable_paging();

extern uint32_t kernel_physical_end;

uint32_t* page_directory/*[NUM_PAGES]*/ __attribute__((aligned(PAGE_FRAME_SIZE)));
uint32_t page_table[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));

uint32_t page_directory_location = 0;
uint32_t* last_page = 0;

void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys)
{
	uint16_t id = virt >> 22;
	for(int i = 0; i < 1024; i++)
	{
		last_page[i] = phys | 3;
		phys += 4096;
	}
	page_directory[id] = ((uint32_t)last_page) | 3;
	last_page = (uint32_t *)(((uint32_t)last_page) + 4096);
}

void init_paging()
{
	int i;

	page_directory = (uint32_t*) 0x400000;
	page_directory_location = (uint32_t) page_directory;
	last_page = (uint32_t*) 0x404000;

	printf("1");
	for(i = 0; i < NUM_PAGES; i++)
	{
		page_directory[i] = /*0x00000002;*/ 0 | 2;
	}

	paging_map_virtual_to_phys(0, 0);
	paging_map_virtual_to_phys(0x400000, 0x400000);

	printf("2");
	/*for(i = 0; i < NUM_PAGES; i++)
	{
		page_table[i] = (i * 0x1000) | 3;
	}*/

	printf("3");
	//page_directory[0] = ((uint32_t) page_table) | 3;

	printf("4");

	enable_paging();

	register_interrupt_handler(14, page_fault);
	/*uint32_t cr3;
	asm("movl %0, %%cr3" : "=r" (cr3));

	printHex((cr3 >> 24) & 0xFF);
	printHex((cr3 >> 16) & 0xFF);
	printHex((cr3 >> 8) & 0xFF);
	printHex(cr3 & 0xFF);

	printf("*\n");*/
}

static void enable_paging()
{
	asm volatile("mov %%eax, %%cr3" : : "a" (page_directory));
	asm volatile("mov %cr0, %eax");
	asm volatile("orl $0x80000000, %eax");
	asm volatile("mov %eax, %cr0");
}

void* get_physical_address(void* virtual_address)
{
	unsigned long pd_index = (unsigned long) virtual_address >> 22;
	unsigned long pt_index = (unsigned long) virtual_address >> 12 & 0x03FF;

	unsigned long* pd = (unsigned long*) page_directory;
	unsigned long* pt = ((unsigned long*) page_table) + (0x1000 * pd_index);

	return (void*) ((pt[pt_index] & ~0xFFF) + ((unsigned long) virtual_address & 0xFFF));
}

void map_page(void* physical_address, void* virtual_address, uint32_t flags)
{
	unsigned long pd_index = (unsigned long) virtual_address >> 22;
	unsigned long pt_index = (unsigned long) virtual_address >> 12 & 0x03FF;

	unsigned long* pd = (unsigned long*) page_directory;
	unsigned long* pt = ((unsigned long*) page_table) + (0x1000 * pd_index);

	pt[pt_index] = ((unsigned long) physical_address) | (flags & 0xFFF) | 0x01;

	flush_tlb((unsigned long)virtual_address);
}

void flush_tlb(unsigned long address)
{
	asm volatile("invlpg (%0)" : : "r" (address) : "memory");
}

void page_fault()
{
	printf("Page fault");
}
