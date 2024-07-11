#include <core/screen.h>
#include <core/types.h>
#include <memory/gdt.h>
#include <driver/driver.h>
#include <driver/ps2/ps2keyboard.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <memory/paging.h>
#include <memory/heap.h>
#include <memory/common.h>

extern uint8_t kernel_start;
extern uint8_t kernel_end;

uint32_t kernel_start_address;
uint32_t kernel_end_address;
uint32_t kernel_size;

uint32_t heap_end;

void kernel_main()
{
	clear_screen();
	printf("<Mercury> Loading MercuryOS... \n");

	segments_install_gdt();

	install_idt();

	kernel_start_address = (uint32_t) &kernel_start;
	kernel_end_address = (uint32_t) &kernel_end;

	kernel_size = kernel_end_address - kernel_start_address;
	print_memory_info();
	size_t heap = 10 * 1024 * 1024;
	heap_init(kernel_end_address, heap);
	heap_end = kernel_end_address + heap;

	printf("<Mercury> Heap size: 0x");
	print_hex((heap >> 24) & 0xFF);
	print_hex((heap >> 16) & 0xFF);
	print_hex((heap >> 8) & 0xFF);
	print_hex(heap & 0xFF);
	printf("\n");

	void* allocated = malloc(1024);
	printf("<Mercury> Heap start: 0x");
	print_hex(((uint32_t) allocated >> 24) & 0xFF);
	print_hex(((uint32_t) allocated >> 16) & 0xFF);
	print_hex(((uint32_t) allocated >> 8) & 0xFF);
	print_hex((uint32_t) allocated & 0xFF);
	printf("\n");

	printf("<Mercury> Heap End: 0x");
	print_hex((heap_end >> 24) & 0xFF);
	print_hex((heap_end >> 16) & 0xFF);
	print_hex((heap_end >> 8) & 0xFF);
	print_hex(heap_end & 0xFF);
	printf("\n");

	printf("<Mercury> Setting up memory paging\n");
	paging_init();

	uint8_t keyboard_driver = create_driver(0x21, "PS2-Keyboard", KEYBOARD, 
			ps2_kb_handle_interrupt, ps2_kb_enable, 
			ps2_kb_disable);

	enable_driver(keyboard_driver);

	printf("<Mercury> Searching PCI deivce drivers\n");
	pci_enumerate_devices();
	printf("<Mercury> PCI Initialization done\n");

	malloc(0);

	while(1);
}
