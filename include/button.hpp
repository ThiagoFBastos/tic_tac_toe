#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include "info.hpp"
#include "text.hpp"

class Button {
	SDL_Texture* buttonTexture;
	Text text;
	SDL_Rect buttonBounds;
	SDL_Color buttonBackgroundColor;
	public:
	Button();
	void build();
	void setText(Text);
	Text& getText();
	SDL_Rect getBounds() const;
	void setBounds(int, int, int, int);
	void setBackgroundColor(Uint8, Uint8, Uint8, Uint8);
	void draw();
	void dispose();
	bool checkClick(int, int);
};

#endif
