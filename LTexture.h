#ifndef _LTEXTURE_
#define _LTEXTURE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Clase sin sentido
//Sólo importa el tamaño.
class LTexture { 
public: 
	LTexture(int w, int h, SDL_Texture* tex); 
	~LTexture(); 
	//Gets image dimensions 
	int getWidth(); 
	int getHeight();
	
	void setAlpha(int);
	void setColor(int, int, int);
	
	//gets texture
	SDL_Texture* getTexture();
	
private: 
	//The actual hardware texture 
	SDL_Texture* tex;
	//Image dimensions 
	int w; int h;
	int alpha; //alpha
	int r, g, b; //color
};

#endif
