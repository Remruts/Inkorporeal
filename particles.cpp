#include "particles.h"
#include <iostream>

particle::particle(LTexture* sprt, int X, int Y, int Life){
	spritesheet = sprt;
	
	x = X;
	y = Y;
	
	life = Life;
	maxLife = life;
	alive = true;
	
	bounce = false;
	gravity = false;
	gravity_dir = 90;
	
	spdX = 0;
	spdY = 0;
	
	scaleX = 1;
	scaleY = 1;
	
	alpha = 255;
	angle = 0;
	
	permanent = false;
	blend = 0;
	
	spriteSize = 32;
	
	r = 255;
	g = 255;
	b = 255;
	
}
particle::~particle(){
	
}

void particle::setSpriteSize(int s){
	if (s > 0)
		spriteSize = s;
}

void particle::setFriction(double f){
	friction = f;
}

void particle::setBounce(bool b){
	bounce = b;
}
	
void particle::setSprite(int sprNum){
	sprite = sprNum;
}

void particle::setScale(double s){
	scaleX = s;
	scaleY = s;
}

void particle::setScaleX(double s){
	scaleX = s;
}

void particle::setScaleY(double s){
	scaleY = s;
}
	
void particle::setSpeed(double SPDX, double SPDY){
	spdX = SPDX;
	spdY = SPDY;
}
double particle::getSpeedX(){
	return spdX;
}
double particle::getSpeedY(){
	return spdY;
}
	
bool particle::isAlive(){
	return alive;
}
int particle::getLife(){
	return life;
}
int particle::getMaxLife(){
	return maxLife;
}
	
void particle::setAlpha(int a){
	if (a<0)
		a = 0;
		
	if (a>255)
		a = 255;
		
	alpha = a;
}
void particle::setAngle(int dir){
	dir %= 360;
	if (dir<0){
		dir += 360;
	}
	angle = dir;
}
	
void particle::setPermanence(bool p){
	permanent = p;
}
	
void particle::setGravity(bool g){
	gravity = g;
}
void particle::setGravityDir(int dir){
	dir %= 360;
	if (dir<0){
		dir += 360;
	}
		
	gravity_dir = dir;
}
	
//blend or add?
void particle::setBlend(bool b){
	blend = b;
} 

void particle::setColor(unsigned int R, unsigned int G, unsigned int B){
	r = R;
	g = G;
	b = B;
}

double particle::getScaleX(){
	return scaleX;
}

double particle::getScaleY(){
	return scaleY;
}
	
void particle::step(level* lvl){
	
	if (alive){
		life -= 1;
		if (life <= 0){
			alive = false;
		}
		
		if (gravity){
			spdX += 0.2*cos(gravity_dir*3.1415/180);
			spdY += 0.2*sin(gravity_dir*3.1415/180);
		}
		
		spdX = friction*spdX;
		spdY = friction*spdY;
		
		if (bounce){
			if (lvl->isSolid(x+spdX, y+spdY)){
				spdX = -spdX;
				spdY = -spdY;
			} 
			
			if (lvl->isSolid(x+spdX, y)){
				spdX = -spdX;
			} 
			
			if (lvl->isSolid(x, y+spdY)){
				spdY = -spdY;
			}
		}
		
		x += spdX;
		y += spdY;
	}
}

void particle::draw(painter* picasso, LTexture* background){
	spritesheet->setBlendMode(blend);
	spritesheet->setAlpha(alpha);
	spritesheet->setColor(r, g, b);	

	if (life > 1){
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), ((sprite*spriteSize)/spritesheet->getWidth())*spriteSize, 
			spriteSize, spriteSize, x, y, spriteSize*scaleX, spriteSize*scaleY, angle, 0);
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), ((sprite*spriteSize)/spritesheet->getWidth())*spriteSize, 
			spriteSize, spriteSize, x, 448+(320-y*0.7143), spriteSize*scaleX, spriteSize*scaleY*0.7143, angle, 2); //espejado
	}	
	
	if ((life == 1) && permanent){
		picasso->setRenderTarget(background);
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), (sprite*spriteSize)/spritesheet->getWidth(), 
			spriteSize, spriteSize, x-192, y, spriteSize*scaleX, spriteSize*scaleY, angle, 0);
		picasso->resetRenderTarget();
	}
	
	spritesheet->setColor(255, 255, 255);
	spritesheet->setAlpha(255);
	spritesheet->setBlendMode(0);
}


emitter::emitter(LTexture* sprt, int Life, int X, int Y){
	alive = true;
	spritesheet = sprt;
	life = Life;
	x = X;
	y = Y;
	timer = 10;
	maxTimer = timer;
}

emitter::~emitter(){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if (*it != NULL)
			delete *it;
		*it = NULL;
		it = particles.erase(it);
	}
}
	
void emitter::emit(){
	particle* part = NULL;
	for (int i = 0; i < rate; i++){
		part = new particle(spritesheet, x, y, 5);
		particles.push_back(part);
	}
}

void emitter::step(level* lvl){
	
	if ((life == 0) && (particles.size() == 0)){
		alive = false;
	}
	
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->step(lvl);
			it++;
		} else {
			if (*it != NULL)
				delete *it;
			*it = NULL;
			
			it = particles.erase(it);
		}
	}
	
	if (life>0){
		life -= 1;
		timer -=1;
		if (timer <= 0){
			emit();
			timer = maxTimer;
		}
	}
}

void emitter::draw(painter* pintor, LTexture* back){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->draw(pintor, back);
		}
		
		it++;
	}
}

bool emitter::isAlive(){
	return alive;
}

void emitter::setPos(int X, int Y){
	x = X;
	y = Y;
}



colourExplosion::colourExplosion(LTexture* sprt, int X, int Y, painter* p) : emitter(sprt, 1, X, Y){
	timer = 0;
	maxTimer = 1;
	rate = 10+rand()%2;
	
	p->getRandomColor(r, g, b); //Distribuido por número de oro
	
	//Randomnes es mejor?
	/*
	r = rand()%256;
	g = rand()%256;
	b = rand()%256;
	*/
	
}

colourExplosion::~colourExplosion(){
	
}
	
void colourExplosion::emit(){
	srand(time(NULL)*(x+y));
	
	particle* part = NULL;
	double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
		speed = (rand()%50-25)/5;
		spdX = cos((i/double(rate))*2*3.1415)*speed;
		spdY = sin((i/double(rate))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, rand()%2+10);
		part->setSpeed(spdX, spdY);
		part->setSprite(rand()%9);
		part->setScale(rand()%15/10.0f+0.5);
		part->setAlpha(90);
		part->setBlend(1);
		part->setAngle(rand()%360);
		part->setColor(r, g, b);
		//part->setColor(rand()%256, rand()%256, rand()%256);
		part->setFriction(0.98);
		part->setPermanence(true);
		particles.push_back(part);
	}
}

void colourExplosion::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->setScale((*it)->getScaleX()*1.02);
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

coinSparkle::coinSparkle(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 4+rand()%2;
}

coinSparkle::~coinSparkle(){
	
}
	
void coinSparkle::emit(){
	srand(time(NULL)*(x+y));
	
	particle* part = NULL;
	double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
		speed = (rand()%50-25)/10;
		spdX = cos((i/double(rate))*2*3.1415)*speed;
		spdY = sin((i/double(rate))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, rand()%3+12);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(16);
		part->setSprite(rand()%6+5);
		part->setGravity(true);
		part->setGravityDir(90);
		part->setScale(rand()%3/10.0f+0.1);
		part->setAlpha(255);
		part->setBlend(1);
		part->setAngle(rand()%360);
		part->setColor(255, 255, 255);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
}

void coinSparkle::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->setAlpha(((*it)->getLife()/double((*it)->getMaxLife()))*255);
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
