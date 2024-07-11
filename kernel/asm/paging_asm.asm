;void paging_enable()
;{
;	//asm volatile("mov %%cr3, %%eax": :"a"(&page_directory));	
;	//asm volatile("mov %eax, %cr0");
;	//asm volatile("or $0x80000001, %eax");
;	//asm volatile("mov %cr0, %eax");
;}

global paging_enable

paging_enable:
	mov eax, [esp + 4]
	mov cr3, eax

	mov eax, cr4
	or eax, 0x00000010
	mov cr4, eax

	mov eax, cr0
	or eax, 0x00000001
	mov cr0, eax


	ret
