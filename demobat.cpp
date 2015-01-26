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
	onGround = false;
	
	colBox.x = x+12;
	colBox.y = y+15;
	colBox.w = 10;
	colBox.h = 10;
	
	lives = 1;
}

demobat::~demobat(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void demobat::step(level* lvl){
	
	if (alive){
		
		int colDisplace = lvl->vRaySolid(colBox.y+colBox.h, colBox.y+colBox.h+spdY+1, colBox.x+colBox.w/2);
		if (colDisplace != -1){
			y = colDisplace*32;
			spdY = 0;
			onGround = true;
		} else {
			onGround = false;
			spdY += 0.5;
		}
			
		timer -= 0.1;
		
		if (timer <= 0){
			srand(time(NULL)+(long int)(this));
			timer = 1+rand()%3;
			spdY = -7;
			onGround = false;
		} 
		
		colDisplace = lvl->vRaySolid(colBox.y, colBox.y+colBox.h+spdY-1, colBox.x+colBox.w/2);
		if (spdY<0){
			if (colDisplace != -1){
				//y = colDisplace*32;
				spdY = 0;
			}
		}
		
		if (facingRight){
			colDisplace = lvl->hRaySolid(colBox.x+colBox.w, colBox.x+colBox.w+spdX, colBox.y+colBox.h/2+spdY);
		} else {
			colDisplace = lvl->hRaySolid(colBox.x, colBox.x+spdX, colBox.y+colBox.h/2+spdY);
		}
		
		
		if (colDisplace != -1){
			x = 192+colDisplace*32;
			facingRight = !facingRight;
			spdX *= -1;
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
