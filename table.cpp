#include "table.hpp"

extern SDL_Renderer* render;

Table :: Table() {}

void Table :: setSize(int r, int c) {
	rows = r;
	cols = c;
	cellBG.resize(r);
	for(auto& row : cellBG) row.assign(c, {});
}

void Table :: setBounds(int x, int y, int w, int h) {
	cellBounds = {x, y, w, h};
}

void Table :: setBackgroundColor(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	cellBG[x][y] = {r, g, b, a};
}

void Table :: draw() {
	auto [x0, y0, w, h] = cellBounds;
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			SDL_Rect rect = {x0 + i * (w + space), y0 + j * (h + space), w, h};
			auto [r, g, b, a] = cellBG[i][j];
			SDL_SetRenderDrawColor(render, r, g, b, a);
			SDL_RenderFillRect(render, &rect);
		}
	}
}

void Table :: setSpace(int space) {
	this->space = space;
}

SDL_Rect Table :: getBounds() const {
	int w = cellBounds.w * cols + space * (cols - 1);
	int h = cellBounds.h * rows + space * (rows - 1);
	return {cellBounds.x, cellBounds.y, w, h};
}

bool Table :: checkClick(int x, int y, int& r, int& c) {
	auto [x0, y0, w, h] = cellBounds;
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			int a = x0 + i * (w + space), b = y0 + j * (h + space);
			if(x >= a && x <= a + w && y >= b && y <= b + h) {
				r = i, c = j;
				return true;
			}
		}
	}
	return false;
}
