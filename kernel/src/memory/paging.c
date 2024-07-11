#include <memory/paging.h>
#include <core/screen.h>
#include <hardware/interrupts.h>

extern uint32_t heap_end;

extern void paging_enable(uint32_t*);

unsigned int* page_directory __attribute__((aligned(PAGE_FRAME_SIZE))) = (unsigned int*) 0xF00000;
unsigned int page_table[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));

void paging_init()
{
	int i;
  
	for (i = 0; i < NUM_PAGES; i++)
	{
		page_directory[i] = 0x00000002;  
    }     

	for (i = 0; i < NUM_PAGES; i++)
	{ 
	        page_table[i] = (i * 0x1000) | 3;
	}	

	page_directory[0] = ((unsigned int) page_table) | 3;
	 	

	paging_enable(page_directory);
   	register_interrupt_handler(14, handle_page_fault);
}

void handle_page_fault()
{
	printf("Page fault");
}
