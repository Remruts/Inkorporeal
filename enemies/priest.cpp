#include "priest.h"
#include "enemy.h"
#include <iostream>

priest::priest(LTexture* sprt, int X, int Y) : enemy(sprt, X, Y){
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
	
	shadowDevil = new demon(spritesheet, x, y-32);
	
	nextState = NULL;
	state = new stIdle();
	state->enter(NULL, this);
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

void priest::setDevilAnim(const string& str){
	shadowDevil->setAnimation(str);
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

	}
	
}

void priest::draw(painter* pintor){
	
	//cool pero MUY ineficiente
	//for (int i = 0; i < 28; ++i)
	//	drawHalo(pintor, spritesheet, 224+i*33, 400, 270, 1, 3, 1);
	
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
	//int chances = rand()%80+1;

	p->setTimer(120+300*proportion);
	/*
	if (chances < 10){
		
	} else if (chances < 20){
		
	} else if (chances < 30){
		
	} else if (chances < 40){
		
	} else if (chances < 50){
		
	} else if (chances < 60){
		
	} else if (chances < 70){
		
	} else {
		
	}
	*/
	//nextSt = new stTeleport();
	nextSt = new stDemonCrush();
	
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
	//WIP cambiar efectos
	lvl->addEmitter(new hurtEffect(lvl->getEffectSheet(), nextX, nextY+16));
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
	//WIP cambiar efectos
	lvl->addEmitter(new starEffect(lvl->getEffectSheet(), X, Y+16));
	p->setPos(nextX, nextY);
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
	
	p->setTimer(160);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("falling");
}

void stDemonCrush::exit(level* lvl, priest* p){
	//TODO crear balas
}

//stDemonShoot
stDemonShoot::stDemonShoot(){
	
}

stDemonShoot::~stDemonShoot(){
	
}

void stDemonShoot::step(level* lvl, priest* p){
	
}

void stDemonShoot::enter(level* lvl, priest* p){
	//double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(160);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("shoot");
}

void stDemonShoot::exit(level* lvl, priest* p){
	//TODO crear balas
}

//stDemonCharge
stDemonCharge::stDemonCharge(){
	charged = 0;
}

stDemonCharge::~stDemonCharge(){
	
}

void stDemonCharge::step(level* lvl, priest* p){
	if ((!charged) && (p->getTimer() <= 35)){
		charged = 1;
		p->setDevilAnim("release");
		//TODO disparar	+ efecto shockwave
	}
	
	if (!charged){
		//TODO bola de energía
	}
}

void stDemonCharge::enter(level* lvl, priest* p){
	//double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(320);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("charging");
}

void stDemonCharge::exit(level* lvl, priest* p){

}

//stPillars
stPillars::stPillars(){
	fire = 0;
	pattern = 0;
}

stPillars::~stPillars(){
	
}

void stPillars::step(level* lvl, priest* p){
	/*
	double T = p->getTimer();
	double dT = T/160.0;
	
	Pillar* toSpawn;
	if (fire){
		toSpawn = new firePillar();
	} else {
		toSpawn = new beamPillar();
	}
	
	if (int(T)%20 == 0){
		switch(pattern){
		case 0:
			lvl->addEnemyBullet(toSpawn, 224+(int(T)%20)*100,120);
		break;
		case 1:
			lvl->addEnemyBullet(toSpawn, 764-(int(T)%20)*100,120);
		break:
		case 2:
			lvl->addEnemyBullet(toSpawn, 224+(int(T)%20)*100,120);
			lvl->addEnemyBullet(toSpawn, 764-(int(T)%20)*100,120);
		break;
		}
	}	
	*/
}

void stPillars::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	
	if (proportion < 0.5){
		pattern = rand()%3;
	} else {
		pattern = rand()%2;
	}
	
	fire = rand()%2;
	
	p->setTimer(160);
	
	p->setNextState(new stIdle());
	p->setDevilAnim("release");
}

void stPillars::exit(level* lvl, priest* p){

}

//stThrowMine
stThrowMine::stThrowMine(){
	
}

stThrowMine::~stThrowMine(){
	
}

void stThrowMine::step(level* lvl, priest* p){
	//WIP cambiar efectos
	lvl->addEmitter(new starEffect(lvl->getEffectSheet(), nextX, nextY+16));
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
	
	p->setTimer(30);
	p->setNextState(new stTeleport());		
}

void stThrowMine::exit(level* lvl, priest* p){
	int X, Y;
	p->getPos(X, Y);
	
	//WIP cambiar efectos
	//lvl->addEnemyBullet(new bossMine(), x, y+16);
	lvl->addEmitter(new starEffect(lvl->getEffectSheet(), X, Y+16));
	p->setPos(nextX, nextY);
}

//stMultishot
stMultishot::stMultishot(){

}

stMultishot::~stMultishot(){
	
}

void stMultishot::step(level* lvl, priest* p){
	//efecto de cargar
}

void stMultishot::enter(level* lvl, priest* p){
	double proportion = p->getLives()/double(p->getMaxLives());
	
	p->setTimer(30+30*proportion);	
	p->setNextState(new stIdle());
}

void stMultishot::exit(level* lvl, priest* p){
	//disparar balas para todos lados
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
