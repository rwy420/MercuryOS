#include <syscalls.h>
#include <core/screen.h>
#include <core/types.h>
#include <exec/elf/elf_loader.h>
#include <memory/gdt.h>
#include <driver/driver.h>
#include <driver/ps2/ps2keyboard.h>
#include <driver/ata/ata.h>
#include <driver/vga/vga.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <memory/mem_manager.h>
#include <memory/common.h>
#include <memory/paging.h>
#include <fs/bootfs/bootfs.h>

#define ATA
//#define VGA

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

	printf("<Mercury> Registering syscalls\n");
	register_interrupt_handler(0x80, syscall);
	register_syscall_handler(0x4, (syscall_t) syscall_printf);

#ifdef ATA
	Disk ata0m = init_disk(0x1F0, true);
	if(identify_disk(&ata0m))
	{
		printf("<Mercury> Selected ata0m as default disk\n");
		set_default_disk(&ata0m);
	}

	Disk ata0s = init_disk(0x1F0, false);
	if(identify_disk(&ata0s))
	{
		printf("<Mercury> Selected ata0s as default disk\n");
		set_default_disk(&ata0s);
	}
#endif

	kernel_start_address = (uint32_t) &kernel_start;
	kernel_end_address = (uint32_t) &kernel_end;

	kernel_end_address = ((kernel_end_address / 4096) + 1) * 4096;

	kernel_size = kernel_end_address - kernel_start_address;
	print_memory_info();
	size_t mem_manager_size = 64 * 1024;
	init_memory_manager(kernel_start_address - mem_manager_size, mem_manager_size);
	init_memory_region(kernel_start_address - mem_manager_size, mem_manager_size);

	heap_init(0x200000, 0x50000);

	printf("<Mercury> Block manager size: 0x");
	print_hex((mem_manager_size >> 24) & 0xFF);
	print_hex((mem_manager_size >> 16) & 0xFF);
	print_hex((mem_manager_size >> 8) & 0xFF);
	print_hex(mem_manager_size & 0xFF);
	printf("\n");

	printf("<Mercury> Setting up paging\n");
	paging_enable();
	
	uint8_t keyboard_driver = create_driver(0x21, "PS2-Keyboard", 0, 
			ps2_kb_handle_interrupt, ps2_kb_enable, 
			ps2_kb_disable);

	enable_driver(keyboard_driver);

	printf("<Mercury> Searching PCI deivce drivers\n");
	pci_enumerate_devices(false);
	printf("<Mercury> PCI Initialization done\n");

	//clear_screen();
	
	/*printf(" __  __                                 ___  ____ \n");
	printf("|  \\/  | ___ _ __ ___ _   _ _ __ _   _ / _ \\/ ___| \n");
	printf("| |\\/| |/ _ \\ '__/ __| | | | '__| | | | | | \\___ \\ \n");
	printf("| |  | |  __/ | | (__| |_| | |  | |_| | |_| |___) | \n");
	printf("|_|  |_|\\___|_|  \\___|\\__,_|_|   \\__, |\\___/|____/ \n"); 
	printf("                                 |___/ \n");*/
#ifdef ATA
	read_files();

	uint8_t* buffer = malloc(64*512);
	memset(buffer, 0x0, sizeof(buffer));

	for(int i = 0; i < 32; i++)
	{
		read28(73 + i, (buffer + i * 512), 512);
	}

	void(*entry)();
	entry = image_load((char*) buffer, sizeof(buffer), false);
	entry();
#endif

#ifdef VGA
	vga_set_mode(320, 200, 8);
	vga_bluescreen();
#endif
	
	while(1);
}
