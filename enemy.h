#ifndef _ENEMY_INCLUDED_
#define _ENEMY_INCLUDED_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "LTexture.h"
#include "painter.h"
#include "animation.h"
#include "level.h"

//class level; // fd ftw

class enemy{
public:
	
	enemy(LTexture* sprt, int x, int y);
	virtual ~enemy();
	
	virtual void step(level*);
	virtual void draw(painter*);
	
	void getPos(int &x, int &y) const;
	SDL_Rect* getColBox();
	
	virtual void getHurt();
	
	bool isAlive();
protected:
	int x, y;
	double spdX, spdY;
	double maxSpeedX, maxSpeedY;
	double accel;
	int visible;
	
	int lives;
	bool hurt;
	double hurtTimer;
	bool alive;
	
	animation* currentAnim;
	int alpha;
	
	SDL_Rect colBox;
	
	bool facingRight; //si veo hacia la derecha, para flip de textura
	LTexture* spritesheet;
};

#endif

