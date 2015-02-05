#ifndef _POINTS_
#define _POINTS_


#include "LTexture.h"
#include "painter.h"

class movingPoints{
public:
	movingPoints(LTexture*, int X, int Y, int p);
	~movingPoints();
	
	void step();
	void draw(painter*);
	
	bool isAlive();
	
private:
	LTexture* spritesheet;
	int x, y;
	int life, maxLife;
	bool alive;
	int points;
	int power, divPoints, maxI;
};

#endif
