#include <core/screen.h>

#define MAX_X 80
#define MAX_Y 25

void printf(string str)
{
	// Video memory in ram
	uint16_t* videoMemory = (uint16_t*) 0xb8000;
	// Static screen coordinates
	static uint8_t x = 0, y = 0;
	for(int i = 0; str[i] != '\0'; i++)
	{

		switch(str[i])
		{
			// Line break
			case '\n':
			{
				y++;
				x = 0;
				break;
			}

			default:
			{
				videoMemory[MAX_X * y + x] = (videoMemory[MAX_X * y + x] & 0xFF00) | str[i];
				x++;
				break;
			}
		}

		// New line if line is full
		if(x >= MAX_X)
		{
			y++;
			x = 0;
		}

		// Clear screen if it is full
		if(y >= 25)
		{
			for(y = 0; y < MAX_Y; y++)
			{
				for(x = 0; x < MAX_X; x++)
				{ 
					videoMemory[MAX_X * y + x] = (videoMemory[MAX_X * y + x] & 0xFF00) | ' ';
				}
			}

			x = 0;
			y = 0;
		}
	}
}

void printHex(uint8_t h)
{
	char* foo = "00";
	char* hex = "0123456789ABCDEF";
	foo[0] = hex[(h >> 4) & 0xF];
	foo[1] = hex[h & 0xF];
	printf(foo);
}
