#include <core/screen.h>
#include <core/types.h>
#include <memory/gdt.h>
#include <driver/driver.h>
#include <driver/ps2/ps2keyboard.h>
#include <driver/ata/ata.h>
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

	/*uint32_t page_2 = kmalloc_page();

	printHex((page_2 >> 24) & 0xFF);
	printHex((page_2 >> 16) & 0xFF);
	printHex((page_2 >> 8) & 0xFF);
	printHex(page_2 & 0xFF);*/

	/*printf("\n");

	map_page((void*) (uint32_t) 0x100, (void*) (uint32_t) 0x6969, 0x0);

	uint32_t page = (uint32_t) get_physical_address((void*) (uint32_t) 0x100);*/




	/*struct Disk ata0m;
	ata0m.port_base = 0x1F0;
	ata0m.master = true;

	bool ata0m_ident_success = ata_ident(&ata0m);

	if(!ata0m_ident_success) printf("ATA INIT FAILED");*/

	/*printf("<Mercury> Booting MercuryOS      \n"); //Spacing to overwrite grub message
	
	GDT gdt;
	printf("<Mercury> Loaded GDT\n");

	uint32_t* memUpper = (uint32_t*) (((size_t) multiboot_structure) + 8);
	size_t heapStart = 10 * 1024 * 1024;
	Heap heap(heapStart, (*memUpper) * 1024 - heapStart - 10 * 1024);

	printf("<Mercury> Heap: 0x");
	printHex((heapStart >> 24) & 0xFF);
	printHex((heapStart >> 16) & 0xFF);
	printHex((heapStart >> 8) & 0xFF);
	printHex(heapStart & 0xFF);
	printf("\n");

	InterruptManager interrupts(&gdt);
	printf("<Mercury> Loaded IDT\n");

	DriverManager driverManager;

	ConsoleKeyboardHandler keyboardHandler;
	PS2KeyboardDriver ps2keyboard(&interrupts, &keyboardHandler);
	driverManager.AddDriver(&ps2keyboard);

	printf("<Mercury> Initializing PCI\n");
	PCI pci;
	printf("<Mercury> Looking for available PCI device drivers\n");
	pci.SelectDrivers(&driverManager, &interrupts);

	driverManager.ActivateAll();
	
	am79c973* eth0 = (am79c973*) driverManager.GetAvailableDriver(mercury::driver::ETHERNET);

	ATA ata0m(0x1F0, true);
	ata0m.Ident();

	ATA ata0s(0x1F0, false);
	ata0s.Ident();


	interrupts.Activate();
	printf("<Mercury> Interrupts activated\n");

	PartitionTable partitionTable;
	partitionTable.ReadPartitions(&ata0s);

	printf("<Mercury> Welcome to CelestialOS\n");

	*uint8_t buffer[128];

	ata0s.Write28(1, (uint8_t*) "This is sector 1", 16);
	ata0s.FlushCache();
	//ata0s.Read28(1, buffer, 16);

	ata0s.Write28(2, (uint8_t*) "This is sector 2", 16);
	ata0s.FlushCache();
	//ata0s.Read28(2, buffer, 16);

	ata0s.Write28(3, (uint8_t*) "This is sector 3", 16);
	ata0s.FlushCache();
	//ata0s.Read28(3, buffer, 16);

	for(int i = 0; i < 15; i++)
	{
		printHex(buffer[i]);
	}

	ata0s.Write28(1, (uint8_t*) "This is sector 1", 16);
	ata0s.FlushCache();
	ata0s.Read28(1, buffer, 16);*/

	// Infinite loop*/
	while(1);
}
