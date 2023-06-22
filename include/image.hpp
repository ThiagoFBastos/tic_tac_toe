#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "info.hpp"
#include <string>

class Image {
	SDL_Rect imageBounds;
	SDL_Texture* imageTexture;
	std :: string imagePath;
	public:	
	Image();	
	void build();
	void setImagePath(std :: string);
	void setBounds(int, int, int, int);
	void draw();
	void dispose();
};

#endif
