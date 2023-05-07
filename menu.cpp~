#include "menu.hpp"
#include "text.hpp"
#include "game.hpp"

extern SDL_Window* window;
extern SDL_Renderer* render;

#define WIDTH 300
#define HEIGHT 300
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define TEXT_BUTTON_WIDTH 30
#define TEXT_BUTTON_HEIGHT 30

Button Menu :: x33;
Button Menu :: x44;
TTF_Font* Menu :: textFont = nullptr;
Image Menu :: topImage;

void Menu :: build() {
	SDL_SetWindowSize(window, WIDTH, HEIGHT);

	dispose();
	textFont = TTF_OpenFont("data/Roboto-Regular.ttf", 80);

	topImage.setImagePath("data/XO.jpg");
	topImage.setBounds((WIDTH - 100) / 2, 20, 100, 100);
	topImage.build();

	Text& x33Text = x33.getText();
	Text& x44Text = x44.getText();

	x33.setBounds((WIDTH - BUTTON_WIDTH) / 2, (HEIGHT - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
	x33.setBackgroundColor(0xC5, 0xC1, 0xCD, 0xFF);

	x44.setBounds((WIDTH - BUTTON_WIDTH) / 2, BUTTON_HEIGHT + 5 + (HEIGHT - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
	x44.setBackgroundColor(0xC5, 0xC1, 0xCD, 0xFF);

	SDL_Rect bounds = x33.getBounds();

	x33Text.setText("3x3");
	x33Text.setColor(0, 0, 0, 0);
	x33Text.setFont(textFont);
	x33Text.setBounds(bounds.x + (bounds.w - TEXT_BUTTON_WIDTH) / 2, bounds.y + (bounds.h - TEXT_BUTTON_HEIGHT) / 2, TEXT_BUTTON_WIDTH, TEXT_BUTTON_HEIGHT);

	bounds = x44.getBounds();

	x44Text.setText("4x4");
	x44Text.setColor(0, 0, 0, 0);
	x44Text.setFont(textFont);
	x44Text.setBounds(bounds.x + (bounds.w - TEXT_BUTTON_WIDTH) / 2, bounds.y + (bounds.h - TEXT_BUTTON_HEIGHT) / 2, TEXT_BUTTON_WIDTH, TEXT_BUTTON_HEIGHT);

	x33.build();
	x44.build();
}

GAME_STATUS Menu :: run() {
	SDL_Event e;
	GAME_STATUS status = GAME_STATUS :: MENU;
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) status = GAME_STATUS :: EXITING;
		else if(e.type == SDL_MOUSEBUTTONDOWN) {
			int x = e.button.x, y = e.button.y;
			if(x33.checkClick(x, y)) {
				Game :: build(3);
				status = GAME_STATUS :: RUNNING;
			} else if(x44.checkClick(x, y)) {
				Game :: build(4);
				status = GAME_STATUS :: RUNNING;
			}
		}
	}
	SDL_SetRenderDrawColor(render, 0xf6, 0xf6, 0xf6, 0xf6);
	SDL_RenderClear(render);
	x33.draw();
	x44.draw();
	topImage.draw();
	SDL_RenderPresent(render);
	if(status != GAME_STATUS :: MENU) dispose();
	return status;
}

void Menu :: dispose() {
	x33.dispose();
	x44.dispose();
	topImage.dispose();
	if(textFont) TTF_CloseFont(textFont);
	textFont = nullptr;
}
