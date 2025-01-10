#include <core/screen.h>
#include <driver/driver.h>
#include <hardware/pci.h>

#define DATA_PORT 0xCFC
#define COMMAND_PORT 0xCF8

uint32_t pci_read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset)
{
	uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (offset & 0xFC);
	outl(COMMAND_PORT, id);
	uint32_t result = inl(DATA_PORT);
	return result >> (8 * (offset % 4));
}

void pci_write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t value)
{
	uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (offset & 0xFC);
	outl(COMMAND_PORT, id);
	outl(DATA_PORT, value);
}

void pci_enumerate_devices(bool debug)
{
	for(int bus = 0; bus < 8; bus++)
	{
		for(int device = 0; device < 32; device++)
		{
			int function_count = has_functions(bus, device) ? 8 : 1; 
			for(int function = 0; function < function_count; function++)
			{
				uint16_t vendor_id = pci_get_vendor_id(bus, device, function);
				uint16_t device_id = pci_get_device_id(bus, device, function);
				uint32_t port_base = 0x0;

				if(vendor_id == 0x0000 || vendor_id == 0xFFFF) continue;

				for(int bar_idx = 0; bar_idx < 6; bar_idx++)
				{
					BAR* bar = pci_get_bar(bus, device, function, bar_idx);
					if(bar->address && (bar->type == IO)) port_base = (uint32_t) bar->address;
				}

				if(debug) 
				{
					print_hex(bus & 0xFF);
					printf(" ");
					print_hex(device & 0xFF);
					printf(" ");
					print_hex(function & 0xFF);
					printf(" ");
					print_hex((vendor_id & 0xFF00) >> 8);
					print_hex(vendor_id & 0xFF);
					printf(" ");
					print_hex((device_id & 0xFF00) >> 8);
					print_hex(device_id & 0xFF);
					printf(" ");
					print_hex32((uint32_t) port_base);
					printf("\n");
				}
			}
		}
	}
}

struct Driver* get_driver(uint16_t vendor, uint16_t device)
{
	switch(vendor)
	{
		case 0x1022: // AMD
		{
			switch(device)
			{
				case 0x2000: //79c970
					break;
			}
			break;
		}

		case 0x8086: // Intel
		{

		}
	}
	
	return 0;
}

uint16_t pci_get_vendor_id(uint16_t bus, uint16_t device, uint16_t function)
{
	uint32_t result = pci_read(bus, device, function, 0);
	return result;
}

uint16_t pci_get_device_id(uint16_t bus, uint16_t device, uint16_t function)
{	
	uint32_t result = pci_read(bus, device, function, 2);
	return result;
}

uint16_t pci_get_class_id(uint16_t bus, uint16_t device, uint16_t function)
{
	uint32_t result = pci_read(bus, device, function, 0xA);
	return (result & ~0x00FF) >> 8;
}

uint16_t pci_get_subclass_id(uint16_t bus, uint16_t device, uint16_t function)
{
	uint32_t result = pci_read(bus, device, function, 0xA);
	return (result & ~0xFF00);
}

BAR* pci_get_bar(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
	BAR* result;

	uint32_t header_type = pci_read(bus, device, function, 0x0E) & 0x7F;
	int max_bars = 6 - (4 * header_type);

	if(bar >= max_bars) return result;

	uint32_t bar_value = pci_read(bus, device, function, 0x10 + 4 * bar);
	result->type = (bar_value & 0x1) ? IO : MM;
	uint32_t temp;

	if(result->type == MM)
	{
		switch ((bar_value >> 1) & 0x3)
		{
			case 0:
			{
				// 32 Bit mode
				break;
			}
			case 1:
			{
				// 20 Bit mode
				break;
			}
			case 2:
			{
				// 64 Bit mode
				break;
			}
		}
		result->prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
	}
	else
	{
		// IO
		result->address = (uint8_t*) (bar_value & ~0x3);
		result->prefetchable = false;
	}

	return result;
}

bool has_functions(uint16_t bus, uint16_t device)
{
	return pci_read(bus, device, 0, 0x0E) & (1 << 7);
}
