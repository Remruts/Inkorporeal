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
	
	spdX = 0;
	facingRight = false;
	onGround = true;
	
	colBox.x = x+6;
	colBox.y = y;
	colBox.w = 20;
	colBox.h = 32;
	
	lives = 100;
	maxLives = 100;
	
	state = stPrepare;
	swordSprite->setCurrentFrame(0);
	nextState = stQuake;
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
	
		if (lives == 0){
			lvl->addKey(x, y);
		}
		
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
				y = colDisplace*32;
				
				if (state == stJump)
					state = stRun;
				
				if ((state == stQuake) && !onGround){
					lvl->addEmitter(new upSmoke(lvl->getEffectSheet(), x+colBox.w/2, y+colBox.h));
				}
				
				onGround = true;
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
			if (state == stRun || state == stIdle)
				state = stJump;
		}
		
		lvl->getPlayerPos(playerX, playerY);
		playerX = playerX - x;
		playerY = playerY - y;
		normalized = sqrt(playerX*playerX + playerY*playerY);
		
		timer -= 1;
		//timeFactor = lives/(double(maxLives)/2);
		timeFactor = lives/double(maxLives);
		
		if (state == stRun || state == stJump){
		
			spdX += (2*facingRight-1)*0.2;
			
			if (timer <= 0){
				if (rand()%2==0){
					timer = 20;
					state = stPrepare;
					if (abs(playerX) < 150 && playerY < 0 && onGround){
						nextState = stUppercut;
					} else {
						nextState = stDash;
					}
					
				} else if (rand()%10==0){
					state = stIdle;
					timer = timeFactor*(60+rand()%30);
				} else{
					timer = timeFactor*(60+rand()%30);
					if (onGround){
						//lvl->addEmitter(new upSmoke(lvl->getEffectSheet(), x+colBox.w/2, y+colBox.h));
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
				timer = (60+rand()%120)*timeFactor;
				state = stRun;
			}
		} else if (state == stDash) {
			
			//if (onGround && abs(spdX) > 0)
			//	lvl->addEmitter(new upSmoke(lvl->getEffectSheet(), x+colBox.w/2, y+colBox.h));
			swordSprite->setCurrentFrame(2);
			
			if (spdX != 0){
				facingRight = spdX>0;
				if (int(timer)%10 == 0)
					lvl->addEmitter(new circleEmitter(lvl->getEffectSheet(), x+ (facingRight ? 0 : colBox.w ), y+colBox.h/2, facingRight));
				
				//if (onGround)
				//	lvl->addEmitter(new upSmoke(lvl->getEffectSheet(), x+colBox.w/2, y+colBox.h));
			}
			
			if (timer <= 0){
				if (abs(spdX) > 0){
					timer = 60*timeFactor;
					spdX = 0;
				} else {
					colBox.x = x+6;
					colBox.w = 20;
					
					if (lives <= maxLives/4 && onGround){
						state = stPrepare;
						nextState = stUppercut;
						timer = 30*timeFactor;
					} else{
						if (!onGround && (rand()%3 == 0 || colBox.y > 320)){
							nextState = stQuake;
							state = stPrepare;
							timer = 20;
						} else {
							timer = (30+rand()%30)*timeFactor;
							state = stIdle;
						}
					}
					maxSpeedX = 2;
				}
			}
		} else if (state == stUppercut){
			swordSprite->setSpeed(0.5);
			swordSprite->step();
			if (timer<= 0){
				if (rand()%100 >= timeFactor*100){
					nextState = stQuake;
					state = stPrepare;
					timer = 20;
				} else {
					timer = (30+rand()%30)*timeFactor;
					state = stIdle;
				}
			}
			
		} else if (state == stPrepare){
			if (nextState == stDash){
				
				swordSprite->setCurrentFrame(1);
				spdX = 0;
				spdY = 0;
				facingRight = playerX > 0;
				
				if (timer <= 0){
					lvl->addEmitter(new circleEmitter(lvl->getEffectSheet(), x+ (facingRight ? 0 : colBox.w ), y+colBox.h/2, facingRight));
					state = stDash;
					timer = abs(playerX)/20;
					spdX = (2*(playerX>0)-1)*20;
					spdY = 0;
					maxSpeedX = 20;
				}
			} else if (nextState == stUppercut){
				
				swordSprite->setCurrentFrame(1);
				spdY = 0;
				spdX = 0;
				facingRight = playerX > 0;
				
				if (timer <= 0){
					lvl->addEmitter(new upSmoke(lvl->getEffectSheet(), x+colBox.w/2, y+colBox.h));
					state = stUppercut;
					timer = abs(playerY)/20;
					if (timer < 10);
						timer = 30;
					spdY = -15;
				}
			} else if (nextState == stQuake){
				swordSprite->setCurrentFrame(0);
				spdY = 0;
				spdX = 0;
				
				if (timer <= 0){
					state = stQuake;
					timer = 80*timeFactor+10;
					spdY = 20;
				}
			}
			
		} else if (state == stQuake){
			swordSprite->setCurrentFrame(0);
			if (!onGround){
				spdY = 20;
			}
			
			if (timer <= 0 && onGround){
				timer = (60+rand()%30)*timeFactor;
				state = stIdle;
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
		
				
		enemy::step(lvl);
		
		if (state == stDash){
			if (facingRight){
				colBox.x = x+6;
			} else{
				colBox.x = x-22;
			}
			
			colBox.y = y+10;
			
			colBox.w = 48;
			colBox.h = 22;
			
		} else if (state == stUppercut){
			if (facingRight){
				colBox.x = x+6;
			} else{
				colBox.x = x-14;
			}
			
			colBox.w = 40;
			colBox.h = 32;
			
		} else {
			colBox.x = x+6;
			colBox.y = y;
			colBox.w = 20;
			colBox.h = 32;
		}	
		
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
		} else if ((state == stPrepare) || (state == stQuake)){
			
			if (swordSprite->getCurrentFrame() == 0){
				swordSprite->setFlip(!facingRight);
				pintor->drawEx(spritesheet, 128, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
				swordSprite->draw(pintor, (facingRight) ? x-10 : x-22, y+8-up);
			} else {
				swordSprite->setFlip(facingRight);
				pintor->drawEx(spritesheet, 64, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
				swordSprite->draw(pintor, (facingRight) ? x-25 : x-7, y+4-up);
			}
		} else if (state == stUppercut){
			int cFrame = swordSprite->getCurrentFrame();
			swordSprite->setFlip(!facingRight);
			
			switch (cFrame){
			case 1:
				pintor->drawEx(spritesheet, 64, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
				swordSprite->draw(pintor, (!facingRight) ? x-32 : x, y+4-up);
			break;
			case 2:
				pintor->drawEx(spritesheet, 96, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
				swordSprite->draw(pintor, (facingRight) ? x+10 : x-41, y-11-up);
			break;
			case 3:
				pintor->drawEx(spritesheet, 96, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
				swordSprite->draw(pintor, (facingRight) ? x+4 : x-36, y-19-up);
			break;
			default:
				pintor->drawEx(spritesheet, 64, 32, 32, 32, x, y+7-up, 32, 32, 0, !facingRight);
			break;
			}	
			
		} else {
			runSpriteTop->draw(pintor, x, y+7-up);
		}
		
		spritesheet->setAlpha(255);
		
		//barra
		pintor->setColor(0x17, 0x17, 0x17, 255);
		pintor->drawRect(260, 64, 820, 16, 1);
		pintor->setColor(180, 0, 0, 255);
		pintor->drawRect(260, 64, 820*(lives/double(maxLives)), 16, 1);
		pintor->setColor(180, 180, 180, 255);
		pintor->drawRect(260, 64, 820, 16, 0);
		pintor->drawRect(259, 63, 822, 18, 0);
		pintor->setColor(0x17, 0x17, 0x17, 255);
	}
	//enemy::draw(pintor);
	
	//debug
	//pintor->setColor(255, 0, 0, 255);
	//pintor->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
	//pintor->setColor(0x7F, 0x7F, 0x7F, 255);
}


// "upSmoke" emitter
upSmoke::upSmoke(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 10+rand()%2;
}

upSmoke::~upSmoke(){
	
}
	
void upSmoke::emit(){
	srand(time(NULL)*(x+y));
	
	particle* part = NULL;
	double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
		speed = (rand()%31-15)/10;
		spdX = cos((i/double(rate))*2*3.1415)*speed*1.1;
		spdY = sin((i/double(rate))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, rand()%3+12);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(64);
		part->setSprite(3);
		part->setGravity(true);
		part->setGravityDir(270);
		part->setScale(0.3);
		part->setAlpha(255);
		part->setBlend(0);
		//part->setAngle(rand()%360);
		//part->setColor(255, 255, 255);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
}

void upSmoke::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double lifeScale;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			lifeScale = (*it)->getLife()/double((*it)->getMaxLife());
			(*it)->setAlpha(lifeScale*255);
			(*it)->setScale(lifeScale*0.3);
			it++;
		} else {
			if (*it != NULL)
				delete *it;
			*it = NULL;
			
			it = particles.erase(it);
		}
	}
	emitter::step(lvl);
}
//end "upSmoke" emitter


//circleEmitter
circleEmitter::circleEmitter(LTexture* sprt, int X, int Y, bool IZQ) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 5;
	izq = IZQ;
}

circleEmitter::~circleEmitter(){
	
}
	
void circleEmitter::emit(){
	srand(time(NULL)*(x+y));
	
	particle* part = NULL;
	//double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
		//speed = (rand()%31-15)/10;
		//spdX = cos((i/double(rate))*2*3.1415)*speed*1.1;
		//spdY = sin((i/double(rate))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, 15);
		part->setSpeed((izq*(-2)+1), -2);
		part->setSpriteSize(64);
		part->setSprite(4);
		part->setGravity(false);
		//part->setGravityDir(270);
		part->setScaleX(0.1);
		part->setScaleY(0.1);
		part->setAlpha(255);
		part->setBlend(1);
		//part->setAngle(rand()%360);
		part->setColor(0, 100, 200);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
}

void circleEmitter::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double lifeScale;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			lifeScale = (*it)->getLife()/double((*it)->getMaxLife());
			(*it)->setAlpha(lifeScale*255);
			(*it)->setScaleY((1.1-lifeScale));
			it++;
		} else {
			if (*it != NULL)
				delete *it;
			*it = NULL;
			
			it = particles.erase(it);
		}
	}
	emitter::step(lvl);
}
//end circleEmitter

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
