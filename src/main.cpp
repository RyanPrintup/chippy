#include "Chip8.h"

#include <iostream>
#include <SDL2/SDL.h>

const int SCALE = 10;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

SDL_Window* window = nullptr;
SDL_Surface* surface = nullptr;

Chip8 chip8;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL could not create window. SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	surface = SDL_GetWindowSurface(window);

	return true;
}

void draw()
{
	SDL_Rect rect;

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			rect.x = x * SCALE;
			rect.y = y * SCALE;
			rect.w = SCALE;
			rect.h = SCALE;

			if (chip8.gfx[(y * 64) + x] == 1) {
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));
			} else {
				SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));
			}
		}
	}
}

void handleInput(SDL_Event &event)
{
	switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_1: chip8.key[0x1] = 1; break;
				case SDLK_2: chip8.key[0x2] = 1; break;
				case SDLK_3: chip8.key[0x3] = 1; break;
				case SDLK_4: chip8.key[0xC] = 1; break;
				case SDLK_q: chip8.key[0x4] = 1; break;
				case SDLK_w: chip8.key[0x5] = 1; break;
				case SDLK_e: chip8.key[0x6] = 1; break;
				case SDLK_r: chip8.key[0xD] = 1; break;
				case SDLK_a: chip8.key[0x7] = 1; break;
				case SDLK_s: chip8.key[0x8] = 1; break;
				case SDLK_d: chip8.key[0x9] = 1; break;
				case SDLK_f: chip8.key[0xE] = 1; break;
				case SDLK_z: chip8.key[0xA] = 1; break;
				case SDLK_x: chip8.key[0x0] = 1; break;
				case SDLK_c: chip8.key[0xB] = 1; break;
				case SDLK_v: chip8.key[0xF] = 1; break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
				case SDLK_1: chip8.key[0x1] = 0; break;
				case SDLK_2: chip8.key[0x2] = 0; break;
				case SDLK_3: chip8.key[0x3] = 0; break;
				case SDLK_4: chip8.key[0xC] = 0; break;
				case SDLK_q: chip8.key[0x4] = 0; break;
				case SDLK_w: chip8.key[0x5] = 0; break;
				case SDLK_e: chip8.key[0x6] = 0; break;
				case SDLK_r: chip8.key[0xD] = 0; break;
				case SDLK_a: chip8.key[0x7] = 0; break;
				case SDLK_s: chip8.key[0x8] = 0; break;
				case SDLK_d: chip8.key[0x9] = 0; break;
				case SDLK_f: chip8.key[0xE] = 0; break;
				case SDLK_z: chip8.key[0xA] = 0; break;
				case SDLK_x: chip8.key[0x0] = 0; break;
				case SDLK_c: chip8.key[0xB] = 0; break;
				case SDLK_v: chip8.key[0xF] = 0; break;
			}
			break;
	}

}

void close()
{
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: Chippy rom" << std::endl << std::endl;
		return 1;
	}

	if (!chip8.loadProgram(argv[1])) {
		
		std::cout << "Could not load program " << argv[1] << std::endl;
		return 1;
	}

	if (!init()) {
		return 1;
	}

	// Clear screen
	SDL_FillRect(surface, NULL, 0);
	SDL_UpdateWindowSurface(window);

	bool running = true;
	SDL_Event event;
	
	while (running) {
		chip8.run();

		if (chip8.drawFlag) {
			draw();
			SDL_UpdateWindowSurface(window);
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			} else {
				handleInput(event);
			}
		}

		SDL_Delay(1);
	}

	close();
	return 0;
}
