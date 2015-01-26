#ifndef _DEMON_WANNABE_
#define _DEMON_WANNABE_

#include "enemy.h"

class imp : public enemy{
public:
	imp(LTexture* sprt, int x, int y);
	~imp();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	bool standing;
	double timer;
	bool onGround;
};

#endif
