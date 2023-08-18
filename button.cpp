#include "button.hpp"

extern SDL_Renderer* render;

Button :: Button() {
	buttonTexture = nullptr;
}

void Button :: build() {
	text.dispose();
	dispose();
	text.build();
}

void Button :: setText(Text text) {
	this->text = text;
}

Text& Button :: getText() {
	return text;
}

SDL_Rect Button :: getBounds() const {
	return buttonBounds;
}

void Button :: setBounds(int x, int y, int w, int h) {
	buttonBounds = {x, y, w, h};
}

void Button :: setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	buttonBackgroundColor = {r, g, b, a};
}

void Button :: draw() {
	auto [r, g, b, a] = buttonBackgroundColor;
	SDL_SetRenderDrawColor(render, r, g, b, a);        
    SDL_RenderFillRect(render, &buttonBounds);
	text.draw();
}

void Button :: dispose() {
	if(buttonTexture) SDL_DestroyTexture(buttonTexture);
	text.dispose();
	buttonTexture = nullptr;
}

bool Button :: checkClick(int xt, int yt) {
	auto [x, y, w, h] = buttonBounds;
	return xt >= x && xt <= x + w && yt >= y && yt <= y + h;
}
