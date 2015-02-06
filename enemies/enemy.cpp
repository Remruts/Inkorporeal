#include "enemy.h"
#include <iostream>

enemy::enemy(LTexture* sprt, int X, int Y){
	currentAnim = NULL;
	
	x = X+192;
	y = Y;
	
	accel = 0.03;
	
	spritesheet = sprt;
	
	hurt = false;
	hurtTimer = 2;
	alive = true;
	
	spdX = 0;
	spdY = 0;
	
	maxSpeedX = 5;
	maxSpeedY = 5;
	
	colBox.x = x;
	colBox.y = y;
	colBox.w = 32;
	colBox.h = 32;
	
	lives = 3;
	maxLives = lives;
	visible = 0;
	
	facingRight = true;
	
	srand(time(NULL)*(x+y));
	alpha = 255;
	
}
enemy::~enemy(){
	
}
	
void enemy::step(level*){
	if (alive){
		colBox.x = x;
		colBox.y = y;
		
		if (spdX > 0){
			facingRight = true;
			//spdX -= accel;
		
			if ((spdX > maxSpeedX)){
				spdX = maxSpeedX;
			}
		
			if (spdX <= 0.1){
				spdX = 0;
			}
		
			x += int(spdX);
		}
		if (spdX < 0){
			facingRight = false;
			//spdX += accel;
		
			if ((spdX < -maxSpeedX)){
				spdX = -maxSpeedX;
			}
		
			if (spdX >= -0.1){
				spdX = 0;
			}
			x -= int(abs(spdX));
		}
	
		if (spdY>0){
			//spdY -= accel;
			if (spdY > maxSpeedY)
				spdY = maxSpeedY;
		
			if (spdY <= 0.1){
				spdY = 0;
			}
		
			y += int(spdY);
		} else if (spdY<0){
			//spdY += accel;
		
			if (spdY < -maxSpeedY)
				spdY = -maxSpeedY;
			
			if (spdY >= -0.1){
				spdY = 0;
			}
			y -= int(abs(spdY));
		}

		if (lives <= 0){
			alive = false;
		}
	}
	if (currentAnim != NULL)
		currentAnim->step();
		
	if (hurt){
		hurtTimer-=0.2;
		if (hurtTimer<=0){
			hurt = false;
			hurtTimer = 2;
		}
	}
}

void enemy::draw(painter* disney){
	if (alive){
		if (currentAnim != NULL){
			
			spritesheet->setAlpha(alpha);
			
			if (hurt){
				spritesheet->setColor(200, 0, 0);
			} else{
				spritesheet->setColor(255, 255, 255);
			}
			
			currentAnim->setFlip(!facingRight);
			
			if ((visible == 0) || (visible == 2)){
				currentAnim->draw(disney, x, y);
			}
			
			if (visible == 0){
				currentAnim->setStretch(1);
				spritesheet->setAlpha(10+hurt*90);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
				currentAnim->setStretch(0);
			}
			
			if (visible == 1){
				spritesheet->setAlpha(10+hurt*90);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
			}
			
			if ((visible == 1) || (visible == 2)){
				currentAnim->setStretch(1);
				currentAnim->draw(disney, x, y);
				currentAnim->setStretch(0);
			}
		}
	}
	
	spritesheet->setAlpha(255);
	spritesheet->setColor(255, 255, 255);
}

bool enemy::isAlive(){
	return alive;
}

void enemy::getPos(int &X, int &Y) const{	
	X = x;
	Y = y;
}

SDL_Rect* enemy::getColBox(){
	return &colBox;
}

void enemy::getHurt(){
	if (!hurt){
		lives -= 1;
		hurt = true;
	}
	
}

int enemy::getMaxLives(){
	return maxLives;
}

enemyBullet::enemyBullet(LTexture* sprt, int X, int Y, double SPDX, double SPDY){
	
	currentAnim = NULL;
	angle = 0;
	
	x = X;
	y = Y;
	
	spritesheet = sprt;
	
	alive = true;
	life = -1;
	
	spdX = SPDX;
	spdY = SPDY;
	
	accelX = 0;
	accelY = 0;
	
	colBox.x = x;
	colBox.y = y;
	colBox.w = 32;
	colBox.h = 32;
	
	visible = 0;
	
	srand(time(NULL)*(x+y));
	alpha = 255;
}

enemyBullet::~enemyBullet(){
	
}
	
void enemyBullet::step(level* lvl){
	
	
	if (life == 0){
		alive = false;
	}
	life -= 1;
	
	if (angle>360){
		angle = angle % 360;
	}
	if (angle < 0){
		angle = 360+angle%360;
	}

	if (alive){
		colBox.x = x;
		colBox.y = y;
		
		if (spdX > 0){
			
			spdX -= accelX;
		
			if (spdX <= 0.1){
				spdX = 0;
			}
		
			x += int(spdX);
			
		}
		if (spdX <= 0){
		
			spdX += accelX;
		
			if (spdX >= -0.1){
				spdX = 0;
			}
			
			x -= int(abs(spdX));
			
		}
		
		spdY += accelY;
		
		if (spdY> 15)
			spdY = 15;
		
		y += int(spdY);
		

	}
	if (currentAnim != NULL)
		currentAnim->step();
}

void enemyBullet::draw(painter* disney){
	if (alive){
		if (currentAnim != NULL){
		
			currentAnim->setAngle(angle);
			spritesheet->setAlpha(alpha);
			
			if ((visible == 0) || (visible == 2)){
				currentAnim->draw(disney, x, y);
			}
			
			if (visible == 0){
				currentAnim->setStretch(1);
				spritesheet->setAlpha(10);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
				currentAnim->setStretch(0);
			}
			
			if (visible == 1){
				spritesheet->setAlpha(10);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
			}
			
			if ((visible == 1) || (visible == 2)){
				currentAnim->setStretch(1);
				currentAnim->draw(disney, x, y);
				currentAnim->setStretch(0);
			}
		}
	}
}
	
bool enemyBullet::isAlive(){
	return alive;
}

void enemyBullet::getPos(int &X, int &Y) const{	
	X = x;
	Y = y;
}

SDL_Rect* enemyBullet::getColBox(){
	return &colBox;
}

void enemyBullet::die(){
	life = 0;
}
