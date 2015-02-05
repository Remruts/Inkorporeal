#ifndef _JACKO_DARKO_
#define _JACKO_DARKO_

#include "enemy.h"
#include "../particles.h"
#include <cmath>

class jack : public enemy{
public:
	jack(LTexture* sprt, int x, int y);
	~jack();
	
	virtual void step(level*);
	virtual void draw(painter*);
private:

	enum JState {stRandom, stPursue, stEscape, stShoot};
	
	animation* movingAnim;
	int playerX, playerY;
	double normalized; //meh
	JState state;
	int timer;
};

class jackFire : public enemyBullet{
public:
	jackFire(LTexture* sprt, int X, int Y, double SPDX, double SPDY);
	~jackFire();
	
	void step(level*);
	void draw(painter*);
private:
	emitter* em;
	animation* fireAnim;
};

class fireEmitter : public emitter{
public:
	fireEmitter(LTexture* sprt, int X, int Y);
	~fireEmitter();
	
	void step(level* lvl);
	void emit();
private:
	
};

#endif
