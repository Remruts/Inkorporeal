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
	invAngle = 0;
	
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
	
	double radAngle = (angle/180.0)*(3.1415);
	double X = cos(radAngle);
	double Y = -sin(radAngle);
	invAngle = 180*(atan2(Y, X)/3.1415);
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
			
		spdX = friction*spdX;
		spdY = friction*spdY;
		
		if (gravity){
			spdX += 0.2*cos(gravity_dir*3.1415/180);
			spdY += 0.2*sin(gravity_dir*3.1415/180);
		}
		
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
		
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			life = 0;
		}
		if (((y+32 >= 448) && (spdY > 0)) || ((y <= 0) && (spdY < 0))){
			life = 0;
		}
	}
}

void particle::draw(painter* picasso, LTexture* background){
	spritesheet->setBlendMode(blend);
	spritesheet->setAlpha(alpha);
	spritesheet->setColor(r, g, b);	

	if (life > 1){
	
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), 
			((sprite*spriteSize)/spritesheet->getWidth())*spriteSize, 
			spriteSize, spriteSize, int(x), int(y), spriteSize*scaleX, spriteSize*scaleY, angle, 0);
			
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), 
			((sprite*spriteSize)/spritesheet->getWidth())*spriteSize, 
			spriteSize, spriteSize, int(x), 448+(320-int(y)*0.7143-spriteSize*scaleY*0.7143), 
			spriteSize*scaleX, spriteSize*scaleY*0.7143, invAngle, 2); //espejado
	}	
	
	if ((life == 1) && permanent){
		picasso->setRenderTarget(background);
		picasso->drawEx(spritesheet, (sprite*spriteSize)%spritesheet->getWidth(), (sprite*spriteSize)/spritesheet->getWidth(), 
			spriteSize, spriteSize, int(x)-192, int(y), spriteSize*scaleX, spriteSize*scaleY, angle, 0);
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
	rate = 5+rand()%2;
}

coinSparkle::~coinSparkle(){
	
}
	
void coinSparkle::emit(){
	srand(time(NULL)*(x+y));
	
	particle* part = NULL;
	double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
		speed = (rand()%11)/10+1;
		spdX = cos((i/double(rate))*2*3.1415)*speed;
		spdY = sin((i/double(rate))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, rand()%3+12);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(16);
		part->setSprite(rand()%5+6);
		part->setGravity(true);
		part->setGravityDir(90);
		part->setScale(rand()%5/10.0f+0.5);
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

//DASH

dashEmitter::dashEmitter(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 16;
}

dashEmitter::~dashEmitter(){
	
}

void dashEmitter::emit(){

	particle* part = NULL;
	int Y, LIFE;
	for (int i = 0; i < rate; i++){
		Y = y+rand()%15-7;
		LIFE = double(1-(double(abs(Y-y))/14.0))*(rand()%2+5);
	
		part = new particle(spritesheet, x+rand()%21-10, Y, LIFE);
		//part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(22);
		part->setGravity(false);
		//part->setGravityDir(270);
		//part->setAngle(180*atan2(spdY/speed, spdX/speed)/3.1415);
		part->setScaleY(0.2);
		part->setAlpha(30);
		part->setBlend(1);	
		part->setColor(255, 255, 255);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	
}

void dashEmitter::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			(*it)->setAlpha(((*it)->getLife()/double((*it)->getMaxLife()))*30);
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

//"Aftershot" effect
afterShot::afterShot(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 1;
}

afterShot::~afterShot(){
	
}

void afterShot::emit(){

	particle* part = NULL;
	for (int i = 0; i < rate; i++){
		part = new particle(spritesheet, x, y-8, 10);
		//part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(9);
		part->setGravity(false);
		//part->setGravityDir(270);
		//part->setAngle(180*atan2(spdY/speed, spdX/speed)/3.1415);
		part->setAlpha(200);
		part->setScale(0.5);
		part->setBlend(1);	
		part->setColor(255, 210, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	
}

void afterShot::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*200);
			(*it)->setColor(255*proportion, 210*proportion*proportion*proportion, 32*proportion);
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

//hurt effect
hurtEffect::hurtEffect(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 3+rand()%2;
}

hurtEffect::~hurtEffect(){
	
}

void hurtEffect::emit(){

	particle* part = NULL;
	double spdX, spdY, speed;
	for (int i = 0; i < rate; i++){
	
		speed = 5+rand()%2;
		spdX = cos((i/double(rate)+((rand()%21-10)/100.0))*2*3.1415)*speed;
		spdY = sin((i/double(rate)+((rand()%21-10)/100.0))*2*3.1415)*speed;
		
		part = new particle(spritesheet, x, y, 4+rand()%3);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(22);
		part->setGravity(false);
		//part->setGravityDir(270);
		part->setAngle(180*atan2(spdY/speed, spdX/speed)/3.1415);
		part->setScaleY(0.5);
		part->setAlpha(255);
		part->setBlend(1);	
		part->setColor(200, 100, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	
}

void hurtEffect::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*255);
			(*it)->setColor(200*proportion, 100*proportion*proportion, 32*proportion);
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

//stars
starEffect::starEffect(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 3+rand()%3;
}

starEffect::~starEffect(){
	
}

void starEffect::emit(){

	particle* part = NULL;
	double spdX, spdY, speed, angle;
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate)+(rand()%2-1)/100.0)*2*3.1415;
		speed = 2+rand()%5;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
		
		part = new particle(spritesheet, x+6+cos(angle)*16, y+sin(angle)*16, 10+rand()%5);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(13);
		part->setGravity(true);
		part->setGravityDir(90);
		//part->setAngle(180*atan2(spdY/speed, spdX/speed)/3.1415);
		part->setScale(0.5);
		part->setAlpha(255);
		part->setBlend(1);	
		part->setColor(200, 100, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
}

void starEffect::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*255);
			(*it)->setColor(200*proportion, 100*proportion*proportion, 32*proportion);
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

//plasmaEffect
plasmaEffect::plasmaEffect(LTexture* sprt, int X, int Y, int R, int G, int B) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 5+rand()%3;
	
	r = R;
	g = G;
	b = B;
}

plasmaEffect::~plasmaEffect(){
	
}

void plasmaEffect::emit(){

	particle* part = NULL;
	double spdX, spdY, speed, angle;
	double startAngle = (rand()%11)/10.0;
	
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate) + startAngle)*2*3.1415;
		speed = 1;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
		
		part = new particle(spritesheet, x-8, y-8, 30);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(9);
		part->setGravity(false);
		part->setScale(1);
		part->setAlpha(200);
		part->setBlend(1);	
		part->setColor(r, g, b);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate) + startAngle)*2*3.1415;
		speed = 1;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
	
		part = new particle(spritesheet, x, y, 30);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(9);
		part->setGravity(false);
		part->setScale(0.5);
		part->setAlpha(200);
		part->setBlend(1);	
		part->setColor(0xff, 0xff, 0xff);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	
}

void plasmaEffect::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*200);
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

//waveEffect
waveEffect::waveEffect(LTexture* sprt, int X, int Y, int R, int G, int B, double tam, int Life) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 5+rand()%3;
	
	r = R;
	g = G;
	b = B;
	
	size = tam;
	partLife = Life;
	if (partLife < 1){
		partLife = 1;
	}
	
	invert = false;
}

waveEffect::waveEffect(LTexture* sprt, int X, int Y, int R, int G, int B, double tam, int Life, bool Invert) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 5+rand()%3;
	
	r = R;
	g = G;
	b = B;
	
	size = tam;
	partLife = Life;
	if (partLife < 1){
		partLife = 1;
	}
	
	invert = Invert;
}

waveEffect::~waveEffect(){
	
}

void waveEffect::emit(){

	particle* part = NULL;
			
	part = new particle(spritesheet, x, y, partLife);
	if (invert){
		part->setSpeed(size*32/double(partLife), size*32/double(partLife));
	} else{
		part->setSpeed(-(size*28/double(partLife)), -(size*28/double(partLife)));
	}	
	part->setSpriteSize(64);
	part->setSprite(4);
	part->setGravity(false);
	if (invert){
		part->setScale(1);
	} else {
		part->setScale(0.1);
	}	
	part->setAlpha(255);
	part->setBlend(1);
	part->setColor(r, g, b);
	part->setFriction(1);
	part->setPermanence(false);
	particles.push_back(part);
	
}

void waveEffect::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*255);
			if (invert){
				(*it)->setScale(proportion*size);
			} else {
				(*it)->setScale((1.1-proportion)*size);
			}
			
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

//explosionEffect
explosionEffect::explosionEffect(LTexture* sprt, int X, int Y) : emitter(sprt, 1, X, Y){
	timer = 1;
	maxTimer = 1;
	rate = 8+rand()%3;
}

explosionEffect::~explosionEffect(){
	
}

void explosionEffect::step(level* lvl){
	vector<particle*>::iterator it = particles.begin();
	double proportion;
	while(it != particles.end()){
		if ((*it) != NULL && (*it)->isAlive()){
			proportion = ((*it)->getLife()/double((*it)->getMaxLife()));
			(*it)->setAlpha(proportion*255);
			(*it)->setColor(200*proportion, 100*proportion*proportion, 32*proportion);
			//(*it)->setScale(proportion*size);
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

void explosionEffect::emit(){
	particle* part = NULL;
	double spdX, spdY, speed, angle;
	double randomAngle = ((rand()%11)/10.0)*2*3.1415;
	
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate))*2*3.1415 + randomAngle;
		speed = 5;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
		
		part = new particle(spritesheet, x+cos(angle)*16+rand()%9-4, y+sin(angle)*16+rand()%9-4, 15);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(rand()%8);
		part->setGravity(false);
		part->setScale(1);
		part->setAlpha(200);
		part->setBlend(1);	
		part->setColor(200, 100, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate))*2*3.1415 + randomAngle/2;
		speed = 1;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
		
		part = new particle(spritesheet, x+cos(angle)*16+rand()%17-8, y+sin(angle)*16+rand()%17-8, 15);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(rand()%8);
		part->setGravity(false);
		part->setScale(1);
		part->setAlpha(200);
		part->setBlend(1);	
		part->setColor(200, 100, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}	
	for (int i = 0; i < rate; i++){
		angle = (i/double(rate))*2*3.1415;
		speed = 1;
		spdX = cos(angle)*speed;
		spdY = sin(angle)*speed;
		
		part = new particle(spritesheet, x+cos(angle)*4, y+sin(angle)*4, 15);
		part->setSpeed(spdX, spdY);
		part->setSpriteSize(32);
		part->setSprite(9);
		part->setGravity(false);
		part->setScale(1);
		part->setAlpha(200);
		part->setBlend(1);	
		part->setColor(200, 100, 32);
		part->setFriction(1);
		part->setPermanence(false);
		particles.push_back(part);
	}
	
}
