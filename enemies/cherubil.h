#ifndef _CHERUB_DEVIL_
#define _CHERUB_DEVIL_

#include "enemy.h"
#include "../particles.h"
#include <cmath>

class cherubil : public enemy{
public:
	cherubil(LTexture* sprt, int x, int y);
	~cherubil();
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	
	animation *cherubAnim, *devilAnim;
	int playerX, playerY;
	double normalized; //meh
	int timer;
};

class cherubilArrow : public enemyBullet{
public:
	cherubilArrow(LTexture* sprt, int X, int Y, double SPDX, double SPDY, bool TYPE);
	~cherubilArrow();
	
	void step(level*);
	void draw(painter*);
private:
	animation* shootAnim;
	double invAngle;
};

/*
class fireEmitter : public emitter{
public:
	fireEmitter(LTexture* sprt, int X, int Y);
	~fireEmitter();
	
	void step(level* lvl);
	void emit();
private:
	
};
*/

#endif
