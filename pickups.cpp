#include "pickups.h"

pickup::pickup(LTexture* sprt, int X, int Y, int Lives){
	x = X;
	y = Y;
	
	spdX = 0;
	spdY = 0;
	
	accelX = 0;
	accelY = 0;
	visible = 2;
	alpha = 255;
	angle = 0;
	
	colBox.x = x;
	colBox.y = y;
	
	spritesheet = sprt;
	currentAnim = NULL;
	
	pickable = false;
	
	alive = true;
	lives = Lives;
	maxLives = lives;
	
	onGround = false;
}

pickup::~pickup(){
	
}

void pickup::onCollisionWithPlayer(level* lvl){
	die();
}

void pickup::step(level* lvl){
	if (angle>360){
		angle = angle % 360;
	}
	
	if (angle < 0){
		angle = 360+angle%360;
	}
	
	if (alive){
	
		if (lives < maxLives-5)
			pickable = true;
			
		colBox.x = x;
		colBox.y = y;
		
		if (lives <= 0){
			alive = false;
		}
		
		lives -= 1;
		
		if (spdX>0){
			spdX -= accelX;
		
			if (spdX <= 0.1){
				spdX = 0;
			}
			
			x += int(spdX);
		} else{
			spdX += accelX;
		
			if (spdX >= -0.1){
				spdX = 0;
			}
			
			x -= int(abs(spdX));
		}
		
		if (spdY>0){
			spdY -= accelY;
		
			if (spdY <= 0.1){
				spdY = 0;
			}
			
			y += int(spdY);
		} else{
			spdY += accelY;
		
			if (spdY >= -0.1){
				spdY = 0;
			}
			
			y -= int(abs(spdY));
		}
		
	}
	if (currentAnim != NULL)
		currentAnim->step();
}

//CTRL+C... CTRL+V
void pickup::draw(painter* disney){
	if (alive){
		if ((currentAnim != NULL) && ((lives > maxLives/4) || (lives%4<2))){
		
			currentAnim->setAngle(angle);
			spritesheet->setAlpha(alpha);
			
			if ((visible == 0) || (visible == 2)){
				currentAnim->draw(disney, x, y);
			}
			
			if (visible == 0){
				currentAnim->setStretch(1);
				spritesheet->setAlpha(10);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
				currentAnim->setStretch(0);
			}
			
			if (visible == 1){
				spritesheet->setAlpha(10);
				currentAnim->draw(disney, x, y);
				spritesheet->setAlpha(alpha);
			}
			
			if ((visible == 1) || (visible == 2)){
				currentAnim->setStretch(1);
				currentAnim->draw(disney, x, y);
				currentAnim->setStretch(0);
			}
		}
	}
}
	
void pickup::getPos(int &X, int &Y) const{
	X = x;
	Y = y;
}

SDL_Rect* pickup::getColBox(){
	return &colBox;
}
	
bool pickup::isAlive(){
	return alive;
}
void pickup::die(){
	alive = false;
}

bool pickup::isPickable(){
	return pickable;
}


coin::coin(LTexture* sprt, int X, int Y) : pickup(sprt, X, Y, 400){
	srand(time(NULL)+(X+Y)*(long int)(this));
	
	int random = (1-(rand()%2)*2);
	
	spdX = 0.5*random + random*(rand()%200)/100.0f;
	spdY = -5 - (rand()%300/100.0);
	
	accelY = 0.1;
	accelX = 0.005;
	
	unsigned int frms[] = {0, 1, 2, 3, 4, 5};
	monedita = new animation(6, 0.2, true, sprt, frms, 16);
	monedita->setCurrentFrame(rand()%5);
	currentAnim = monedita;
	
	colBox.x = x;
	colBox.y = y;
	colBox.w = 16;
	colBox.h = 16;
}

coin::~coin(){
	if (monedita != NULL){
		delete monedita;
		monedita = NULL;
	}
}

void coin::onCollisionWithPlayer(level* lvl){
	lvl->addEmitter(new coinSparkle(spritesheet, x+8, y));
	lvl->addPoints(5);
	pickup::onCollisionWithPlayer(lvl);
}

void coin::step(level* lvl){
	
	int colDisplace;
	if (spdY >= 0){
		colDisplace = lvl->vRaySolid(colBox.y+colBox.h, colBox.y+colBox.h+spdY+1, colBox.x+colBox.w/2);
	} else{
		colDisplace = lvl->vRaySolid(colBox.y, colBox.y+spdY-1, colBox.x+colBox.w/2);
	}
	
	if (colDisplace != -1){
		
		spdY *= -0.98;
		
		if (spdY <= 0.1 && spdY > -0.1){
			onGround = true;
			spdX *= 0.98; //fricción en piso
			spdY = 0;
		}
		
		if (spdY > 0){
			spdY = 0;
			y = colDisplace*32-colBox.h;
		} else {
			y = colDisplace*32+colBox.h;
		}
		
		colBox.y = y;
			
	} else {
		onGround = false;
	}
		
	if (!onGround && spdY<12){
		spdY+=0.5;
	}
	
	// Problemas con colisiones
	// Con estas líneas espero se solucionen
	if (lvl->isSolid(x, y)){
		if (spdY<0)
			y+=32;
		else
			y-=32;
	}
	
	if (spdX>=0){
		colDisplace = lvl->hRaySolid(colBox.x+colBox.w, colBox.x+colBox.w+spdX+1, colBox.y+colBox.h/2+spdY);
	} else {
		colDisplace = lvl->hRaySolid(colBox.x, colBox.x+spdX-1, colBox.y+colBox.h/2+spdY);
	}
	
	
	if (colDisplace != -1){
		if (spdX>=0)
			x = 192+colDisplace*32+(colBox.w+(colBox.x-x));
		else
			x = 192+colDisplace*32-(colBox.x-x);
		
		spdX *= -1;
		colBox.x = x;
	}
	
	pickup::step(lvl);
}

void coin::draw(painter* disney){
	pickup::draw(disney);
}


