#include "enemy.h"
#include <iostream>

enemy::enemy(LTexture* sprt, int X, int Y){
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
	currentAnim->step();
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
	
	if (hurt){
		hurtTimer-=0.2;
		if (hurtTimer<=0){
			hurt = false;
			hurtTimer = 2;
		}
	}
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
