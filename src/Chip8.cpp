#include "Chip8.h"
#include <iostream>

void Chip8::init()
{
	// Initialize registers and memory once
	opcode = 0;

	for (int i = 0; i < 4096; i++) {
		memory[i] = 0;
	}

	for (int i = 0; i < 16; i++) {
		V[i] = 0;
	}

	I = 0x0;
	pc = 0x200;

	for (int i = 0; i < 64 * 32; i++) {
		gfx[i] = 0;
	}

	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < 16; i++) {
		stack[i] = 0;
	}
	
	sp = 0;

	for (int i = 0; i < 16; i++) {
		key[i] = 0;
	}

	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	}

	// Load fontset into memory
	for (int i = 0; i < 80; i++) {
		memory[i] = fontset[i];
	}
}

bool Chip8::loadProgram(std::string file)
{
	std::ifstream program(file, std::ios::in | std::ios::binary);
	
	if (program.is_open()) {
		file.seekg(0, file.end);
		int size = file.tellg();
		file.seekg(0, file.beg);

		if (size > 0x1000 - 0x200) {
			std::cerr << "File '" << file << "' exceeds memory size." << std::endl;
			return false;
		}

		char* buffer[] = new char[size];

		file.read(buffer, size);
		file.close();

		for (int i = 0; i < size; i++) {
			memory[i + 0x200] = buffer[i];
		}

		delete buffer[];
		return true;
	}
	
	return false;
}

void Chip8::run()
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// Decode opcode
	switch (opcode  0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				default:
					std::cout << "Unknown opcode [0x0000]: " << opcode << std::endl;	
			}

			break;
		case 0x1000:
			break;
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;
			break;
		case 0x3000:
			break;
		case 0x4000:
			break;
		case 0x5000:
			break;
		case 0x6000:
			int x = (opcode & 0x0F00) >> 8;
			V[x] = opcode & 0x00FF;
			pc += 2;
			break;
		case 0x7000:
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
				default:
					std::cout << "Unknown opcode [0x8000]: " << opcode << std::endl;
			}

			break;
		case 0x9000:
			break;
		case 0xA000:
			break;
		case 0xB000:
			break;
		case 0xC000:
			break;
		case 0xD000:
			break;
		case 0xE000:
			switch (opcode & 0x000F) {
				default:
					std::cout << "Unknown opcode [0xE000]: " << opcode << std::endl;
			}

			break;
		case 0xF000:
			switch (opcode & 0x000F) {
				default:
					std::cout << "Unknown opcode [0xF000]: " << opcode << std::endl;
			}

			break;
		default:
			std::cout << "Unknown opcode: " << opcode << std::endl;
	}
	// Execute opcode

	// Update timers
}
