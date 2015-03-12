#include "priest.h"
#include "enemy.h"
#include <iostream>

priest::priest(LTexture* sprt, int X, int Y) : enemy(sprt, X-16, Y){
	//unsigned int frms[] = {0, 1, 2};
	//movingAnim = new animation(1, 0.2, true, spritesheet, frms, 32);
	//currentAnim = movingAnim;
	currentAnim = NULL;
	
	timer = 2.0;
	accel = 0.01;
	maxSpeedX = 2;
	maxSpeedY = 2;
	visible = 1;
	alpha = 170;
	
	colBox.x = x+4;
	colBox.y = y+12;
	colBox.w = 24;
	colBox.h = 32;
	
	lives = 200;
	maxLives = lives;
	
	haloAngle = 0;
	haloColor = 0;
	
	shadowDevil = new demon(spritesheet, 656, y-32);
	
	nextState = NULL;
	state = new stIdle();
	state->enter(NULL, this);
	prevState = "stIdle";
}

priest::~priest(){
	/*
	if (movingAnim != NULL){
		delete movingAnim;
		movingAnim = NULL;
	}
	*/
	
	if (shadowDevil != NULL){
		delete shadowDevil;
		shadowDevil = NULL;
	}
	
	vector<lightPillarEffect*>::iterator it = pillars.begin();
	while (it != pillars.end()){
		if (*it != NULL){
			delete *it;
			*it = NULL;
			it = pillars.erase(it);
		}else{
			it++;	
		}
	}
}

void priest::setTimer(double t){
	timer = t;
	if (timer < 1){
		timer = 1;
	}
}

double priest::getTimer(){
	return timer;
}

void priest::setNextState(bossState* st){
	if ((st != NULL) && (nextState != NULL)){
		if (state != nextState){
			delete nextState;
		}
	}
	nextState = st;
}

void priest::setPrevState(const string& str){
	prevState = str;
}

string priest::getPrevState(){
	return prevState;
}

void priest::setDevilAnim(const string& str){
	shadowDevil->setAnimation(str);
}

void priest::addPillar(level* lvl, int X){
	pillars.push_back(new lightPillarEffect(lvl->getEffectSheet(), X));
}

LTexture* priest::getSpriteSheet(){
	return spritesheet;
}

void priest::step(level* lvl){
	if (alive){
	
		timer -= 1;

		if (timer <= 0){
			if (state != NULL){
				state->exit(lvl, this);
				delete state;
				state = NULL;
			}
			
			state = nextState;
			state->enter(lvl, this);
		}
		
		state->step(lvl, this);
		enemy::step(lvl);
	
		if (shadowDevil != NULL){
			shadowDevil->step(lvl);
		}
	
		colBox.x = x+4;
		colBox.y = y+12;
	
		haloAngle += 0.5;
		if (haloAngle >= 360){
			haloAngle -= 360;
		}
	
		if (lives/double(maxLives) <= 0.25){
			haloColor = 1;
		} else {
			haloColor = 0;
		}
		
		vector<lightPillarEffect*>::iterator it = pillars.begin();
		while (it != pillars.end()){			
			if (*it != NULL){
				if ((*it)->isAlive()){
					(*it)->step(lvl);
					it++;
				} else {
					delete *it;
					*it = NULL;
					it = pillars.erase(it);
				}				
			}else{
				it++;	
			}
		}

	}
	
}

void priest::draw(painter* pintor){
	
	//cool pero MUY ineficiente
	//for (int i = 0; i < 28; ++i)
	//	drawHalo(pintor, spritesheet, 224+i*33, 400, 270, 1, 3, 1);
	
	//pilares de luz
	vector<lightPillarEffect*>::iterator it = pillars.begin();
	while (it != pillars.end()){
		if (*it != NULL){
			(*it)->draw(pintor);
		}
		it++;		
	}
	
	//halo
	int dir;
	for (int i = 0; i<15; ++i){
		dir = i*24+haloAngle;
		if (dir>360)
			dir-=360;
		drawHalo(pintor, spritesheet, colBox.x+colBox.w/2+cos(dir/180.0*3.1415)*24, 
			colBox.y-colBox.h/2+sin(dir/180.0*3.1415)*24, dir, 8, 3, (i%2==0)+haloColor);
		drawHalo(pintor, spritesheet, colBox.x+colBox.w/2+cos((360-dir)/180.0*3.1415)*24, 
			colBox.y-colBox.h/2+sin((360-dir)/180.0*3.1415)*24, (360-dir), 8, 3, (i%2==0)+haloColor);
	}
	
	//enemy::draw(pintor);
	double flap = sin((haloAngle*10)/180.0*3.1415)*10;
	
	//alas
	spritesheet->setBlendMode(1);
	spritesheet->setAlpha(255);
	//derecha
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x+4, y-20, 64, 64, 315+flap, 0);
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x+8, y-4, 64, 64, 0+flap, 0);
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x+4, y+12, 64, 64, 45+flap, 0);
	//izq
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x-34, y+12, 64, 64, 315-flap, 1);
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x-38, y-4, 64, 64, 0-flap, 1);
	pintor->drawEx(spritesheet, 64, 0, 64, 64, x-34, y-20, 64, 64, 45-flap, 1);
	spritesheet->setBlendMode(0);
	spritesheet->setAlpha(255);
	//priest
	pintor->draw(spritesheet, 32, 0, 32, 48, x, y-flap/8);
	
	//demonio
	shadowDevil->draw(pintor);
	
	//debug
	//pintor->setColor(200, 0, 0, 255);
	//pintor->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
	
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

//size es 1, 2, 4, 8, 16, etc
//counter debería empezar en 6?
void priest::drawHalo(painter* p, LTexture* cube, int x, int y, double direction, int size, int counter, int colour){
	if (size < 64){
		if ((x >= 1152) || (x <= 192)){
			return;
		}
		if ((y >= 448) || (y <= 0)){
			return;
		}
		//en radianes
		double inRad = direction/180.0*3.14159265359;
		//ángulo perpendicular
		double perpDir = ((direction+90 < 360) ? direction+90 : (direction+90)-360);
		//tamaño a dibujar, etc.
		int realSize = 32/size;
		
		//dibujo
		switch(colour){
		case 0:
			cube->setColor(0xff, 0xd5, 0x0);
		break;
		case 1:
			cube->setColor(200, 35, 35);
		break;
		case 2:
			cube->setColor(0x6e, 0x26, 0x97);
		break;
		default:
			cube->setColor(0xff, 0xd5, 0x0);
		break;	
		}
		
		//cube->setBlendMode(1);
		//cube->setAlpha(128);
		//if (rand()%10) //podría usar esto para que aparezcan "agujeros". No quedan mal.
		p->drawEx(cube, 0, 0, 32, 32, x-realSize/2, y-realSize/2, realSize, realSize, perpDir, 0);
		//cube->setBlendMode(0);
		//cube->setAlpha(255);
		cube->setColor(255, 255, 255);
		
		//parte recursiva
		if (rand()%counter == 0){
			//Dos ramas
			
			//perpendicular en radianes
			double inRad2 = perpDir/180.0*3.14159265359;
			
			size *= 2;
			counter = 3;
			
			double realSize2 = 32/size+1;
			
			realSize = realSize/2+realSize2/2+1;
						
			drawHalo(p, cube, x+realSize*cos(inRad)+(realSize2*cos(inRad2))/2, 
				y+realSize*sin(inRad)+(realSize2*sin(inRad2))/2, direction, size, counter, colour);
			drawHalo(p, cube, x+realSize*cos(inRad)-(realSize2*cos(inRad2))/2, 
				y+realSize*sin(inRad)-(realSize2*sin(inRad2))/2, direction, size, counter, colour);
				
		} else {
			//una rama
			realSize +=2;
			counter -= 1;
			drawHalo(p, cube, x+realSize*cos(inRad), y+realSize*sin(inRad), direction, size, counter, colour);
		}
	}
}

//---------------------------------------------------------------------------------
//BALAS
//sineBullet
sineBullet::sineBullet(LTexture* sprt, int X, int Y, bool side) : enemyBullet(sprt, X, Y, 2*(side*2-1), 0){
	
	currentAnim = NULL;
	
	visible = 1;
	angle = 0;
	
	life = 600;
	
	colBox.x = x;
	colBox.y = y;
	colBox.w = 32;
	colBox.h = 32;
	alpha = 200;
	
	timer = 0;
	
}

sineBullet::~sineBullet(){
	
}

void sineBullet::step(level* lvl){
	
	timer-=10;
	if (timer<0)
		timer = 360+int(timer)%360;
	
	if (life == 0){
		lvl->addEmitter(new plasmaEffect(spritesheet, x, y, 0, 200, 0));
	}
	
	y += 3*sin(timer/180.0*3.1415); //+0.5
	
	enemyBullet::step(lvl);
		
	colBox.x = x;
	colBox.y = y;
}

void sineBullet::draw(painter* picasso){	
	int scale = (rand()%41)/10;
	spritesheet->setBlendMode(1);
		
	spritesheet->setAlpha(5);
	spritesheet->setColor(0, 200, 0);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-16-scale/2, y-48-scale/2, 64+scale, 64+scale, 0, 2);
	spritesheet->setColor(0xff, 0xff, 0xff);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-scale/2, y-32-scale/2, 32+scale, 32+scale, 0, 2);
	
	spritesheet->setAlpha(255);
	spritesheet->setColor(0, 200, 0);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-16-scale/2, 448+(320-y*0.7143)-(48-scale/2)*0.7143, 64+scale, (64+scale)*0.7143, 0, 2);
	spritesheet->setColor(0xff, 0xff, 0xff);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-scale/2, 448+(320-y*0.7143)-(32-scale/2)*0.7143, 32+scale, (32+scale)*0.7143, 0, 2);
	
	spritesheet->setColor(0xff, 0xff, 0xff);
	spritesheet->setBlendMode(0);
}

//regularBullet
regularBullet::regularBullet(LTexture* sprt, int X, int Y, double spdX, double spdY) : enemyBullet(sprt, X, Y, spdX, spdY){
	
	currentAnim = NULL;
	
	visible = 1;
	angle = 0;
	
	life = 600;
	
	colBox.x = x-16;
	colBox.y = y-16;
	colBox.w = 32;
	colBox.h = 32;
	alpha = 200;
	
	r = 255;
	g = 255;
	b = 255;
	
	size = 1;
	
}

regularBullet::~regularBullet(){
	
}

void regularBullet::setColor(int R, int G, int B){
	r = (unsigned int)(R);
	g = (unsigned int)(G);
	b = (unsigned int)(B);
}

void regularBullet::setSize(double s){
	if (s>0){
		size = s;
	}
	colBox.x = x;
	colBox.y = y-16*size;
	colBox.w = 32*size;
	colBox.h = 32*size;
}

void regularBullet::setVisible(int v){
	if (v >= 0 && v < 3)
		visible = v;
}

void regularBullet::setLife(int l){
	if (l > 0)
		life = l;
}

void regularBullet::step(level* lvl){
	
	if (life == 0){
		lvl->addEmitter(new plasmaEffect(spritesheet, x, y, int(r), int(g), int(b)));
	}
	
	enemyBullet::step(lvl);
		
	colBox.x = x;
	colBox.y = y-32*size;
}

void regularBullet::draw(painter* picasso){	
	int scale = (rand()%41)/10;
	spritesheet->setBlendMode(1);
	
	if (visible == 1){
		spritesheet->setAlpha(5);
	}
	spritesheet->setColor(int(r), int(g), int(b));
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-16*size-scale/2, y-48*size-scale/2, 64*size, 64*size, 0, 2);
	spritesheet->setColor(0xff, 0xff, 0xff);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-scale/2, y-32*size-scale/2, 32*size, 32*size, 0, 2);
	
	spritesheet->setAlpha(5);
	if (visible > 0){
		spritesheet->setAlpha(255);
	}	
	spritesheet->setColor(int(r), int(g), int(b));
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-16*size-scale/2, 448+(320-y*0.7143)-(48*size-scale/2)*0.7143, 
		64*size+scale, (64*size+scale)*0.7143, 0, 2);
	spritesheet->setColor(0xff, 0xff, 0xff);
	picasso->drawEx(spritesheet, 32, 64, 32, 32, x-scale/2, 448+(320-y*0.7143)-(32*size-scale/2)*0.7143, 
		32*size+scale, (32*size+scale)*0.7143, 0, 2);
		
	spritesheet->setAlpha(255);
	//debug
	//picasso->setColor(0xff, 0x00, 0x00, 0xff);
	//picasso->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
	//picasso->setColor(0xff, 0xff, 0xff, 0xff);
	
	spritesheet->setColor(0xff, 0xff, 0xff);
	spritesheet->setBlendMode(0);
}

//Bala que gira
spinningBullet::spinningBullet(LTexture* sprt, int X, int Y, double dir, double spd) 
  : regularBullet(sprt, X, Y, cos(dir/180.0*3.1415)*spd, sin(dir/180.0*3.1415)*spd){
  
	direction = dir;
	speed = spd;
	life = 300;
	
}

spinningBullet::~spinningBullet(){
	
}

void spinningBullet::step(level* lvl){
	direction += 1;
	if (direction > 360){
		direction = int(direction)%360;
	}
	
	spdX = cos(direction/180.0*3.1415)*speed;
	spdY = sin(direction/180.0*3.1415)*speed;
	
	regularBullet::step(lvl);
}

void spinningBullet::draw(painter* pintor){
	regularBullet::draw(pintor);
}

//MINE
bossMine::bossMine(LTexture* sprt, int X, int Y) : enemyBullet(sprt, X, Y, 0, 0){
	unsigned int frms[] = {9};
	mineAnim = new animation(1, 0, false, spritesheet, frms, 32);
	currentAnim = mineAnim;
	
	visible = 0;
	
	life = 300;
	maxLife = life;
	
	colBox.x = x+4;
	colBox.y = y+4;
	colBox.w = 24;
	colBox.h = 24;

	exploded = false;
}

bossMine::~bossMine(){
	if (mineAnim != NULL){
		delete mineAnim;
		mineAnim = NULL;
	}

}
	
void bossMine::step(level* lvl){
	
	if (alive){
		
		int playerX, playerY;
		lvl->getPlayerPos(playerX, playerY);
	
		playerX = playerX - x;
		playerY = playerY - y;
		double distance = sqrt(playerX*playerX + playerY*playerY);
	
		if (distance < 100 && !exploded){
			spdX += double(playerX)/distance;
			spdY += double(playerY)/distance;
		} else {
			spdX *= 0.98;
			spdY *= 0.98;
		}
		
		if (spdX > 1){
			spdX = 1;
		} else if (spdX < -1){
			spdX = -1;
		}
		
		if (spdY > 2){
			spdY = 2;
		} else if (spdY < -2){
			spdY = -2;
		}
		
		if (life == 10){
			colBox.x = x-8;
			colBox.y = y-8;
			colBox.w = 48;
			colBox.h = 48;
			
			lvl->addEmitter(new explosionEffect(lvl->getEffectSheet(), x, y));
			lvl->shake(1, 30);
			exploded = true;
		}
		
		if (life == 0 && !exploded){
			lvl->addEmitter(new explosionEffect(lvl->getEffectSheet(), x, y));
			lvl->shake(1, 30);
			exploded = true;
		}
	
		if (life%60 == 0 && !exploded){
			lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), x+8, y+8, 200, 0, 0, 2, 20));
		}

		enemyBullet::step(lvl);
		
		if (exploded){
			colBox.x = x-8;
			colBox.y = y-8;
		} else {
			colBox.x = x+4;
			colBox.y = y+4;			
		}
	}
	
	
}

void bossMine::draw(painter* pintor){
	double proportion = life/double(maxLife);
	spritesheet->setColor(255, 255*proportion, 255*proportion);
	if (!exploded){
		enemyBullet::draw(pintor);
	}
	spritesheet->setColor(255, 255, 255);
		
	//debug
	//pintor->setColor(200, 0, 0, 255);
	//pintor->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
	//pintor->setColor(0x17, 0x17, 0x17, 0xff);
}

//bossPillar
bossPillar::bossPillar(LTexture* sprt, int X) : enemyBullet(sprt, X, 96, 0, 0){
	currentAnim = NULL;
		
	visible = 0;
	
	life = 1;
	maxLife = life;
	
	colBox.x = x;
	colBox.y = y;
	colBox.w = 48;
	colBox.h = 320;
}

bossPillar::~bossPillar(){
	
}

void bossPillar::step(level* lvl){
	enemyBullet::step(lvl);
	
	colBox.x = x;
	colBox.y = y;
}

void bossPillar::draw(painter* pintor){
	//pintor->setColor(0, 200, 0, 255);
	//pintor->drawRect(colBox.x, colBox.y, colBox.w, colBox.h, 0);
	//pintor->setColor(0x17, 0x17, 0x17, 0xff);
}

//lightPillarEffect
lightPillarEffect::lightPillarEffect(LTexture* sprt, int X){
	spritesheet = sprt;
	x = X;
	alpha = 255;
	scale = 0.1;
	life = 360;
	maxLife = life;
	alive = true;
}

lightPillarEffect::~lightPillarEffect(){
	
}

bool lightPillarEffect::isAlive(){
	return alive;	
}

void lightPillarEffect::step(level* lvl){
	if (alive){
		life -= 1;
		if (life <= 270 && life > 30){
			lvl->addEnemyBullet(new bossPillar(NULL, x-24));
		}
		
		if (life > 300){
			scale = 0.05;
			alpha = 128;
		} else if (life > 270){
			scale = 1-((life-270)/30.0);
			if (scale < 0.05){
				scale = 0.05;
			}
			alpha = 100 + 155*scale;
		} else if (life < 30){
			scale = life/30.0;
			if (scale < 0.05){
				scale = 0.05;
			}
			alpha = scale*255;
		} else {
			scale = 1;
			alpha = 255;
		}	
		
		
		if (life <= 0){
			alive = false;
		}
	}
	
}

void lightPillarEffect::draw(painter* pintor){
	if (alive){
	
		spritesheet->setBlendMode(1);
		spritesheet->setAlpha(alpha);

		if (life <= 300){
			scale += (rand()%16)/100.0;
		} else {
			scale += (rand()%6)/100.0;
		}
		
		for (int i = 0; i < 5; ++i){
			pintor->drawEx(spritesheet, 0, 192, 64, 64, x-32*scale, 96+i*64, 64*scale, 64, 0, 0);
		}
		if (life <= 30){
			spritesheet->setAlpha(255*scale);
		} else{
			spritesheet->setAlpha(255);
		}
		for (int i = 0; i < 4 ; ++i)
			pintor->drawEx(spritesheet, 0, 128, 64, 64, x-48, 93, 96, 16, 0, 0);
		spritesheet->setBlendMode(0);
		spritesheet->setAlpha(255);		
	}	
}

//---------------------------------------------------------------------------------
//ESTADOS

stIdle::stIdle(){
	
}

stIdle::~stIdle(){
	
}

void stIdle::step(level* lvl, priest* p){
	
}

void stIdle::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	bossState* nextSt = NULL; 
	int chances = rand()%70+1;
	if (p->getPrevState() == "stPillars"){
		p->setTimer(240+290*proportion*proportion);
	} else {
		p->setTimer(30+380*proportion*proportion);
	}
	

	if (chances < 10){
		nextSt = new stTeleport();
	} else if (chances < 20){
		nextSt = new stDemonCrush();
	} else if (chances < 30){
		nextSt = new stDemonShoot();
	} else if (chances < 40){
		nextSt = new stMultishot();
	} else if (chances < 50){
		nextSt = new stDemonCharge();
	} else if (chances < 60){
		nextSt = new stThrowMine();		
	} else {
		if (p->getPrevState() == "stPillars"){
			nextSt = new stDemonCharge();
		} else {
			nextSt = new stPillars();
		}		
	}
	
	//nextSt = new stTeleport();
	//nextSt = new stDemonCrush();
	//nextSt = new stDemonShoot();
	//nextSt = new stMultishot();
	//nextSt = new stThrowMine();
	//nextSt = new stPillars();
	//nextSt = new stDemonCharge();
	
	p->setNextState(nextSt);
	p->setDevilAnim("idle");
}

void stIdle::exit(level* lvl, priest* p){
	
}

//teleport
unsigned int stTeleport::teleportNum = 0;

stTeleport::stTeleport(){
	teleportNum++;
	//std::cout << "teleNum " << teleportNum << std::endl;
}

stTeleport::~stTeleport(){
	
}

void stTeleport::step(level* lvl, priest* p){
	if (int(p->getTimer()) == 15){
		lvl->addEmitter(new hurtEffect(lvl->getEffectSheet(), nextX+12, nextY+20));
	}
}

void stTeleport::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	int X, Y;
	p->getPos(X, Y);
	
	nextX = 256+rand()%832;
	nextY = 240+rand()%120;
	
	if (abs(nextX-X) < 150){
		if (X < 683){
			nextX = 672+rand()%208;
		} else {
			nextX = 256+rand()%208;
		}
	}
	
	lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), nextX-32, nextY-32, 255, 255, 0, 2, 30, 1));
	
	p->setTimer(30+30*proportion);
	
	if ((rand()%3 == 0) || (teleportNum > 4) ){
		p->setNextState(new stIdle());
		teleportNum = 0;
	} else {
		if (proportion < 0.5){
			p->setNextState(new stThrowMine());
		} else {
			p->setNextState(new stTeleport());
		}
		
	}
	
	//p->setDevilAnim("idle");
}

void stTeleport::exit(level* lvl, priest* p){
	int X, Y;
	p->getPos(X, Y);
	lvl->addEmitter(new starEffect(lvl->getEffectSheet(), X, Y+16));
	p->setPos(nextX, nextY);
	p->setPrevState("stTeleport");	
}


//stDemonCrush
stDemonCrush::stDemonCrush(){
	
}

stDemonCrush::~stDemonCrush(){
	
}

void stDemonCrush::step(level* lvl, priest* p){

}

void stDemonCrush::enter(level* lvl, priest* p){
	//double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(150);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("falling");
}

void stDemonCrush::exit(level* lvl, priest* p){
	lvl->shake(2, 30);
	
	int X, Y;
	p->getPos(X, Y);
	
	lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 324, 384, 0, 200, 0, 2, 30));
	lvl->addEnemyBullet(new sineBullet(lvl->getEffectSheet(), 324, 384, true));
	lvl->addEnemyBullet(new sineBullet(lvl->getEffectSheet(), 324, 384, false));
	lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 1042, 384, 0, 200, 0, 2, 30));
	lvl->addEnemyBullet(new sineBullet(lvl->getEffectSheet(), 1042, 384, false));
	lvl->addEnemyBullet(new sineBullet(lvl->getEffectSheet(), 1042, 384, true));
	p->setPrevState("stDemonCrush");
}

//stDemonShoot
stDemonShoot::stDemonShoot(){
	
}

stDemonShoot::~stDemonShoot(){
	
}

void stDemonShoot::step(level* lvl, priest* p){
	if (int(p->getTimer()) == 100){
		double proportion = (p->getLives()/double(p->getMaxLives()));
		double speed = 3+2*proportion;
		
		int playerX, playerY, X, Y;
		lvl->getPlayerPos(playerX, playerY);
		
		X = playerX - 300;
		Y = playerY - 300;
		
		double normalized = sqrt(X*X + Y*Y);
		
		lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 300, 300, 0, 0, 200, 2, 30));
		lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 300, 200, 0, 0, 200, 2, 30));
		lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 1042, 300, 0, 0, 200, 2, 30));
		lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), 1042, 200, 0, 0, 200, 2, 30));
		
		regularBullet* bullet = new regularBullet(lvl->getEffectSheet(), 300, 300, 
			speed*(double(X)/normalized), (speed+1)*(double(Y)/normalized));
		bullet->setColor(0, 0, 200);
		bullet->setSize(2-proportion);
		bullet->setVisible(1);
		lvl->addEnemyBullet(bullet);
		
		X = playerX - 300;
		Y = playerY - 200;
		normalized = sqrt(X*X + Y*Y);
		
		bullet = new regularBullet(lvl->getEffectSheet(), 300, 200, 
			speed*(double(X)/normalized), speed*(double(Y)/normalized));
		bullet->setColor(0, 0, 200);
		bullet->setSize(2-proportion);
		bullet->setVisible(1);
		lvl->addEnemyBullet(bullet);
		
		X = playerX - 1042;
		Y = playerY - 300;
		normalized = sqrt(X*X + Y*Y);
		
		bullet = new regularBullet(lvl->getEffectSheet(), 1042, 300, 
			speed*(double(X)/normalized), (speed+1)*(double(Y)/normalized));
		bullet->setColor(0, 0, 200);
		bullet->setSize(2-proportion);
		bullet->setVisible(1);
		lvl->addEnemyBullet(bullet);
		
		X = playerX - 1042;
		Y = playerY - 200;
		normalized = sqrt(X*X + Y*Y);
		
		bullet = new regularBullet(lvl->getEffectSheet(), 1042, 200, 
			speed*(double(X)/normalized), speed*(double(Y)/normalized));
		bullet->setColor(0, 0, 200);
		bullet->setSize(2-proportion);
		bullet->setVisible(1);
		lvl->addEnemyBullet(bullet);
	}
}

void stDemonShoot::enter(level* lvl, priest* p){
	//double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(160);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("shoot");
}

void stDemonShoot::exit(level* lvl, priest* p){
	p->setPrevState("stDemonShoot");
}

//stDemonCharge
stDemonCharge::stDemonCharge(){
	charged = 0;
}

stDemonCharge::~stDemonCharge(){
	
}

void stDemonCharge::step(level* lvl, priest* p){
	int X, Y;
	X = 656;
	Y = 320;
	int timer = p->getTimer();
	
	regularBullet* bullet;
	
	if ((!charged) && (timer <= 35)){
		charged = 1;
		p->setDevilAnim("release");
		lvl->shake(1, 30);
		lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), X-16, Y-16, 200, 0, 0, 5, 30));
		
		
		double angle;
		for (int i = 0; i< 16; ++i){
			angle = (i/16.0)*2*3.1415;
			bullet = new regularBullet(lvl->getEffectSheet(), X, Y, cos(angle)*3, sin(angle)*4);
			bullet->setVisible(1);
			bullet->setColor(200, 0, 0);
			bullet->setSize(1);
			lvl->addEnemyBullet(bullet);
		}
	}
	
	if (!charged && (timer <= 200)){
		bullet = new regularBullet(lvl->getEffectSheet(), X+8+rand()%65-32, Y+rand()%65-32, 0, 0);
		bullet->setVisible(1);
		bullet->setColor(200, 0, 0);
		bullet->setSize(1);
		bullet->setLife(1);
		lvl->addEnemyBullet(bullet);
		
		if (timer%60 == 0){
			lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), X-16, Y-16, 200, 0, 0, 5, 30));
		}
	}
}

void stDemonCharge::enter(level* lvl, priest* p){
	//double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(320);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("charging");
}

void stDemonCharge::exit(level* lvl, priest* p){
	p->setPrevState("stDemonCharge");
}

//stPillars
stPillars::stPillars(){
	pattern = 0;
}

stPillars::~stPillars(){
	
}

void stPillars::step(level* lvl, priest* p){
	
	double T = p->getTimer();
		
	if (int(T)%20 == 0){
		switch(pattern){
		case 0:
			p->addPillar(lvl, 188+(int(T)/20)*100);
		break;
		case 1:
			p->addPillar(lvl, 1156-(int(T)/20)*100);
		break;
		case 2:
			if (T > 80){
				p->addPillar(lvl, 188+(int(T)/20)*100);
				p->addPillar(lvl, 1156-(int(T)/20)*100);
			}			
		break;
		case 3:
			if (T >60){
				p->addPillar(lvl, (int(T)/20)*64);
				p->addPillar(lvl, 1344-(int(T)/20)*64);
			}			
		break;
		}
	}
}

void stPillars::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	
	if (proportion < 0.5){
		pattern = rand()%4;
	} else {
		pattern = rand()%2;
	}
	
	p->setTimer(180);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("release");
		
}

void stPillars::exit(level* lvl, priest* p){
	p->setPrevState("stPillars");
}

//stThrowMine
stThrowMine::stThrowMine(){
	
}

stThrowMine::~stThrowMine(){
	
}

void stThrowMine::step(level* lvl, priest* p){
	if (p->getTimer() == 5){
		lvl->addEmitter(new hurtEffect(lvl->getEffectSheet(), nextX+12, nextY+20));
	}
}

void stThrowMine::enter(level* lvl, priest* p){
			
	int X, Y;
	p->getPos(X, Y);

	nextX = 256+rand()%832;
	nextY = 240+rand()%120;

	if (abs(nextX-X) < 150){
		if (X < 683){
			nextX = 672+rand()%208;
		} else {
			nextX = 256+rand()%208;
		}
	}
	
	lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), nextX-32, nextY-32, 255, 255, 0, 2, 30, 1));
	
	p->setTimer(30);
	p->setNextState(new stTeleport());		
}

void stThrowMine::exit(level* lvl, priest* p){
	int X, Y;
	p->getPos(X, Y);
	
	lvl->addEnemyBullet(new bossMine(p->getSpriteSheet(), X, Y+16));
	lvl->addEmitter(new starEffect(lvl->getEffectSheet(), X, Y+16));
	p->setPos(nextX, nextY);
	p->setPrevState("stThrowMine");
}

//stMultishot
stMultishot::stMultishot(){

}

stMultishot::~stMultishot(){
	
}

void stMultishot::step(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	int X, Y;
	p->getPos(X, Y);
	
	int timer = p->getTimer();

	regularBullet* bullet;
	unsigned int r, g, b;
	double angle;
	double startAngle = rand()%361;
	double speed = 2+3*proportion;
	if (timer < 30+40*(1-proportion)){
		if (timer%20 == 0){
			lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), X, Y, 255, 255, 255, 5, 30));
			//disparar balas para todos lados
			for (int i = 0; i< 10; ++i){
				angle = int((i/10.0)*360+startAngle)%360;
	
				bullet = new spinningBullet(lvl->getEffectSheet(), X+8, Y+32, angle, speed);
	
				lvl->getRandomColor(r, g, b);
				bullet->setColor(int(r), int(g), int(b));
	
				bullet->setSize(0.5);
				bullet->setVisible(0);
				lvl->addEnemyBullet(bullet);
			}
		}
	} else if (timer < 45+40*(1-proportion)){
		lvl->addEmitter(new hurtEffect(lvl->getEffectSheet(), X+12, 264));
	}
	
	if (timer == int(30+40*(1-proportion))){
		lvl->addEmitter(new starEffect(lvl->getEffectSheet(), X, Y+16));
		p->setPos(X, 244);
	} 
	
}

void stMultishot::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	int X, Y;
	p->getPos(X, Y);
	
	lvl->addEmitter(new waveEffect(lvl->getEffectSheet(), X-36, 200, 255, 255, 0, 2, 30, 1));
	
	p->setTimer(60+40*(1-proportion));
	p->setNextState(new stTeleport());
}

void stMultishot::exit(level* lvl, priest* p){
	p->setPrevState("stMultishot");
}


//FIN PRIEST

//----------------------------------------------------

//DEMON
demon::demon(LTexture* sprt, int X, int Y){
	spritesheet = sprt;
	
	x = X;
	y = Y;
	
	timer = 0;
	limb* miembroActual = NULL;
	
	brazoIzq = new limbSkeleton();
	brazoDer = new limbSkeleton();
	if (brazoIzq == NULL || brazoDer == NULL){
		std::cout << "Algo horrible pasó con el sistema de animación por huesos al crear los esqueletos..." << std::endl;
		exit(1);
	}
		
	// Construyo brazo izquierdo
	// "Brazo"
	miembroActual = brazoIzq->addLimb(sprt, -1); 
	miembroActual->setAngle(180);
	miembroActual->setMaxAngles(90, 270);
	miembroActual->setSprite(224, 160, 64, 160);
	miembroActual->setPivot(32, 16);
	miembroActual->setPos(x-100, y+4);	
	
	// "Antebrazo"
	miembroActual = brazoIzq->addLimb(sprt, 0);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-135, 135);
	miembroActual->setSprite(224, 160, 64, 160);
	miembroActual->setPivot(32, 16);
	miembroActual->setPos(0, 140);	
	
	// "Mano"
	miembroActual = brazoIzq->addLimb(sprt, 1);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-90, 90);
	miembroActual->setSprite(224, 96, 64, 64);
	miembroActual->setPivot(32, 16);
	miembroActual->setPos(0, 138);	
	
	
	// Construyo brazo derecho
	// "Brazo"
	miembroActual = brazoDer->addLimb(sprt, -1);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-90, 90);
	miembroActual->setSprite(224, 160, 64, 160);
	miembroActual->setPivot(32, 16);
	miembroActual->setFlip(1);	
	miembroActual->setPos(x+70, y); //en mirrored queda bien
	
	// "Antebrazo"
	miembroActual = brazoDer->addLimb(sprt, 0);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-135, 135);
	miembroActual->setSprite(224, 160, 64, 160);
	miembroActual->setPivot(32, 16);
	miembroActual->setFlip(1);	
	miembroActual->setPos(32, 155);
	
	// "Mano"
	miembroActual = brazoDer->addLimb(sprt, 1);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-90, 90);
	miembroActual->setSprite(224, 96, 64, 64);
	miembroActual->setPivot(32, 16);
	miembroActual->setFlip(1);	
	miembroActual->setPos(0, 136);
	
	//Animaciones:
	
	//Animaciones Idle
	idleIzqAnim = new limbAnim(brazoIzq, 1);
	idleDerAnim = new limbAnim(brazoDer, 1);
	if (idleIzqAnim == NULL || idleDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones -idle- para los brazos" << std::endl;
		exit(1);
	}
	
	idleIzqAnim->addKeyframe(90, 135, 0);
	idleIzqAnim->addKeyframe(90, 45, 1);
	idleIzqAnim->addKeyframe(90, 0, 2);
	idleIzqAnim->addKeyframe(180, 155, 0);
	idleIzqAnim->addKeyframe(180, 25, 2);
	
	idleDerAnim->addKeyframe(90, 45, 0);
	idleDerAnim->addKeyframe(90, -45, 1);
	idleDerAnim->addKeyframe(90, 0, 2);
	idleDerAnim->addKeyframe(180, 25, 0);	
	idleDerAnim->addKeyframe(180, -25, 2);
	
	//animaciones de "caer brazos"
	fallingIzqAnim = new limbAnim(brazoIzq, 0);
	fallingDerAnim = new limbAnim(brazoDer, 0);
	if (fallingIzqAnim == NULL || fallingDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones -falling- para los brazos" << std::endl;
		exit(1);
	}
	
	fallingIzqAnim->addKeyframe(120, 270, 0);
	fallingIzqAnim->addKeyframe(120, 45, 1);
	fallingIzqAnim->addKeyframe(120, 0, 2);
	fallingIzqAnim->addKeyframe(150, 135, 0);
		
	fallingDerAnim->addKeyframe(120, -90, 0);
	fallingDerAnim->addKeyframe(120, -45, 1);
	fallingDerAnim->addKeyframe(120, 0, 2);
	fallingDerAnim->addKeyframe(150, 45, 0);
	
	//animaciones de "cargar"
	chargingIzqAnim = new limbAnim(brazoIzq, 0);
	chargingDerAnim = new limbAnim(brazoDer, 0);
	if (chargingIzqAnim == NULL || chargingDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones -charging- para los brazos" << std::endl;
		exit(1);
	}
	
	chargingIzqAnim->addKeyframe(120, 200, 0);
	chargingIzqAnim->addKeyframe(120, -90, 1);
	chargingIzqAnim->addKeyframe(120, 0, 2);	
	
	chargingDerAnim->addKeyframe(120, -20, 0);
	chargingDerAnim->addKeyframe(120, 90, 1);
	chargingDerAnim->addKeyframe(120, 0, 2);
	
	//animaciones de "release"
	releaseIzqAnim = new limbAnim(brazoIzq, 0);
	releaseDerAnim = new limbAnim(brazoDer, 0);
	if (releaseIzqAnim == NULL || releaseDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones -release- para los brazos" << std::endl;
		exit(1);
	}
	
	releaseIzqAnim->addKeyframe(30, 270, 0);
	releaseIzqAnim->addKeyframe(30, 0, 1);
	releaseIzqAnim->addKeyframe(30, 0, 2);	
	
	releaseDerAnim->addKeyframe(30, -90, 0);
	releaseDerAnim->addKeyframe(30, 0, 1);
	releaseDerAnim->addKeyframe(30, 0, 2);
	
	//animaciones de disparo
	shootIzqAnim = new limbAnim(brazoIzq, 0);
	shootDerAnim = new limbAnim(brazoDer, 0);
	if (shootIzqAnim == NULL || shootDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones -disparo- para los brazos" << std::endl;
		exit(1);
	}
	
	shootIzqAnim->addKeyframe(60, 180, 0);
	shootIzqAnim->addKeyframe(60, 0, 1);
	shootIzqAnim->addKeyframe(60, 0, 2);
	shootIzqAnim->addKeyframe(70, 240, 0);
	shootIzqAnim->addKeyframe(70, 90, 1);
	shootIzqAnim->addKeyframe(70, 45, 2);
	shootIzqAnim->addKeyframe(80, 235, 0);
	shootIzqAnim->addKeyframe(80, 0, 2);
	
	shootDerAnim->addKeyframe(60, 0, 0);
	shootDerAnim->addKeyframe(60, 0, 1);
	shootDerAnim->addKeyframe(60, 0, 2);
	shootDerAnim->addKeyframe(70, -55, 0);
	shootDerAnim->addKeyframe(70, -90, 1);
	shootDerAnim->addKeyframe(70, -45, 2);
	shootDerAnim->addKeyframe(80, -50, 0);
	shootDerAnim->addKeyframe(80, 0, 2);
		
	//Seteo animaciones actuales
	currentIzqAnim = idleIzqAnim;
	currentDerAnim = idleDerAnim;
	currentAnim = "idle";
	
	armsTexture = NULL;
		
}

demon::~demon(){
	
	if (brazoIzq != NULL){
		delete brazoIzq;
		brazoIzq = NULL;
	}
	
	if (brazoDer != NULL){
		delete brazoDer;
		brazoDer = NULL;
	}
	
	
	if (idleIzqAnim != NULL){
		delete idleIzqAnim;
		idleIzqAnim = NULL;
	}
	
	if (idleDerAnim != NULL){
		delete idleDerAnim;
		idleDerAnim = NULL;
	}
	
	
}

void demon::setAnimation(const string& str){
		
	if (str == "idle"){
		currentDerAnim = idleDerAnim;
		currentIzqAnim = idleIzqAnim;
	} else if (str == "falling"){
		currentDerAnim = fallingDerAnim;
		currentIzqAnim = fallingIzqAnim;
	} else if (str == "charging"){
		currentDerAnim = chargingDerAnim;
		currentIzqAnim = chargingIzqAnim;
	} else if (str == "release"){
		currentDerAnim = releaseDerAnim;
		currentIzqAnim = releaseIzqAnim;
	} else if (str == "shoot"){
		currentDerAnim = shootDerAnim;
		currentIzqAnim = shootIzqAnim;
	} else {
		return;
	}
	
	currentAnim = str;
	
	currentDerAnim->reset();
	currentIzqAnim->reset();
}

	
void demon::step(level* lvl){
	
	timer += 1;
	if (timer > 360){
		timer = int(timer)%360;
	}
			
	if (currentIzqAnim != NULL)
		currentIzqAnim->step();
	if (currentDerAnim != NULL)
		currentDerAnim->step();	
	
}

void demon::draw(painter* pintor){
	//cara y cuerpo
	if (spritesheet != NULL){
		//pintor->drawEx(spritesheet, 0, 64, 224, 256, x-96, y-128, 224, 256, 0, 0);
		//pintor->drawEx(spritesheet, 160, 0, 64, 64, x-16, y-72, 64, 64, 0, 0);
		pintor->drawEx(spritesheet, 0, 64, 224, 256, x-96, 759-y, 224, 256*0.7143, 0, 2);
		pintor->drawEx(spritesheet, 160, 0, 64, 64, x-16, 432+(320-(y-108)), 64, 64*0.7143, 0, 2);
	}
	
	if (armsTexture == NULL){
		//La textura es muy grande, pero meh...
		armsTexture = pintor->newBlankTexture(1366, 448);
	}
	//No hago un else, porque así se ejecuta lo de arriba primero
	if (armsTexture != NULL){
		pintor->setRenderTarget(armsTexture);
		pintor->setColor(0, 0, 0, 0);
		pintor->clear();
		
		brazoDer->draw(pintor);
		brazoIzq->draw(pintor);
		
		pintor->resetRenderTarget();
		pintor->setColor(0x17, 0x17, 0x17, 0xff);
	}
	//pintor->draw(armsTexture, 0, 0, 0, 0, 0, 0);
	pintor->drawEx(armsTexture, 0, 0, 1366, 448, 0, 448, 1366, 448*0.7143, 0, 2);
	
}
