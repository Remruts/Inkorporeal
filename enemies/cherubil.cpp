#include "enemy.h"
#include "cherubil.h"
#include <iostream>

cherubil::cherubil(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {30, 31, 32, 31, 30};
	cherubAnim = new animation(5, 0.25, true, spritesheet, frms, 32);
	currentAnim = cherubAnim;
	unsigned int frms2[] = {35, 36, 37, 36, 35};
	devilAnim = new animation(5, 0.25, true, spritesheet, frms2, 32);
	if (devilAnim != NULL)
		devilAnim->setStretch(1);
	
	accel = 1;
	maxSpeedX = 5;
	maxSpeedY = 5;
	visible = 0;
	alpha = 255;
	
	colBox.x = x+6;
	colBox.y = y;
	colBox.w = 20;
	colBox.h = 32;
	
	lives = 2;
	maxLives = lives+2;
	
	timer = 10;
	spdX = 0;
	spdY = 0;
}

cherubil::~cherubil(){
	if (cherubAnim != NULL){
		delete cherubAnim;
		cherubAnim = NULL;
	}
	if (devilAnim != NULL){
		delete devilAnim;
		devilAnim = NULL;
	}
}

void cherubil::step(level* lvl){
	if (alive){
		
		timer -= 1;
		if (timer <= 0){
			timer = 100;
		}
		spdY = sin(2*(3.1415)*timer/99)*3;
		
		lvl->getPlayerPos(playerX, playerY);
		playerX = playerX - x;
		playerY = playerY - y;
		normalized = sqrt(playerX*playerX + playerY*playerY);
		
		if (normalized<300){
			facingRight = (playerX >= 0);
			
			if (int(timer)%50 == 0){
				lvl->playSound("shootSound2");
				lvl->addEnemyBullet(new cherubilArrow(spritesheet, x, y, 
					6*(double(playerX)/normalized), 8*(double(playerY)/normalized), rand()%2));
			}
		}
		
	
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			spdX = -spdX*2;
		}
		if (((y+32 >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY*2;
		}
		
	}
	
	enemy::step(lvl);
	devilAnim->step();
	
	colBox.x = x+6;
	colBox.y = y;
}

void cherubil::draw(painter* pintor){	

	enemy::draw(pintor);
	if (devilAnim != NULL){
		if (hurt){
			spritesheet->setColor(200, 0, 0);
		} else{
			spritesheet->setColor(255, 255, 255);
		}
	
		devilAnim->setFlip(!facingRight);
		spritesheet->setAlpha(alpha);
	
		devilAnim->draw(pintor, x, y);
		spritesheet->setAlpha(255);
		spritesheet->setColor(255, 255, 255);
	}
	
}



cherubilArrow::cherubilArrow(LTexture* sprt, int X, int Y, double SPDX, double SPDY, bool TYPE) : enemyBullet(sprt, X, Y, SPDX, SPDY){
	
	visible = TYPE;
	
	unsigned int frms[1];
	if (visible)
		frms[0] = 38;
	else
		frms[0] = 33;
	shootAnim = new animation(1, 0.3, true, spritesheet, frms, 32);
	currentAnim = shootAnim;
	
	
	invAngle = atan2(-SPDY, SPDX)/3.14159265359*180.0;
	angle = atan2(SPDY, SPDX)/3.14159265359*180.0; //(SPDX<0)*180;
	
	life = 180;
	
	colBox.x = x+8;
	colBox.y = y+8;
	colBox.w = 16;
	colBox.h = 16;
	alpha = 255;
	
}

cherubilArrow::~cherubilArrow(){
	
	if (shootAnim != NULL){
		delete shootAnim;
		shootAnim = NULL;
	}
}

void cherubilArrow::step(level* lvl){
		
	enemyBullet::step(lvl);
	
	colBox.x = x+8;
	colBox.y = y+8;
}

void cherubilArrow::draw(painter* picasso){	
	spritesheet->setAlpha(20);
	if (visible){
		currentAnim->setStretch(!visible);
		currentAnim->setAngle(angle);
		currentAnim->draw(picasso, x, y);
		currentAnim->setAngle(invAngle);
	} else {
		currentAnim->setStretch(1);
		currentAnim->setAngle(invAngle);
		currentAnim->draw(picasso, x, y);
		currentAnim->setAngle(angle);
	}
	spritesheet->setAlpha(alpha);
	currentAnim->setStretch(visible);
	currentAnim->draw(picasso, x, y);
	currentAnim->setStretch(0);
	spritesheet->setAlpha(255);
}

