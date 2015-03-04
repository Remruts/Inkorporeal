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

//LIMBS

limb::limb(LTexture* sprt, limb* padre){
	
	spritesheet = sprt;
	
	sprtX = 0;
	sprtY = 0;
	
	// default 32
	sprtSizeX = 32;
	sprtSizeY = 32;
	
	x = 0;
	y = 0;
	
	pivotX = 0;
	pivotY = 0;
	
	angle = 0; 
	parent  = padre;
	
	mirroring = 0;
}

limb::~limb(){
	
}

//posición relativa
void limb::getPos(int &X, int &Y){
	X = x;
	Y = y;
}
//ángulo relativo en grados
double limb::getAngle(){
	return angle;
}

//obtener pivote
void limb::getPivot(int &X, int &Y){
	X = pivotX;
	Y = pivotY;
}

//setear ángulo relativo;
void limb::setAngle(double ang){
	if (ang < 0){
		ang = 360 + int(ang)%360;
	} else if (ang >= 360){
		ang = int(ang) % 360;
	}
	angle = ang;
}
//setear el pivote;
void limb::setPivot(int X, int Y){
	pivotX = X;
	pivotY = Y;
}

//qué sprite va a ser dibujado
void limb::setSprite(int X, int Y, int tamX, int tamY){
	sprtX = X;
	sprtY = Y;
	sprtSizeX = tamX;
	sprtSizeY = tamY;
}

//espejar: 0=no, 1=horizontal, 2=vertical, 3=ambos.
void limb::mirror(int m) {
	if (m<0){
		m = 0;
	} else if (m>3){
		m = m%4;
	}
	
	mirroring = m;
}

//setear posición
void limb::setPos(int X, int Y){
	x = X;
	y = Y;
}

//posición real en el mundo
void limb::getWorldPos(int &X, int &Y){
	X = 0;
	Y = 0;
	
	limb* nuevoPadre = parent;
	limb* miembroActual = this;
	
	int pivX, pivY;
	double parentAngle;
	int dstX, dstY;
	double magnitud;
	
	//Funciona, pero los sprites tienen que estar para abajo y coincidir los pivotes
	while(nuevoPadre != NULL){
		
		
		nuevoPadre->getPivot(pivX, pivY);
		
		//obtengo el ángulo real del padre 
		parentAngle = nuevoPadre->getWorldAngle();
		
		//primero calculo ángulo entre pivote padre y pivote hijo, y magnitud
		dstX = (miembroActual->x - miembroActual->pivotX)-pivX;
		dstY = (miembroActual->y - miembroActual->pivotY)-pivY;
		magnitud = sqrt(dstX*dstX+dstY*dstY);
		
		//Luego calculo la posición real
		X += cos(parentAngle/180.0*3.1415)*magnitud;
		Y += sin(parentAngle/180.0*3.1415)*magnitud;
		
		miembroActual = nuevoPadre;
		nuevoPadre = miembroActual->parent;
	}
	
	X += miembroActual->x;
	Y += miembroActual->y;	
	
}

//ángulo real
double limb::getWorldAngle(){
	limb* nuevoPadre = parent;
	double res = angle;
	while (nuevoPadre != NULL){
		res += nuevoPadre->angle;
		nuevoPadre = nuevoPadre->parent;
	}
	if (res>360){
		res = int(res)%360;
	}
	return res;
}

//dibujar
void limb::draw(painter* pintor){
	limb* miembro = this;
	int X, Y;
	while(miembro != NULL){
		
		miembro->getWorldPos(X, Y);
		
		pintor->setPivot(miembro->pivotX, miembro->pivotY);
		pintor->drawEx(miembro->spritesheet, miembro->sprtX, miembro->sprtY, miembro->sprtSizeX, miembro->sprtSizeY, X, Y, 
			miembro->sprtSizeX, miembro->sprtSizeY, miembro->getWorldAngle()-90, miembro->mirroring);
			
		miembro = miembro->parent;
	}
	pintor->defaultPivot();
}
//FIN LIMBS


//DEMON
demon::demon(LTexture* sprt, int X, int Y){
	spritesheet = sprt;
	
	angBrazoIzq = 180;
	
	x = X;
	y = Y;
	
	brazoIzq = new limb(sprt, NULL);
	brazoIzq->setAngle(angBrazoIzq);
	brazoIzq->setSprite(224, 160, 64, 160);
	brazoIzq->setPivot(32, 16);
	brazoIzq->setPos(x-100, y);
	
	antebrazoIzq = new limb(sprt, brazoIzq);
	antebrazoIzq->setAngle(0);
	antebrazoIzq->setSprite(224, 160, 64, 160);
	antebrazoIzq->setPivot(32, 16);
	antebrazoIzq->setPos(0, 140);
	
	manoIzq = new limb(sprt, antebrazoIzq);
	manoIzq->setAngle(0);
	manoIzq->setSprite(224, 96, 64, 64);
	manoIzq->setPivot(32, 16);
	manoIzq->setPos(0, 138);
	
	
	brazoDer = new limb(sprt, NULL);
	brazoDer->setAngle(angBrazoIzq-180);
	brazoDer->setSprite(224, 160, 64, 160);
	brazoDer->setPivot(32, 16);
	brazoDer->mirror(1);
	brazoDer->setPos(x+60, y-8);
	
	antebrazoDer = new limb(sprt, brazoDer);
	antebrazoDer->setAngle(0);
	antebrazoDer->setSprite(224, 160, 64, 160);
	antebrazoDer->setPivot(32, 16);
	antebrazoDer->mirror(1);
	antebrazoDer->setPos(0, 140);
	
	manoDer = new limb(sprt, antebrazoDer);
	manoDer->setAngle(0);
	manoDer->setSprite(224, 96, 64, 64);
	manoDer->setPivot(32, 16);
	manoDer->mirror(1);
	manoDer->mirror(1);
	manoDer->setPos(0, 136);
	
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
	
	if (antebrazoIzq != NULL){
		delete antebrazoIzq;
		antebrazoIzq = NULL;
	}
	
	if (antebrazoDer != NULL){
		delete antebrazoDer;
		antebrazoDer = NULL;
	}
	
	if (manoIzq != NULL){
		delete manoIzq;
		manoIzq = NULL;
	}
	
	if (manoDer != NULL){
		delete manoDer;
		manoDer = NULL;
	}
}
	
void demon::step(level* lvl){
	angBrazoIzq += 1;
	if (angBrazoIzq > 360){
		angBrazoIzq = int(angBrazoIzq)%360;
	}
	brazoIzq->setAngle(angBrazoIzq);
	antebrazoIzq->setAngle(angBrazoIzq);
	manoIzq->setAngle(angBrazoIzq);
	brazoDer->setAngle(-angBrazoIzq-180);
	antebrazoDer->setAngle(-angBrazoIzq-180);
	manoDer->setAngle(-angBrazoIzq-180);
}

void demon::draw(painter* pintor){
	//cara y cuerpo
	if (spritesheet != NULL){
		pintor->drawEx(spritesheet, 0, 64, 192, 256, x-96, y-128, 192, 256, 0, 0);
		pintor->drawEx(spritesheet, 160, 0, 64, 64, x-16, y-72, 64, 64, 0, 0);
	}
	
	//brazos
	manoDer->draw(pintor);
	manoIzq->draw(pintor);
}
