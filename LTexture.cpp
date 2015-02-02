#include "LTexture.h"
#include <iostream> //debug
//Implementación de LTexture.h

LTexture::LTexture(int width, int height, SDL_Texture* texture){
	w = width;
	h = height;
	tex = texture;
	alpha = 255;
	
	r = 255;
	g = 255;
	b = 255;
}

LTexture::~LTexture(){
	SDL_DestroyTexture(tex);
}
//Gets image dimensions 
int LTexture::getWidth(){
	return w;
}
int LTexture::getHeight(){
	return h;
}

void LTexture::setAlpha(int a){
	alpha = a;
	if (alpha<0)
		alpha = 0;
	if (alpha>255)
		alpha = 255;
		
	SDL_SetTextureAlphaMod(tex, alpha);
}

void LTexture::setColor(int R, int G, int B){
	r = R;
	g = G;
	b = B;
	
	if (r>255)
		r = 255;
	if (g>255)
		g = 255;
	if (b>255)
		b = 255;
		
	if (r<0)
		r = 0;
	if (g<0)
		g = 0;
	if (b<0)
		b = 0;
	
	SDL_SetTextureColorMod(tex, r, g, b);
}

void LTexture::setBlendMode(int blend){
	if (blend == 1){
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
	} else if (blend == 2){
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_MOD);
	} else {
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	}
	
}

//gets texture
SDL_Texture* LTexture::getTexture(){
	return tex;
}
