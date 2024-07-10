ENTRY(boot)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

kernel_virtual_start = .;
kernel_physical_start = . - 0xC0000000;

SECTIONS
{
	. = 0xA000;

	.text ALIGN(0x1000) :
	{
		*(.text*)
		*(.rodata)
	}

	.data ALIGN(0x1000) :
	{
		start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;

		*(.data*)
	}

	.bss ALIGN(0x1000) :
	{
		*(.bss)
	}

	/DISCARD/  :
	{
		*(.fini_array*)
		*(.comment)
	}
}

kernel_virtual_end = .;
kernel_physical_end = . - 0xC0000000;
