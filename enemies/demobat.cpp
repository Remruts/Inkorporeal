#include "demobat.h"
#include "enemy.h"
#include <iostream>

demobat::demobat(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {15, 16, 17};
	movingAnim = new animation(3, 0.3, true, spritesheet, frms, 32);
		
	currentAnim = movingAnim;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 2;
	maxSpeedY = 7;
	visible = 2;
	alpha = 255;
	
	spdX = 2;
	facingRight = true;
	
	colBox.x = x+12;
	colBox.y = y+15;
	colBox.w = 10;
	colBox.h = 10;
	
	lives = 1;
	maxLives = lives;
}

demobat::~demobat(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void demobat::step(level* lvl){
	
	if (alive){
		
		int colDisplace;
		if (spdY>=0){
			colDisplace = lvl->vRaySolid(colBox.y+colBox.h, colBox.y+colBox.h+spdY+1, colBox.x+colBox.w/2);
		} else{
			colDisplace = lvl->vRaySolid(colBox.y, colBox.y+spdY-1, colBox.x+colBox.w/2);
		}
		
		if (colDisplace != -1){
			
			spdY = 0;
			if (spdY >= 0){
				y = colDisplace*32;
			} else {
				y = colDisplace*32+33;
			}
			colBox.y = y+9;
				
		} else {
			spdY += 0.5;
		}
		
		
			
		timer -= 0.1;
		
		if (timer <= 0){
			srand(time(NULL)+(long int)(this));
			timer = 1+rand()%3;
			spdY = -7;
			lvl->playSound("batSound");
		} 
		
		
		
		if (facingRight){
			colDisplace = lvl->hRaySolid(colBox.x+colBox.w, colBox.x+colBox.w+spdX+1, colBox.y+colBox.h/2+spdY);
		} else {
			colDisplace = lvl->hRaySolid(colBox.x, colBox.x+spdX-1, colBox.y+colBox.h/2+spdY);
		}
		
		
		if (colDisplace != -1){
			if (facingRight)
				x = 192+colDisplace*32-32+(colBox.w+(colBox.x-x));
			else
				x = 192+colDisplace*32-(colBox.x-x);
			facingRight = !facingRight;
			spdX *= -1;
			colBox.x = x+12;
		}
	
		if (((colBox.x+colBox.w >= 1120) && (spdX > 0)) || ((colBox.x <= 224) && (spdX < 0))){
			spdX = -spdX;
		}
		
		if (((colBox.y+colBox.h >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY;
		}
		
		
		enemy::step(lvl);
	
		colBox.x = x+12;
		colBox.y = y+15;
		
	}
}

void demobat::draw(painter* pintor){
	enemy::draw(pintor);
}
