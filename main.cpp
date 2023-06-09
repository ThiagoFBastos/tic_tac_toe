#include <iostream>
#include <cstring>
#include <cerrno>

#include "info.hpp"
#include "game.hpp"
#include "menu.hpp"

#define WIDTH 300
#define HEIGHT 300

SDL_Window *window {};
SDL_Renderer* render {};

int main(int argc, char* argv[]) {
	GAME_STATUS status = GAME_STATUS :: MENU;

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std :: cout << strerror(errno) << '\n';
		return 0;
	}

	window = SDL_CreateWindow("TIC TAC TOE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if(!window) {
		std :: cout << strerror(errno) << '\n';
		return 0;
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(!render) {
		std :: cout << strerror(errno) << '\n';
		return 0;
	}

	int imgInitFlags = IMG_INIT_PNG | IMG_INIT_JPG;
			
	if(IMG_Init(imgInitFlags) != imgInitFlags) {
		std :: cout << strerror(errno) << '\n';
		return 0;
	}

	if(TTF_Init() == -1) {
		std :: cout << strerror(errno) << '\n';
		return 0;
	}

	Menu :: build();

	while(true) {
		bool quit {};
		switch(status) {
			case GAME_STATUS :: MENU:
				status = Menu :: run();
				break;
			case GAME_STATUS :: RUNNING:
				status = Game :: run();
				break;		
			case GAME_STATUS :: EXITING:
				quit = true;
				break;
		}
		if(quit) break;
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
