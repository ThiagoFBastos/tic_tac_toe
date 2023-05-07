#include "text.hpp"
#include <iostream>

extern SDL_Renderer* render;

Text :: Text() {
	textTexture = nullptr;
	textFont = nullptr;
}

void Text :: build() {
	dispose();
	SDL_Surface* textSurface = TTF_RenderText_Solid(textFont, text.c_str(), textColor);
	if(!textSurface) {
		std :: cout << "text: " << SDL_GetError() << '\n';
		exit(0);
	}
	textTexture = SDL_CreateTextureFromSurface(render, textSurface);
	if(!textTexture) {
		std :: cout << "text: " << SDL_GetError() << '\n';
		exit(0);
	}
	SDL_FreeSurface(textSurface);
}

void Text :: setText(std :: string text) {
	this->text = text;
}

void Text :: setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	textColor = {r, g, b, a};
}

void Text :: setBounds(int x, int y, int w, int h) {
	textBounds = {x, y, w, h};
}

void Text :: setFont(TTF_Font* textFont) {
	this->textFont = textFont;
}

void Text :: draw() {
	SDL_RenderCopy(render, textTexture, nullptr, &textBounds);
}

void Text :: dispose() {
	if(textTexture) SDL_DestroyTexture(textTexture);
	textTexture = nullptr;
}
