#ifndef _PRIEST_
#define _PRIEST_

#include "enemy.h"
#include "../limbs.h"
#include <cmath>

class demon;

class priest : public enemy{
public:
	priest(LTexture* sprt, int x, int y);
	~priest();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	void drawHalo(painter* p, LTexture* cube, int x, int y, double direction, int size, int counter, int colour);
	animation* movingAnim;
	double timer;
	double haloAngle;
	int haloColor;
	demon* shadowDevil;
};

class demon {
public:
	demon(LTexture* sprt, int X, int Y);
	~demon();
	
	void step(level*);
	void draw(painter*);
private:
	LTexture* spritesheet;
	int x, y;
	int face;
	
	double timer;
	
	//esqueletos para brazos
	limbSkeleton *brazoIzq, *brazoDer; 
	
	//Animaciones de Idle;
	limbAnim *idleIzqAnim, *idleDerAnim;
	
	//animaciones actuales para brazo izquierdo y derecho respectivamente
	limbAnim *currentIzqAnim, *currentDerAnim; 

};

#endif
