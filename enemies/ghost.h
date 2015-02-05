#ifndef _SPOOOOKY_BOOOO_
#define _SPOOOOKY_BOOOO_

#include "enemy.h"

class ghost : public enemy{
public:
	ghost(LTexture* sprt, int x, int y);
	~ghost();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	double timer;
};

#endif
