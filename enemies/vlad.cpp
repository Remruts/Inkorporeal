#include "vlad.h"
#include "enemy.h"
#include <iostream>

vlad::vlad(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	
	unsigned int frms0[] = {0, 1};
	faceSprite = new animation(2, 0, false, spritesheet, frms0, 32);
	
	unsigned int frms1[] = {3, 2};
	runSpriteBot = new animation(2, 0, true, spritesheet, frms1, 32);
	
	unsigned int frms2[] = {7, 8, 7, 6};
	runSpriteTop = new animation(4, 0.08, true, spritesheet, frms2, 32);
	
	unsigned int frms3[] = {3, 4, 5, 6};
	swordSprite = new animation(4, 0.1, false, spritesheet, frms3, 64);
	
	
	timer = 30.0;
	timeFactor = 1;
	
	accel = 0.04;
	maxSpeedX = 2;
	maxSpeedY = 20;
	visible = 0;
	alpha = 255;
	
	spdX = -2;
	facingRight = false;
	onGround = true;
	
	colBox.x = x+6;
	colBox.y = y;
	colBox.w = 20;
	colBox.h = 32;
	
	lives = 50;
	maxLives = 100;
	
	state = stPrepare;
	currentAnim = NULL;
}

vlad::~vlad(){
	if (faceSprite != NULL){
		delete faceSprite;
		faceSprite = NULL;
	}
	
	if (runSpriteBot != NULL){
		delete runSpriteBot;
		runSpriteBot = NULL;
	}
	
	if (runSpriteTop != NULL){
		delete runSpriteTop;
		runSpriteTop = NULL;
	}
	
	if (swordSprite != NULL){
		delete swordSprite;
		swordSprite = NULL;
	}
}

void vlad::step(level* lvl){
	
	if (alive){
		
		if (spdX>=0){
			runSpriteTop->setSpeed(0.08);
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
			if (state != stDash)
				spdY += 0.5;
			runSpriteBot->setSpeed(0);
			runSpriteBot->setCurrentFrame(1);
			onGround = false;
		}
		
		lvl->getPlayerPos(playerX, playerY);
		playerX = playerX - x;
		playerY = playerY - y;
		normalized = sqrt(playerX*playerX + playerY*playerY);
			
		timer -= 1;
		timeFactor = lives/(double(maxLives)/2);
		
		if (state == stRun){
		
			spdX += (2*facingRight-1)*0.2;
			
			if (timer <= 0){
				if (rand()%2==0){
					state = stPrepare;
					timer = 20;
					
				} else if (rand()%10==0){
					state = stIdle;
					timer = 60*timeFactor+rand()%30;
				} else{
					timer = 60*timeFactor+rand()%30;
					if (onGround){
						spdY = -12;
						onGround = false;
					}
				}
			}
			
		} else if (state == stIdle){
			spdX *= (1-accel);
			
			if (abs(spdX) <= 1){
				spdX = 0;
			}
			
			if (timer <= 0){
				timer = 60+rand()%120;
				state = stRun;
			}
		} else if (state == stDash) {
			swordSprite->setCurrentFrame(2);
			
			if (spdX != 0)
				facingRight = spdX>0;
			
			if (timer <= 0){
				if (abs(spdX) > 0){
					timer = 30+rand()%30;
					spdX = 0;
				} else {
					timer = 30+rand()%30;
					state = stIdle;
					maxSpeedX = 2;
				}
			}
		} else if (state == stPrepare){
			swordSprite->setCurrentFrame(1);
			spdX = 0;
			spdY = 0;
			facingRight = playerX > 0;
			if (timer <= 0){
				state = stDash;
				timer = abs(playerX)/20;
				spdX = (2*(playerX>0)-1)*20;
				spdY = 0;
				maxSpeedX = 20;
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
		
		colBox.x = x+6;
		colBox.y = y;
		
		if (state == stRun){
			if (runSpriteTop->getSpeed() == 0){
				runSpriteTop->setSpeed(0.08);
				runSpriteTop->setCurrentFrame(0);
			}
			runSpriteTop->step();
			runSpriteBot->setCurrentFrame(runSpriteTop->getCurrentFrame()%2);
		} 
		if ((state == stJump) || (state == stDash)){
			runSpriteTop->setSpeed(0);
			runSpriteTop->setCurrentFrame(1);
			runSpriteBot->setSpeed(0);
			runSpriteBot->setCurrentFrame(0);
		}
		if (state == stIdle){
			runSpriteTop->setSpeed(0);
			runSpriteBot->setSpeed(0);
			runSpriteTop->setCurrentFrame(3);
			runSpriteBot->setCurrentFrame(1);
		}
		
	}
}

void vlad::draw(painter* pintor){
	if (alive){
		spritesheet->setAlpha(alpha);
		
		runSpriteTop->setFlip(!facingRight);
		runSpriteBot->setFlip(!facingRight);
		faceSprite->setFlip(!facingRight);
		
		
		if (hurt){
			spritesheet->setColor(200, 0, 0);
		} else{
			spritesheet->setColor(255, 255, 255);
		}
		
		bool up = !runSpriteBot->getCurrentFrame();
		
		faceSprite->draw(pintor, x+2*(!facingRight)-1, y-7-up);
		
		runSpriteBot->draw(pintor, x + (facingRight ? 2 : -2), y+12-up + ((state == stDash) || (state == stPrepare)));
		
		if (state == stDash){
			swordSprite->setFlip(!facingRight);
			pintor->drawEx(spritesheet, 96, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
			swordSprite->draw(pintor, (facingRight) ? x+10 : x-41, y-11-up);
		} else if (state == stPrepare){
			swordSprite->setFlip(facingRight);
			pintor->drawEx(spritesheet, 64, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
			swordSprite->draw(pintor, (facingRight) ? x-25 : x-7, y+4-up);
		} else {
			runSpriteTop->draw(pintor, x, y+7-up);
		}
		
		
		spritesheet->setAlpha(255);
	}
	//enemy::draw(pintor);
}

//thrownbone
/*
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

*/
