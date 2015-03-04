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
	
	double angBrazoIzq, angBrazoDer, angAnteIzq, angAnteDer, angManoIzq, angManoDer;
	double timer;
	
	limb *brazoIzq, *brazoDer; //brazos
	limb *antebrazoIzq, *antebrazoDer; //antebrazos
	limb *manoIzq, *manoDer; //manos
};

#endif
