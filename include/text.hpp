#ifndef TEXT_HPP
#define TEXT_HPP

#include "info.hpp"
#include <string>

class Text {
	SDL_Texture* textTexture;
	SDL_Rect textBounds;
	SDL_Color textColor;
	std :: string text;
	TTF_Font *textFont;
	public:
	Text();
	void build();
	void setText(std :: string);
	void setColor(Uint8, Uint8, Uint8, Uint8);
	void setBounds(int, int, int, int);
	void setFont(TTF_Font*);
	void draw();
	void dispose();
	SDL_Rect getBounds() const;
};

#endif
