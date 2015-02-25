#include "enemy.h"
#include "mask.h"
#include <iostream>

mask::mask(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {3};
	movingAnim = new animation(1, 0, false, spritesheet, frms, 32);
	currentAnim = movingAnim;
	
	accel = 1;
	maxSpeedX = 1;
	maxSpeedY = 1;
	visible = 2;
	alpha = 255;
	
	colBox.x = x+4;
	colBox.y = y;
	colBox.w = 24;
	colBox.h = 32;
	
	lives = 10;
	maxLives = lives;
	timer = 1;
}

mask::~mask(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void mask::step(level* lvl){
	if (alive){
	
		timer -= 1;
		if (timer == 0){
			lvl->playSound("warningSound");
			timer = 120+rand()%120;
		}
		
		lvl->getPlayerPos(playerX, playerY);
		playerX = playerX - x;
		playerY = playerY - y;
		normalized = sqrt(playerX*playerX + playerY*playerY);
		
		spdX += double(playerX)/normalized;
		spdY += double(playerY)/normalized;
	
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			spdX = -spdX*2;
		}
		if (((y+32 >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY*2;
		}
		
	}
	
	enemy::step(lvl);
	facingRight = true;
	
	colBox.x = x+4;
	colBox.y = y;
}

void mask::draw(painter* pintor){	
	spritesheet->setBlendMode(1);
	spritesheet->setAlpha(50);
	for (int i = 0; i<10; i++){
		pintor->drawEx(spritesheet, 96+32*rand()%2, 96, 32, 32, x-9+rand()%5, y-9+rand()%5, 48, 48, rand()%360, rand()%4);
		//Podemos tener algunas discrepancias entre versión espejo y real
		pintor->drawEx(spritesheet, 96+32*rand()%2, 96, 32, 32, x-9+rand()%5, 448+(320-y*0.7143)-48*0.7143+5, 
			48, 48*0.7143, rand()%360, rand()%4); 
	}
	spritesheet->setBlendMode(0);
	spritesheet->setAlpha(255);
	enemy::draw(pintor);
}
