#include <memory/common.h>

void memset(uint8_t* destination, uint8_t value, uint32_t length)
{
	uint8_t* temp = (uint8_t*) destination;
	for(; length != 0; length--) *temp++ = value;
}


