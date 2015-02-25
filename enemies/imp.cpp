#include "imp.h"
#include "enemy.h"
#include <iostream>

imp::imp(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {12, 13, 14, 13};
	movingAnim = new animation(2, 0.2, true, spritesheet, frms, 32);
		
	currentAnim = movingAnim;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 3;
	maxSpeedY = 12;
	visible = 2;
	alpha = 255;
	
	spdX = 0;
	facingRight = true;
	onGround = false;
	standing = true;
	
	colBox.x = x+12;
	colBox.y = y+9;
	colBox.w = 14;
	colBox.h = 24;
	
	lives = 1;
	maxLives = lives;
}

imp::~imp(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void imp::step(level* lvl){
	
	if (alive){
	
		if (standing){
			spdX = 0;
			movingAnim->setSpeed(0);
			movingAnim->setCurrentFrame(0);
		} else {
			spdX = maxSpeedX*(2*facingRight-1);
			movingAnim->setSpeed(0.2);
		}
		
		int colDisplace;
		if (spdY>=0){
			colDisplace = lvl->vRaySolid(colBox.y+colBox.h, colBox.y+colBox.h+spdY+1, colBox.x+colBox.w/2);
		} else{
			colDisplace = lvl->vRaySolid(colBox.y, colBox.y+spdY-1, colBox.x+colBox.w/2);
		}
		
		if (colDisplace != -1){
			
			spdY = 0;
			if (spdY >= 0){
				onGround = true;
				y = colDisplace*32;
			} else {
				y = colDisplace*32+33;
			}
			colBox.y = y+9;
				
		} else {
			spdY += 0.5;
			movingAnim->setSpeed(0);
			movingAnim->setCurrentFrame(1);
			onGround = false;
		}
			
		timer -= 0.1;
		
		if (timer <= 0){
			srand(time(NULL)+(long int)(this));
			timer = 2+rand()%4;
			
			if (rand()%5 == 0 && onGround){
				standing = !standing;
			}
			
			if (onGround && !standing){
				spdY = -12;
				lvl->playSound("jumpSound");
				onGround = false;
			}
				
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
		/*
		if (((colBox.y+colBox.h >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY;
		}
		*/
		
		enemy::step(lvl);
	
		colBox.x = x+12;
		colBox.y = y+9;
		
	}
}

void imp::draw(painter* pintor){
	enemy::draw(pintor);
}
