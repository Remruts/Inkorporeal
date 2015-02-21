#include "enemy.h"
#include "jack.h"
#include <iostream>

jack::jack(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
	unsigned int frms[] = {20, 21, 22};
	movingAnim = new animation(3, 0.2, true, spritesheet, frms, 32);
	currentAnim = movingAnim;
	
	accel = 1;
	maxSpeedX = 1;
	maxSpeedY = 1;
	visible = 1;
	alpha = 255;
	
	colBox.x = x+4;
	colBox.y = y;
	colBox.w = 24;
	colBox.h = 32;
	
	lives = 6;
	maxLives = lives;
	
	state = stRandom;
	timer = 10;
}

jack::~jack(){
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
}

void jack::step(level* lvl){
	if (alive){
		
		timer -= 1;
		
		lvl->getPlayerPos(playerX, playerY);
		playerX = playerX - x;
		playerY = playerY - y;
		normalized = sqrt(playerX*playerX + playerY*playerY);
		
		if (state != stEscape){
			if (normalized <= 200){
				state = stPursue;
			} else if (state != stShoot) {
				state = stRandom;
			}
		}
		if (hurt && hurtTimer>=2){
			state = stEscape;
			spdY = -2;
			timer = 120;
			lvl->addEnemyBullet(new jackFire(spritesheet, x, y, 4*(double(playerX)/normalized), 4*(double(playerY)/normalized)));
		}
		
		switch (state){
		case stPursue:
			spdX += double(playerX)/normalized;
			spdY += double(playerY)/normalized;
		break;
		
		case stRandom:
			if (timer <= 0){
				spdX = (2*(rand()%2)-1);
				spdY = (2*(rand()%2)-1);
				if (rand()%2 == 0)
					state = stShoot;
				timer = 100+rand()%21;
			}
		break;
		
		case stShoot:
			lvl->addEnemyBullet(new jackFire(spritesheet, x, y, 4*(double(playerX)/normalized), 4*(double(playerY)/normalized)));
			state = stRandom;
		break;
		
		case stEscape:
			spdX -= double(playerX)/normalized;
			spdY -= double(playerY)/normalized;
			maxSpeedX = 2;
			maxSpeedY = 2;
			
			if (timer <= 0){
				state = stShoot;
				timer = 100+rand()%21;
				maxSpeedX = 1;
				maxSpeedY = 1;
			}
		break;
		}
	
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			spdX = -spdX*2;
		}
		if (((y+32 >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY*2;
		}
		
	}
	
	enemy::step(lvl);
	
	facingRight = (playerX >= 0);
	
	colBox.x = x+4;
	colBox.y = y;
}

void jack::draw(painter* pintor){	

	enemy::draw(pintor);
	
	//dibujo la linterna
	pintor->draw(spritesheet, 96, 128, 32, 32, x+16*(facingRight*2-1), y+8);
	//Podemos tener algunas discrepancias entre versión espejo y real
	pintor->drawEx(spritesheet, 96, 128, 32, 32, x+16*(facingRight*2-1), 448+(320-y*0.7143)-32*0.7143-4, 
		32, 32*0.7143, 0, 2); 
}




jackFire::jackFire(LTexture* sprt, int X, int Y, double SPDX, double SPDY) : enemyBullet(sprt, X, Y, SPDX, SPDY){
	
	unsigned int frms[] = {24, 25, 26, 27, 28};
	fireAnim = new animation(5, 0.3, true, spritesheet, frms, 32);
	currentAnim = fireAnim;
	
	visible = 1;
	angle = (SPDX<0)*180;
	
	life = 180;
	
	colBox.x = x+8;
	colBox.y = y+8;
	colBox.w = 16;
	colBox.h = 16;
	alpha = 200;
	
}

jackFire::~jackFire(){
	
	if (fireAnim != NULL){
		delete fireAnim;
		fireAnim = NULL;
	}
}

void jackFire::step(level* lvl){
	
	if (life == 0){
		lvl->addEmitter(new fireEmitter(spritesheet, x, y));
	}
			
	enemyBullet::step(lvl);
	
	colBox.x = x+8;
	colBox.y = y+8;
}

void jackFire::draw(painter* picasso){	
	spritesheet->setBlendMode(1);
	enemyBullet::draw(picasso);
	spritesheet->setBlendMode(0);
}


fireEmitter::fireEmitter(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 8;
}

fireEmitter::~fireEmitter(){
	
}

void fireEmitter::emit(){

	particle* part = NULL;
	double spdX, spdY, speed;
	if (life >= life/5){
		for (int i = 0; i < rate; i++){
			speed = (rand()%21-20)/10;
			spdX = cos((i/double(rate))*2*3.1415)*speed;
			spdY = sin((i/double(rate))*2*3.1415)*speed;
		
			part = new particle(spritesheet, x, y, rand()%3+12);
			part->setSpeed(spdX, spdY);
			part->setSpriteSize(32);
			part->setSprite(4);
			part->setGravity(true);
			part->setGravityDir(270);
			part->setScale(0.8);
			part->setAlpha(255);
			part->setBlend(1);	
			part->setColor(30, 30, 30);
			part->setFriction(1);
			part->setPermanence(false);
			particles.push_back(part);
		}
	}
}

void fireEmitter::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->setAlpha(((*it)->getLife()/double((*it)->getMaxLife()))*255);
			//(*it)->setColor(200, ((*it)->getLife()/double((*it)->getMaxLife()))*220, 0);
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
