ENTRY(entry)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0xA000;

	kernel_start = .;

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
		*stack(.bss)
	}

	/DISCARD/  :
	{
		*(.fini_array*)
		*(.comment)
	}

	kernel_end = .;
}
