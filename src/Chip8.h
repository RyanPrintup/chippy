#ifndef _CHIP8_H_
#define _CHIP8_H_

class Chip8 
{
	public:
		Chip8();

		void init();
		bool loadProgram(const char* path);
		void run();

		bool drawFlag;
		unsigned char gfx[64 * 32];

		unsigned char key[16];

	private:
		unsigned short opcode;

		unsigned char memory[4096];
		unsigned char V[16];
		unsigned short I;
		unsigned short pc;

		unsigned char delay_timer;
		unsigned char sound_timer;

		unsigned short stack[16];
		unsigned short sp;
};

#endif
