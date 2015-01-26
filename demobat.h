#ifndef _WINGS_N_EYE_
#define _WINGS_N_EYE_

#include "enemy.h"

class demobat : public enemy{
public:
	demobat(LTexture* sprt, int x, int y);
	~demobat();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	double timer;
	bool onGround;
};

#endif
