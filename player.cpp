#include "player.h"

player::player(LTexture* sprt){
	lives = 3; //Tenemos 3 vidas
	points = 0; //y cero puntos...
	
	//posición inicial
	x = 288;
	y = 384;
	
	//Velocidad inicial
	spdX = 0;
	spdY = 0;
	//velocidad máxima
	maxSpeedX = 3;
	maxSpeedY = 9;
	
	//está herido?
	hurt = false;
	hurtCounter = 15;
	
	//Está disparando?
	shooting = 0;
	
	//Puede hacer dash?
	canDash = 1;
	
	//Veo para la derecha
	facingRight = true;

	//En 60(algo) parpadea
	timeToBlink = 60.0f;
	gun = 0;
	gunCounter = 1.0f;
	
	spritesheet = sprt;
	
	unsigned int blink_frms[] = {0, 1, 2, 2, 1};
	blinkSprite = new animation(5, 0, false, spritesheet, blink_frms, 32); //0.25 spd
	unsigned int runt_frms[] = {6, 7, 6, 5};
	runSpriteTop = new animation(4, 0.08, true, spritesheet, runt_frms, 32);
	unsigned int runb_frms[] = {11, 10};
	runSpriteBot = new animation(2, 0, false, spritesheet, runb_frms, 32);
	
	collisionBox.x = x+6;
	collisionBox.y = y;
	collisionBox.w = 20;
	collisionBox.h = 32;
	
	state = stIdle;
}

player::~player(){
	if (runSpriteBot != NULL){
		delete runSpriteBot;
		runSpriteBot = NULL;
	}
			
	if (runSpriteTop != NULL){
		delete runSpriteTop;
		runSpriteTop = NULL; 
	}
		
	if (blinkSprite != NULL){
		delete blinkSprite;
		blinkSprite = NULL;
	}
}

void player::reset(){
	x = 288;
	y = 384;
	
	spdX = 0;
	spdY = 0;
	
	hurt = false;
	hurtCounter = 15;
	
	shooting = 0;
	canDash = 1;
	
	setFlip(1);

	timeToBlink = 60.0f;
	gunCounter = 1.0f;
	
	collisionBox.x = x+6;
	collisionBox.y = y;
	
	state = stIdle;
}

int player::getLives(){
	return lives;
}

void player::addPoints(int p){
	points += p;
	if (points>9999999999){
		points = 9999999999;
	}
}

void player::addLives(){
	if (lives<3)
		lives++;
}

long int player::getPoints(){
	return points;
}
	
void player::setPos(int X, int Y){
	x = X;
	y = Y;
}

void player::getPos(int &X, int &Y) const{
	X = x;
	Y = y;
}

void player::setSpeed(double X, double Y){
	spdX = X;
	spdY = Y;
}

void player::getSpeed(double &X, double &Y) const{
	X = spdX;
	Y = spdY;
}

void player::setFlip(bool flp){
	facingRight = flp;
	runSpriteTop->setFlip(!flp);
	runSpriteBot->setFlip(!flp);
	blinkSprite->setFlip(!flp);
}
	
player::stateEnum player::getState() const{
	return state;
}

void player::setState(stateEnum st){
	state = st;
}
	
void player::setColBox(int X, int Y, int W, int H){
	collisionBox.x = X;
	collisionBox.y = Y;
	collisionBox.w = W;
	collisionBox.h = H;
}

const SDL_Rect& player::getColBox() const{
	return collisionBox;
}

void player::step(level* lvl){
	//volvemos a los estados que tengamos que volver
	if (state == stRun){
		if (abs(spdX) == 0)
			state = stIdle;
	}
	
	collisionBox.x = x+6;
	collisionBox.y = y;
	
	if (spdX > 0){
		spdX-=0.3;
		
		if ((spdX > maxSpeedX) && (state != stDash)){
			spdX = maxSpeedX;
		}
		
		if (spdX <= 0.1){
			spdX = 0;
		}
		
		x += int(spdX);
	}
	if (spdX < 0){
		spdX+=0.3;
		
		if ((spdX < -maxSpeedX) && (state != stDash)){
			spdX = -maxSpeedX;
		}
		
		if (spdX >= -0.1){
			spdX = 0;
		}
		x -= int(abs(spdX));
	}
	
	
	if (state == stJump){
		if (spdY < maxSpeedY)
			spdY+=0.3;
	}
	
	if (spdY>0){
		y += int(spdY);
	} else if (spdY<0){
		if (spdY < -maxSpeedY)
			spdY = -maxSpeedY;
		y -= int(abs(spdY));
	}
	
	blinkSprite->step();
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
	if (state ==stIdle){
		runSpriteTop->setSpeed(0);
		runSpriteBot->setSpeed(0);
		runSpriteTop->setCurrentFrame(3);
		runSpriteBot->setCurrentFrame(1);
	}
	
	//SHOOTING
	gunCounter -= 0.2;
	if (gunCounter <= 0){
		gunCounter = 0;
	}
	if (shooting && (gunCounter == 0)){
		
		bullet* b = new bullet(spritesheet, x + spdX + facingRight*32, y+20+spdY, 24*facingRight-12);
		lvl->addBullet(b);
		gunCounter = 1;
	}
	
	//blinking
	if (blinkSprite->isFinished()){
		blinkSprite->setCurrentFrame(0);
		blinkSprite->setSpeed(0);
	}
	
	timeToBlink -= 0.2;
	if (hurt){
		hurtCounter -= 0.2;
		if (hurtCounter <= 0){
			hurtCounter = 15;
			hurt = false;
		}
	}
	
	if (canDash && state == stDash){
		canDash = 0;
		timeToBlink = 3.0;
	}
	
	if (timeToBlink <= 2 && state == stDash && !canDash){
		state = stJump;
	}
	

	if (timeToBlink <= 0){
		if (state == stIdle)
			blinkSprite->setSpeed(0.25);
		
		timeToBlink += 60;
		canDash = 1;
	}
	
	//MOAR SHOOTING
	if (shooting && (int(timeToBlink)%3 == 0)){
		if (spdX == 0)
			spdX = (-4)*facingRight+2;
	}
	
	shooting = false;
}

	
void player::draw(painter* pintor){
	
	if ((!hurt) || (int(timeToBlink)%4 == 0)){
		bool up = !runSpriteBot->getCurrentFrame();
		blinkSprite->draw(pintor, x+!facingRight, y-7-up+shooting);
		runSpriteBot->draw(pintor, x, y+12-up);
	
		bool shootTime = (int(timeToBlink)%2 == 0);
	
		if (shooting){
			pintor->drawEx(spritesheet, 128, 32, 32, 32, x, y+8-up, 32, 32, 0, !facingRight);
			if (facingRight){
				//dibujar arma
				pintor->drawEx(spritesheet, 64, 64, 32, 32, x+8+shootTime, y+6-up, 32, 32, 0, 0);
				//dibujar efecto
				if (shootTime)
					pintor->drawEx(spritesheet, 32, 160, 32, 32, x+25, y+5-up, 32, 32, 0, 0);
			}
			else{
				//dibujar arma
				pintor->drawEx(spritesheet, 64, 64, 32, 32, x-8-shootTime, y+6-up, 32, 32, 0, 1);
				//dibujar efecto
				if (shootTime)
					pintor->drawEx(spritesheet, 32, 160, 32, 32, x-24, y+5-up, 32, 32, 0, 1);
			}
		} else {
			runSpriteTop->draw(pintor, x, y+7-up);
		}
		
	}
	
	if (lives > 0){
		pintor->draw(spritesheet, 64, 160, 64, 64, 0, 62);
	}
	if (lives > 1){
		pintor->draw(spritesheet, 64, 160, 64, 64, 64, 62);
	}
	if (lives > 2){
		pintor->draw(spritesheet, 64, 160, 64, 64, 128, 62);
	}
}

void player::shoot(){
	shooting = true;
}

bool player::isShooting(){
	return shooting;
}

bool player::isDashing(){
	return !canDash;
}

void player::getHurt(){
	if (!hurt){
		hurt = true;
		lives -= 1;
	}
}


bullet::bullet(LTexture* sprt, int X, int Y, int SPD){
	
	spritesheet = sprt;
	
	x = X;
	y = Y;
	
	spd = SPD;
	alive = true;
	
	collisionBox.x = x-1;
	collisionBox.y = y-2;
	collisionBox.w = 5;
	collisionBox.h = 5;
	
}

bullet::~bullet(){
	
}

void bullet::step(level* lvl){
	if (alive){
		x += spd;
		collisionBox.x = x-1;
		collisionBox.y = y-2;
		
		if (lvl->hRaySolid(x, x+spd, y) != -1){
			alive = false;
		}
		
	}
}

void bullet::draw(painter* pintor){
	if (alive){
		pintor->draw(spritesheet, 0, 160, 32, 32, x-16, y-16);
		pintor->drawEx(spritesheet, 0, 160, 32, 32, x-16, 448+(320-y*0.7143)-11, 32, 23, 0, 2);
	}
}

bool bullet::isAlive(){
	return alive;
}

void bullet::die(){
	alive = false;
}

SDL_Rect* bullet::getColBox(){
	return &collisionBox;
}

