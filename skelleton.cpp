#include "skelleton.h"
#include "enemy.h"
#include <iostream>

skelleton::skelleton(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {5, 6};
	movingAnim = new animation(2, 0.2, true, spritesheet, frms, 32);
	
	unsigned int frms2[] = {7, 8, 9, 7};
	shootingAnim = new animation(3, 0.05, false, spritesheet, frms2, 32);
	
	currentAnim = movingAnim;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 2;
	maxSpeedY = 12;
	visible = 2;
	alpha = 255;
	
	spdX = 2;
	facingRight = true;
	onGround = true;
	
	colBox.x = x+10;
	colBox.y = y+9;
	colBox.w = 14;
	colBox.h = 24;
}

skelleton::~skelleton(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
	if (shootingAnim != NULL){
		delete shootingAnim;
		shootingAnim = NULL;
	}
}

void skelleton::step(level* lvl){
	
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
			timer = 2+rand()%4;
			if (onGround){
				spdY = -12;
				onGround = false;
			}
				
		} 
		
		colDisplace = lvl->vRaySolid(colBox.y, colBox.y+colBox.h+spdY-1, colBox.x+colBox.w/2);
		if (spdY<0){
			if (colDisplace != -1){
				//y = colDisplace*32+32;
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
		/*
		if (((colBox.y+colBox.h >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY;
		}
		*/
		
		enemy::step(lvl);
	
		colBox.x = x+10;
		colBox.y = y+9;
		
	}
}

void skelleton::draw(painter* pintor){
	enemy::draw(pintor);
}
