#include "door.h"

door::door(LTexture* sprt, int X, int Y, bool Locked){
	
	x = X;
	y = Y;
	
	spritesheet = sprt;
	unsigned int frms[] = {0, 1, 2, 3, 4, 5, 5};
	doorAnim = new animation(5, 0, false, spritesheet, frms, 64);
	
	opened = false;
	colBox.x = x+16;
	colBox.y = y+32;
	colBox.w = 32;
	colBox.w = 32;
	
	locked = Locked;
	timer = 90;
}

door::~door(){
	if (doorAnim != NULL){
		delete doorAnim;
		doorAnim = NULL;
	}
}

void door::step(){
	if (timer>0){
		timer-=1;
	}
	
	doorAnim->step();
	if (!opened && doorAnim->isFinished()){
		opened = true;
	}
}

void door::draw(painter* vincent){
	
	if (timer>0){
		spritesheet->setAlpha(255*timer/90.0);
		vincent->draw(spritesheet, 256, 0, 64, 64, 272, 352);
		spritesheet->setAlpha(255);
	}
	
	doorAnim->draw(vincent, x, y); //992, 352
	if (locked){
		vincent->draw(spritesheet, 64, 64, 32, 32, x+16, y+24);
	}
}

bool door::isOpened(){
	return opened;
}

SDL_Rect* door::getColBox(){
	return &colBox;
}

void door::open(){
	if (!locked)
		doorAnim->setSpeed(0.1);
}

void door::unlock(){
	locked = false;
}


key::key(LTexture* sprt, int X, int Y){
	x = X;
	y = Y;
	
	spritesheet = sprt;
	
	colBox.x = x;
	colBox.y = y+16;
	colBox.w = 32;
	colBox.h = 32;
	
	pickedup = false;
	used = false;
	counter = 0;
	
}

key::~key(){
	
}

void key::step(level* lvl){
	counter+=0.1;
	if (counter >= 360)
		counter = 0;
	
	colBox.x = x+16;
	colBox.y = y+16;
}

void key::draw(painter* picasso){
	if (!used){
		picasso->draw(spritesheet, 0, 64, 64, 64, x, y-5+5*sin(counter/3.1415*2));
	}
	//picasso->setColor(255, 255, 255, 255);
	//picasso->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
}

SDL_Rect* key::getColBox(){
	return &colBox;
}

void key::pick(){
	pickedup = true;
}

bool key::wasPicked(){
	return pickedup;
}

void key::use(){
	used = true;
}

bool key::wasUsed(){
	return used;
}

void key::setPos(int X, int Y){
	x = X;
	y = Y;
}
