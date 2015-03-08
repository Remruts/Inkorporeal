#ifndef _ENEMY_INCLUDED_
#define _ENEMY_INCLUDED_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "../LTexture.h"
#include "../painter.h"
#include "../animation.h"
#include "../level.h"

//class level; // fd ftw

class enemy{
public:
	
	enemy(LTexture* sprt, int x, int y);
	virtual ~enemy();
	
	virtual void step(level*);
	virtual void draw(painter*);
	
	void setPos(int x, int y);
	
	void getPos(int &x, int &y) const;
	
	int getLives();
	int getMaxLives();
	
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
	int maxLives;
	bool hurt;
	double hurtTimer;
	bool alive;
	
	animation* currentAnim;
	int alpha;
	
	SDL_Rect colBox;
	
	bool facingRight; //si veo hacia la derecha, para flip de textura
	LTexture* spritesheet;
};

//Seh, muy parecido a enemy, pero no lo voy a heredar, for reasons
class enemyBullet{
public:
	enemyBullet(LTexture* sprt, int X, int Y, double SDPX, double SPDY);
	virtual ~enemyBullet();
	
	virtual void step(level*);
	virtual void draw(painter*);
	
	void getPos(int &x, int &y) const;
	SDL_Rect* getColBox();
	
	void setSpeed(double SPDX, double SPDY);
	
	bool isAlive();
	virtual void die();
	
protected:
	double x, y;
	double spdX, spdY;
	double accelX, accelY;
	int visible;
	int alpha;
	int angle;
	int life;
	bool alive;
	SDL_Rect colBox;
	LTexture* spritesheet;
	animation* currentAnim;
};

#endif
