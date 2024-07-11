#include <memory/common.h>
#include <core/screen.h>

extern uint32_t kernel_start_address;
extern uint32_t kernel_end_address;
extern uint32_t kernel_size;

void memset(uint8_t* destination, uint8_t value, uint32_t length)
{
	uint8_t* temp = (uint8_t*) destination;
	for(; length != 0; length--) *temp++ = value;
}

void print_memory_info()
{
	printf("<Mercury> Kernel start: 0x");
	print_hex((kernel_start_address >> 24) & 0xFF);
	print_hex((kernel_start_address >> 16) & 0xFF);
	print_hex((kernel_start_address >> 8) & 0xFF);
	print_hex(kernel_start_address & 0xFF);
	printf("\n");

	printf("<Mercury> Kernel end: 0x");
	print_hex((kernel_end_address >> 24) & 0xFF);
	print_hex((kernel_end_address >> 16) & 0xFF);
	print_hex((kernel_end_address >> 8) & 0xFF);
	print_hex(kernel_end_address & 0xFF);
	printf("\n");

	printf("<Mercury> Kernel size: 0x");
	print_hex((kernel_size >> 24) & 0xFF);
	print_hex((kernel_size >> 16) & 0xFF);
	print_hex((kernel_size >> 8) & 0xFF);
	print_hex(kernel_size & 0xFF);
	printf("\n");
}
