#include "game.hpp"
#include "menu.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

extern SDL_Window* window;
extern SDL_Renderer* render;

#define WIDTH 300
#define TITLE_HEIGHT 50
#define HEIGHT 400

Table Game :: table;
int Game :: turnPlayer = 0;
Enemy Game :: adv;
TTF_Font* Game :: textFont {};
bool Game :: isRunning {};
Text Game :: notif;
Button Game :: btnReset;
int Game :: n {};

void Game :: build(int n) {
	dispose();

	Game :: n = n;

	table.setSize(n, n);
	table.setBounds(0, TITLE_HEIGHT, WIDTH / n, (HEIGHT - 60 - TITLE_HEIGHT) / n);
	table.setSpace(1);

	adv = Enemy(n);
	adv.load();

	srand(clock());

	turnPlayer = rand() % 100 < 50 ? -1 : 1;

	isRunning = true;

	textFont = TTF_OpenFont("data/Roboto-Regular.ttf", 80);

	if(!textFont) {
		std :: cout << "game: " << SDL_GetError() << '\n';
		exit(0);
	}

	if(turnPlayer == -1) {
		notif.setText("You begin!");
		notif.setColor(0xff, 0x00, 0x00, 0xff);
	} else {
		notif.setText("I begin!");
		notif.setColor(0x00, 0x00, 0xff, 0xff);
	}

	notif.setFont(textFont);
	notif.setBounds((WIDTH - 100) / 2, 10, 100, 30);
	notif.build();

	Text& txtReset = btnReset.getText();

	int midX = (WIDTH - 90) / 2;
	int midY = (table.getBounds().y + table.getBounds().h + HEIGHT - 30) / 2;

	btnReset.setBounds(midX, midY, 90, 30);
	btnReset.setBackgroundColor(80, 81, 82, 255);

	txtReset.setText("reset");
	txtReset.setFont(textFont);
	txtReset.setColor(0xff, 0xff, 0xff, 0xff);

	midX = (2 * btnReset.getBounds().x + btnReset.getBounds().w - 60) / 2;
	midY = (2 * btnReset.getBounds().y + btnReset.getBounds().h - 25) / 2;

	txtReset.setBounds(midX, midY, 60, 25);

	btnReset.build();

	SDL_SetWindowSize(window, WIDTH, HEIGHT);
}

GAME_STATUS Game :: run() {
	GAME_STATUS status = GAME_STATUS :: RUNNING;
	SDL_Event e;
	bool completed = false;

	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
			status = GAME_STATUS :: MENU;
			Menu :: build();
		} else if(e.type == SDL_MOUSEBUTTONDOWN) {
			int x = e.button.x, y = e.button.y, r, c;
			if(table.checkClick(x, y, r, c)) {
				if(isRunning && adv.can(r, c) && turnPlayer == -1) {
					table.setBackgroundColor(r, c, 0xff, 0x00, 0x00, 0xff);
					adv.put(r, c, 0);
					completed = true;
				}
			} else if(btnReset.checkClick(x, y)) {
				build(n);
				return GAME_STATUS :: RUNNING;
			}
		}
	}

	if(turnPlayer == 1 && isRunning) {
		auto [x, y] = adv.next();
		adv.put(x, y, 1);
		table.setBackgroundColor(x, y, 0x00, 0x00, 0xff, 0xff);
		completed = true;
	}

	if(isRunning) {
		int st = adv.getStatus();
		if(st != ONGOING) {
			if(st == LOOSE) {
				notif.setText("You Win!");
				notif.setColor(0xff, 0x00, 0x00, 0xff);
			} else if(st == WIN) {
				notif.setText("I Win!");
				notif.setColor(0x00, 0x00, 0xff, 0xff);
			} else {
				notif.setText("Draw");
				notif.setColor(0xff, 0xff, 0x00, 0xff);
			}
			notif.build();
			isRunning = false;
		}
	}

	SDL_SetRenderDrawColor(render, 0xf6, 0xf6, 0xf6, 0xf6);
	SDL_RenderClear(render);
	table.draw();
	notif.draw();
	btnReset.draw();
	SDL_RenderPresent(render);
	if(status != GAME_STATUS :: RUNNING) dispose();
	if(completed) turnPlayer *= -1;
	return status;
}

void Game :: dispose() {
	adv.dispose();
	notif.dispose();
	if(textFont) TTF_CloseFont(textFont);
	textFont = nullptr;
}
