#ifndef _PRIEST_
#define _PRIEST_

#include "enemy.h"

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
};

#endif
