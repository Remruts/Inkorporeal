#ifndef _PUERTA_
#define _PUERTA_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "LTexture.h"
#include "painter.h"
#include "animation.h"
#include "level.h"

class door{
public:
	door(LTexture*, int X, int Y, bool Locked);
	~door();
	
	void step();
	void draw(painter*);
	SDL_Rect* getColBox();
	
	bool isOpened();
	void open();
	void unlock();
	
private:
	int x, y;
	double timer;
	LTexture* spritesheet;
	animation* doorAnim;
	SDL_Rect colBox;
	
	bool opened, locked;
};

class key{
public:
	key(LTexture*, int X, int Y);
	~key();
	
	void step(level*);
	void draw(painter*);
	SDL_Rect* getColBox();
	
	void pick();
	bool wasPicked();
	void use();
	bool wasUsed();
	
	void setPos(int, int);
	
private:
	int x, y;
	double counter;
	bool pickedup, used;
	LTexture* spritesheet;
	SDL_Rect colBox;
};

#endif
