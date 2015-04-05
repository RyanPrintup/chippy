#include "Chip8.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

	srand (time(NULL);
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
	switch (opcode &  0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000: // Clears the screen
					for (int i = 0; i < 64 * 32; i++) {
						gfx[i] = 0;
					}
					
					drawFlag = true;
					pc += 2;
					break;
				case 0x000E: // Returns from a subroutine
					sp--;
					pc = stack[sp];
					pc += 2;
					break;
				default:
					std::cout << "Unknown opcode [0x0000]: " << opcode << std::endl;	
			}
			break;
		case 0x1000: // Jumps to address NNN
			pc = opcode & 0x0FFF;
			break;
		case 0x2000: // Calls subroutine at NNN
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;
			break;
		case 0x3000: // Skips the next instruction if VX equals NN
			int x = (opcode & 0x0F00) >> 8;
			
			if (V[x] == (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x4000: // Skips the next instruction if VX doesn't equal NN
			int x = (opcode & 0x0F00) >> 8;

			if (V[x] != (opcode & 0x00FF)) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x5000: // Skips the next instruction if VX equals VY
			int x = (opcode & 0x0F00) >> 8;
			int y = (opcode & 0x00F0) >> 4;

			if (V[x] == V[y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x6000: // Sets VX to NN
			int x = (opcode & 0x0F00) >> 8;
			
			V[x] = opcode & 0x00FF;
			pc += 2;
			break;
		case 0x7000: // Adds NN to VX
			int x = opcode & 0x0F00;
			
			V[x] += opcode & 0x00FF;
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
				case 0x0000: // Sets VX to the value of VY
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;

					V[x] = V[y];
					pc += 2;
					break;
				case 0x0001: // Sets VX to "VX OR VY"	
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;
					
					V[x] |= V[y];	
					pc += 2;
					break;
				case 0x0002: // Sets VX to "VX AND VY"
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;
					
					V[x] &= V[y];
					pc += 2;
					break;
				case 0x0003: // Sets VX to "VX XOR VY"
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;
					
					V[x] ^= V[y];
					pc += 2;
					break;
				case 0x0004: // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;
					
					if (V[y] > (0xFF - V[x])) {
						V[0xF] = 1; // Carry
					} else {
						V[0xF] = 0;
					}

					V[x] += V[y];
					pc += 2;
					break;
				case 0x0005: // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;
					
					if (V[y] > V[x]) {
						V[0xF] = 0; // Borrow
					} else {
						V[0xF] = 1;
					}

					V[x] -= V[y];
					pc += 2;
					break;
				case 0x0006: // Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					int x = (opcode & 0x0F00) >> 8;
					
					V[0xF] = V[x] & 0x1;
					V[x] >>= 1;
					pc += 2;
					break;
				case 0x0007: // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					int x = (opcode & 0x0F00) >> 8;
					int y = (opcode & 0x00F0) >> 4;

					if (V[x] > V[y]) {
						V[0xF] = 0; // Borrow
					} else {
						V[0xF] = 1;
					}

					V[x] = V[y] - V[x];
					pc += 2;
					break;
				case 0x000E: // Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
					int x = (opcode & 0x0F00) >> 8;
					
					V[0xF] = V[x] >> 7;
					V[x] <<= 1;
					pc += 2;
					break;
				default:
					std::cout << "Unknown opcode [0x8000]: " << opcode << std::endl;
			}
			break;
		case 0x9000: // Skips the next instruction if VX doesn't equal VY
			int x = (opcode & 0x0F00) >> 8;
			int y = (opcode & 0x00F0) >> 4;

			if (V[x] != V[y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0xA000: // Sets I to the address NNN
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000: // Jumps to the address NNN plus V0
			pc = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000: // Sets VX to a random number, masked by NN
			int x = (opcode & 0x0F00) >> 8;
			
			V[x] = (rand() % 0xFF) & (opcodede & 0x00FF);
			pc += 2;
			break;
		case 0xD000: // To-do drawing opcode
			break;
		case 0xE000:
			switch (opcode & 0x000F) {
				case 0x000E: // Skips the next instruction if the key stored in VX is pressed
					int x = (opcode & 0x0F00) >> 8;
					
					if (key[V[x]]) {
						pc += 4;
					} else {
						pc += 2;
					}
					break;
				case 0x0001: // Skips the next instruction if the key stored in VX isn't pressed
					int x = (opcode & 0x0F00) >> 8;
					
					if (!key[V[x]]) {
						pc += 4;
					} else {
						pc += 2;
					}
					break;
				default:
					std::cout << "Unknown opcode [0xE000]: " << opcode << std::endl;
			}
			break;
		case 0xF000:
			switch (opcode & 0x000F) {
				case 0x0007: // Sets VX to the value of the delay_timer
					int x = (opcode & 0x0F00) >> 8;
					
					V[x] = delay_timer;
					pc += 2;
					break;
				case 0x000A:
					break;i
				case 0x0015: // Sets the delay_timer to VX
					int x = (opcode & 0x0F00) >> 8;
					
					delay_timer = V[x];
					pc += 2;
					break;
				case 0x0018: // Sets the sound_timer to VX
					int x = (opcode & 0x0F00) >> 8;
					
					sound_timer = V[x];
					pc += 2;
					break;
				case 0x001E: // Adds VX to I
					int x = (opcode & 0x0F00) >> 8;
					
					if ((I + V[x]) > 0xFFF) { // VF is set to 1 when range overflow (I+VX>0xFFF), and 0 when there isn't
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}

					I += V[x];
					pc += 2;
					break;
				case 0x0029: // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					int x = (opcode & 0x0F00) >> 8;
					
					I = V[x] * 0x5;
					pc += 2;
					break;
				case 0x0033:
					/* 
					 * Stores the Binary-coded decimal representation of VX, with the most significant
					 * of three digits at the address in I, the middle digit at I plus 1, and the least
					 * significant digit at I plus 2.
					 */
					int x = (opcode & 0x0F00) >> 8;
					
					memory[I]     = V[x] / 100;
					memory[I + 1] = (V[x] / 10) % 10;
					memory[I + 2] = (V[x] % 100) % 10;
					pc += 2;
					break;
				case 0x0055: // Stores V0 to VX in memory starting at address I
					int x = (opcode & 0x0F00) >> 8;
					
					for (int i = 0; i < x; i++) {
						memory[I + i] = V[i];
					}

					// On the original interpreter, when the operation is done, I=I+X+1
					I += x + 1;
					pc += 2;
					break;
				case 0x0065: // Fills V0 to VX with values from memory starting at address I
					int x = (opcode & 0x0F00) >> 8;

					for (int i = 0; i < x; i++) {
						V[i] = memory[I + i];
					}

					// On the original interpreter, when the operation is done, I=I+X+1
					I += x + 1;
					pc += 2;
					break;
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
