#ifndef _PAINTER_
#define _PAINTER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "LTexture.h"

#define GOLDEN_RATIO_CONJUGATE 0.618033988749895

using namespace std;

class painter{
public:
	
	painter(SDL_Renderer* screen);
	~painter();
		
	LTexture* loadTexture(const string&);
	LTexture* textureFromSurface(SDL_Surface*);
	LTexture* textureFromText(const string& textureText, int size, unsigned char r, unsigned char g, unsigned char b);
	void freeTexture(LTexture*);
	
	//
	void draw(LTexture *tex, int srcX, int srcY, int srcW, int srcH, int screenX, int screenY);
	//SDL_RendererFlip == SDL_FLIP_NONE | SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
	//0 = nada, 1 = horizontal, 2 = vertical
	void drawEx(LTexture *tex, int srcX, int srcY, int srcW, int srcH, 
		int screenX, int screenY, int screenW, int screenH, 
		const double angle, int flip);
		
	void setRenderTarget(LTexture*); 	//Setea a una textura como lugar donde dibujar
	void resetRenderTarget(); 		 	//Vuelve a setear a la pantalla como lugar donde se dibuja
	
	//Esto no lo vamos a usar, supongo...
	SDL_Surface* loadImage(const string& path);
	void freeImage(SDL_Surface*);
	
	//Dibujar cosas lindas
	void setColor(int r, int g, int b, int a); 				// setea el color para dibujar estas cosas
	void setBlendMode(int blend); 							// 0=none, 1=blend, 2=add, 3=modulate;
	void drawRect(int x, int y, int w, int h, bool fill);	//dibuja un rectángulo
	void drawLine(int x1, int y1, int x2, int y2);			//dibuja una línea
	
	void clear(); //limpia la pantalla
	
	//generador de colores aleatorio
	void setSaturation(unsigned int S);
	void setValue(unsigned int V);
	void getRandomColor(unsigned int &R, unsigned int &G, unsigned int &B);
	void hsvToRgb(unsigned int H, double V, double S, unsigned int &R, unsigned int &G, unsigned int &B);

private:
	SDL_Renderer *canvas;
	SDL_Rect rect;
	SDL_Rect screen_rect;
	
	//Fuente Ubuntu
	TTF_Font* eightbit8;
	TTF_Font* ubuntuFont24;
	TTF_Font* ubuntuFont32;
	TTF_Font* ubuntuFont48;
	SDL_Color textColor;
	
	double randomHue;
	double randomValue;
	double randomSaturation;
	
};

#endif
