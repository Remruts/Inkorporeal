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
	colBox.y = y;
	colBox.w = 24;
	colBox.h = 32;
	
	lives = 200;
	maxLives = lives;
	
	haloAngle = 0;
	haloColor = 0;
	
	shadowDevil = new demon(spritesheet, x+156, y-32);
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

void priest::step(level* lvl){
	if (alive){
	
		if (hurt && hurtTimer >= 2){
			//lvl->playSound("priestSound");
		}
		
		timer -= 1;
		
		if (timer <= 0){
			timer = rand()%30+30;
		}
	
		if (((x >= 1088) && (spdX > 0)) || ((x <= 224) && (spdX < 0))){
			spdX = -spdX*2;
		}
		if (((y+32 >= 416) && (spdY > 0)) || ((y <= 32) && (spdY < 0))){
			spdY = -spdY*2;
		}
		
	}
	
	enemy::step(lvl);
	
	if (shadowDevil != NULL){
		shadowDevil->step(lvl);
	}
	
	colBox.x = x+4;
	colBox.y = y;
	
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
	
	/*
	//demon	
	pintor->drawEx(spritesheet, 0, 64, 192, 256, x-96, y-128, 192, 256, 0, 0);
	pintor->drawEx(spritesheet, 160, 0, 64, 64, x-16, y-72, 64, 64, 0, 0);
	pintor->setPivot(32, 16);
	pintor->drawEx(spritesheet, 224, 160, 64, 160, x+40, y, 64, 160, 270+flap, 0);
	pintor->drawEx(spritesheet, 224, 160, 64, 160, x-90, y, 64, 160, 90-flap, 1);
	pintor->defaultPivot();
	*/
	
	shadowDevil->draw(pintor);
	
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
//FIN PRIEST

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
	miembroActual->setPos(x-100, y);
	
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
	miembroActual->mirror(1);
	miembroActual->setPos(x+65, y);
	
	// "Antebrazo"
	miembroActual = brazoDer->addLimb(sprt, 0);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-135, 135);
	miembroActual->setSprite(224, 160, 64, 160);
	miembroActual->setPivot(32, 16);
	miembroActual->mirror(1);
	miembroActual->setPos(0, 140);
	
	// "Mano"
	miembroActual = brazoDer->addLimb(sprt, 1);
	miembroActual->setAngle(0);
	miembroActual->setMaxAngles(-90, 90);
	miembroActual->setSprite(224, 96, 64, 64);
	miembroActual->setPivot(32, 16);
	miembroActual->mirror(1);
	miembroActual->mirror(1);
	miembroActual->setPos(0, 136);
	
	//Animaciones:
	
	//Animaciones Idle
	idleIzqAnim = new limbAnim(brazoIzq, 1);
	idleDerAnim = new limbAnim(brazoDer, 1);
	if (idleIzqAnim == NULL || idleDerAnim == NULL){
		std::cout << "No pudieron crearse las animaciones Idle para los brazos" << std::endl;
		exit(1);
	}
	
	
	idleIzqAnim->addKeyframe(60, 135, 0);
	idleIzqAnim->addKeyframe(60, 45, 1);
	idleIzqAnim->addKeyframe(60, 45, 2);
	idleIzqAnim->addKeyframe(120, 180, 0);
	idleIzqAnim->addKeyframe(120, 0, 1);
	idleIzqAnim->addKeyframe(120, 0, 2);
	
	idleDerAnim->addKeyframe(60, -45, 0);
	idleDerAnim->addKeyframe(120, 0, 0);
	
	
	currentIzqAnim = idleIzqAnim;
	currentDerAnim = idleDerAnim;
		
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
		pintor->drawEx(spritesheet, 0, 64, 224, 256, x-96, y-128, 224, 256, 0, 0);
		pintor->drawEx(spritesheet, 160, 0, 64, 64, x-16, y-72, 64, 64, 0, 0);
	}
	
	brazoDer->draw(pintor);
	brazoIzq->draw(pintor);
}
