#ifndef _THE_MASKED_MAN_WHITHOUT_THE_MAN_
#define _THE_MASKED_MAN_WHITHOUT_THE_MAN_

#include "enemy.h"
#include "../particles.h"

class mask : public enemy{
public:
	mask(LTexture* sprt, int x, int y);
	~mask();
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	int playerX, playerY;
	double normalized; //meh
	int timer;
};

#endif
