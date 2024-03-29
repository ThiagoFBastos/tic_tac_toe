#ifndef TABLE_HPP
#define TABLE_HPP

#include "info.hpp"
#include <vector>

class Table {
	int rows, cols, space;
	SDL_Rect cellBounds;
	std :: vector<std :: vector<SDL_Color>> cellBG;
	public:
	Table();
	void setSize(int, int);
	void setSpace(int);
	void setBounds(int, int, int, int);
	void setBackgroundColor(int, int, Uint8, Uint8, Uint8, Uint8);
	void draw();
	bool checkClick(int, int, int&, int&);
	SDL_Rect getBounds() const;
};

#endif
