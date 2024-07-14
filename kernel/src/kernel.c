#include <core/screen.h>
#include <core/types.h>
#include <memory/gdt.h>
#include <driver/driver.h>
#include <driver/ps2/ps2keyboard.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <memory/mem_manager.h>
#include <memory/common.h>
#include <memory/paging.h>

extern uint8_t kernel_start;
extern uint8_t kernel_end;

uint32_t kernel_start_address;
uint32_t kernel_end_address;
uint32_t kernel_size;

uint32_t mem_manager_end;

void kernel_main()
{
	clear_screen();
	printf("<Mercury> Loading MercuryOS... \n");

	segments_install_gdt();

	install_idt();

	kernel_start_address = (uint32_t) &kernel_start;
	kernel_end_address = (uint32_t) &kernel_end;

	kernel_end_address = ((kernel_end_address / 4096) + 1) * 4096;

	kernel_size = kernel_end_address - kernel_start_address;
	print_memory_info();
	size_t mem_manager_size = 512 * 1024;
	init_memory_manager(kernel_start_address - mem_manager_size - 0x10000, mem_manager_size);
	init_memory_region(kernel_start_address - mem_manager_size - 0x10000, mem_manager_size);

	printf("<Mercury> Block manager size: 0x");
	print_hex((mem_manager_size >> 24) & 0xFF);
	print_hex((mem_manager_size >> 16) & 0xFF);
	print_hex((mem_manager_size >> 8) & 0xFF);
	print_hex(mem_manager_size & 0xFF);
	printf("\n");

	printf("<Mercury> Setting up paging\n");
	paging_enable();

	map_page((void*) 0xb8000, (void*)0xb8000);

	uint8_t keyboard_driver = create_driver(0x21, "PS2-Keyboard", KEYBOARD, 
			ps2_kb_handle_interrupt, ps2_kb_enable, 
			ps2_kb_disable);

	enable_driver(keyboard_driver);

	printf("<Mercury> Searching PCI deivce drivers\n");
	pci_enumerate_devices();
	printf("<Mercury> PCI Initialization done\n");

	clear_screen();
	printf("Welcome to MercuryOS!\n");

	while(1);
}
