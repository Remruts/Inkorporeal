#ifndef _OLD_BONES_
#define _OLD_BONES_

#include "enemy.h"

class skelleton : public enemy{
public:
	skelleton(LTexture* sprt, int x, int y);
	~skelleton();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	animation* shootingAnim;
	double timer;
	bool onGround;
};

#endif
