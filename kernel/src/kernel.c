#include <core/screen.h>
#include <core/types.h>
#include <memory/gdt.h>
#include <driver/driver.h>
#include <driver/ps2/ps2keyboard.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <memory/paging.h>
#include <memory/kheap.h>

void KernelMain(const void* multiboot_structure, uint32_t kernel_virtual_start, uint32_t kernel_virtual_end,
		uint32_t kernel_physical_start, uint32_t kernel_physical_end)
{
	printf("<Mercury> Loading MercuryOS... \n");

	segments_install_gdt();

	install_idt();

	printf("<Mercury> Setting up memory paging");
	init_paging();


	uint8_t keyboard_driver = create_driver(0x21, "PS2-Keyboard", KEYBOARD, 
			ps2_kb_handle_interrupt, ps2_kb_enable, 
			ps2_kb_disable);

	enable_driver(keyboard_driver);

	printf("<Mercury> Searching PCI deivce drivers\n");
	pci_enumerate_devices();
	printf("<Mercury> PCI Initialization done\n");

	while(1);
}
