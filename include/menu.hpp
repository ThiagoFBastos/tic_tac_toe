#ifndef MENU_HPP
#define MENU_HPP

#include "info.hpp"
#include "button.hpp"
#include "image.hpp"

class Menu {
	static Button x33;
	static Button x44;
	static TTF_Font *textFont;
	static Image topImage;
	public:
	static void build();
	static GAME_STATUS run();
	static void dispose();
};

#endif
