#include "points.h"

movingPoints::movingPoints(LTexture* sprt, int X, int Y, int Points){
	spritesheet = sprt;
	x = X;
	y = Y;
	alive = true;
	points = Points;
	life = 40;
	maxLife = life;
	
	divPoints = points;
	maxI = 0;
	while(divPoints != 0){
		divPoints /= 10;
		maxI++;
	}
	
}

movingPoints::~movingPoints(){
	
}
	
void movingPoints::step(){
	y -= 1;
	life -=1;
	if (life <= 0)
		alive = false;
}

void movingPoints::draw(painter* p){
	
	spritesheet->setAlpha(255*(life/double(maxLife)));
	divPoints = points;
	for (int j = 0; j<maxI; j++){
		power = divPoints%10;
		p->drawEx(spritesheet, 8*power, 0, 8, 8, x-maxI*4+8*(maxI-j), y, 8, 8, 0, 0);
		divPoints /= 10;
		if (divPoints == 0)
			break;
	}
	spritesheet->setAlpha(255);
}

bool movingPoints::isAlive(){
	return alive;
}
