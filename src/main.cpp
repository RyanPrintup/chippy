#include "Chip8.h"

int main(int argc, char** argv)
{
	Chip8 chip8;

	chip8.init();
	chip8.run();

	return 0;
}
