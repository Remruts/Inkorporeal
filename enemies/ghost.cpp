#include "ghost.h"
#include "enemy.h"
#include <iostream>

ghost::ghost(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {0, 1, 2};
	movingAnim = new animation(3, 0.2, true, spritesheet, frms, 32);
	currentAnim = movingAnim;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 2;
	maxSpeedY = 2;
	visible = 1;
	alpha = 170;
	
	colBox.x = x+4;
	colBox.y = y;
	colBox.w = 24;
	colBox.h = 32;
	
	lives = 2;
	maxLives = lives;
}

ghost::~ghost(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void ghost::step(level* lvl){
	if (alive){
		
		timer -= 0.1;
		
		if (timer <= 0){
			srand(time(NULL)+(long int)(this));
			spdX += (rand()%10-4.5)/2.0;
			srand(time(NULL)+(long int)(this));
			spdY += (rand()%10-4.5)/2.0;
			timer = rand()%2+1;
		}
	
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			spdX = -spdX*2;
		}
		if (((y+32 >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY*2;
		}
		
	}
	
	enemy::step(lvl);
	
	colBox.x = x+4;
	colBox.y = y;
}

void ghost::draw(painter* pintor){
	enemy::draw(pintor);
}
