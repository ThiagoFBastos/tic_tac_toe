#include "image.hpp"
#include <iostream>

extern SDL_Renderer* render;

Image :: Image() : imageTexture {} {}

void Image :: build() {
	dispose();
	SDL_Surface *imageSurface = IMG_Load(imagePath.c_str());
	if(!imageSurface) {
		std :: cout << "image: " << SDL_GetError() << '\n';
		exit(0);
	}
	imageTexture = SDL_CreateTextureFromSurface(render, imageSurface);
	if(!imageTexture) {
		std :: cout << "image: " << SDL_GetError() << '\n';
		exit(0);
	}
	SDL_FreeSurface(imageSurface);
}

void Image :: setImagePath(std :: string imagePath) {
	this->imagePath = imagePath;
}

void Image :: setBounds(int x, int y, int w, int h) {
	imageBounds = {x, y, w, h};
}

SDL_Rect Image :: getBounds() const {
	return imageBounds;
}
 
void Image :: draw() {
	SDL_RenderCopy(render, imageTexture, nullptr, &imageBounds);
}

void Image :: dispose() {
	if(imageTexture) SDL_DestroyTexture(imageTexture);
	imageTexture = nullptr;
}
