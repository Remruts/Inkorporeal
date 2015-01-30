#include "skelleton.h"
#include "enemy.h"
#include <iostream>

skelleton::skelleton(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {5, 6};
	movingAnim = new animation(2, 0.2, true, spritesheet, frms, 32);
	
	unsigned int frms2[] = {7, 8, 9, 7, 7};
	shootingAnim = new animation(4, 0.1, false, spritesheet, frms2, 32);
	
	currentAnim = movingAnim;
	
	shooting = 0;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 1.5;
	maxSpeedY = 12;
	visible = 2;
	alpha = 255;
	
	spdX = 1.5;
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
				movingAnim->setSpeed(0.2);
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
			timer = 10+rand()%10;
			
			/* jumping
			if (onGround){
				spdY = -12;
				onGround = false;
			}
			*/
			if (!shooting){
				shooting = true;
				shootingAnim->setCurrentFrame(0);
				currentAnim = shootingAnim;
				spdX = 0;
			}
			//thrownbone* bone = new thrownbone(spritesheet, x, y, facingRight);
			//lvl->addEnemyBullet(bone);
				
		} 
		
		if (currentAnim == shootingAnim){
			if (currentAnim->getCurrentFrame() == 2 && shooting){
				thrownbone* bone = new thrownbone(spritesheet, x, y, facingRight);
				lvl->addEnemyBullet(bone);
				shooting = false;
			}
			if (currentAnim->isFinished()){
				currentAnim = movingAnim;
				spdX = maxSpeedX*(2*facingRight-1);
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
		
		if (onGround){
			if ((facingRight && !lvl->isSolid(colBox.x+colBox.w+spdX+1, colBox.y+colBox.h+spdY+1)) || 
				(!facingRight && !lvl->isSolid(colBox.x+spdX-1, colBox.y+colBox.h+spdY+1))){
				facingRight = !facingRight;
				spdX *= -1;
			}
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

//thrownbone

thrownbone::thrownbone(LTexture* sprt, int X, int Y, bool right) : enemyBullet(sprt, X, Y, (2*right-1)*5, -9){
	unsigned int frms[] = {11};
	boneSprite = new animation(1, 0, false, spritesheet, frms, 32);
	currentAnim = boneSprite;
	accelY = 1;
	
	colBox.x = x+8;
	colBox.y = y+8;
	colBox.w = 16;
	colBox.h = 16;
	
	visible = 2;
	side = right;
}
thrownbone::~thrownbone(){
	if (boneSprite != NULL){
		delete boneSprite;
		boneSprite = NULL;
	}
}
	
void thrownbone::step(level* lvl){
	accelY = 0.5;
	prevX.push_back(x);
	prevY.push_back(y);
	
	if (prevX.size() > 10)
		prevX.pop_front();
	if (prevY.size() > 10)
		prevY.pop_front();
	
	if (side){
		angle+=5;
	} else {
		angle-=5;
	}
	
	enemyBullet::step(lvl);
	
	colBox.x = x+8;
	colBox.y= y+8;
}

void thrownbone::draw(painter* picasso){
	
	deque<int>::iterator it = prevX.begin();
	deque<int>::iterator it2 = prevY.begin();
	int i = 0;
	int size = prevX.size();
	
	spritesheet->setBlendMode(1);
	spritesheet->setColor(0, 0, 200);
	
	while(it != prevX.end()){
		
		currentAnim->setAngle(angle+i/double(angle));
		spritesheet->setAlpha(alpha*(i/double(size)));
		currentAnim->draw(picasso, *it, *it2);
		currentAnim->setStretch(1);
		currentAnim->draw(picasso, *it, *it2);
		currentAnim->setStretch(0);
		
		it++;
		it2++;
		i++;
	}
	spritesheet->setColor(255, 255, 255);
	spritesheet->setBlendMode(0);
	
	enemyBullet::draw(picasso);
}

