//Implementación del level de niveles
#include "level.h"
#include "enemies/ghost.h"
#include "enemies/skelleton.h"
#include "enemies/demobat.h"
#include "enemies/imp.h"
#include "enemies/mask.h"
#include "enemies/jack.h"
#include "enemies/vlad.h"
#include "enemies/cherubil.h"
#include "enemies/priest.h"
#include <iostream> //debug

using std::vector;

// map<string, pair<int, int> > & posEnSheet
level::level(const string & filename, juego* game){

	finished = false;
	currentState = stPlaying;
	
	
	leonardo = game->getPainter();			// paso puntero a painter
	bach = game->getJukebox();				// paso puntero a jukebox
	
	jugador = game->getPlayer(); 			// paso puntero a jugador
	
	propSheet = game->getPropsheet(); 		// paso puntero a surface de props
	tileSheet = game->getTilesheet();		// paso puntero a surface de tiles
	enemySprites = game->getEnemySprites();	// paso puntero a sprites de enemigos
	vladSprites = game->getVladSprites();	// paso puntero a sprites de Vlad
	priestSprites = game->getPriestSprites();	// paso puntero a sprites del priest
	effectSheet = game->getEffectSheet();	// paso puntero a sprites de effectos
	coinSheet = game->getCoinSheet();		// paso puntero a sprites de monedita
	doorSheet = game->getDoorSheet();		// paso puntero a sprites de puerta
	
	soundBank = game->getSoundBank();
	musicBank = game->getMusicBank();
	
	hardcoreMode = game->getMode();
	std::cout << ((hardcoreMode) ? "HARDCORE 1337" : "n00b") << std::endl; 
	highscore = game->getHighscore();
	
	//Determino el número de nivel
	int num = game->getLevelNum();
	if (num > 99){
		levelnum = 99;
	} else{
		levelnum = num;
	}
	
	//Esta surface es un buffer con el cual opera la función de carga de archivo
	backSurface= SDL_CreateRGBSurface(0,960,448,32,0,0,0,0);
	if(backSurface == NULL) {
		std::cout << "Error al crear una nueva superficie para el fondo del nivel.\n";
        exit(1);
    }
    
    //inicialicemos en 0, por si acaso
    for (int i = 0; i< 420; i++){
    	solid[i] = 0;
    }
    
	//rectángulo del tilesheet
	rect.w = 32;
	rect.h = 32;
	rect.x = 0;
	rect.y = 0;
	
	//rectángulo en pantalla
	pos.x = 64;
	pos.y = 64;
	
    //Empecemos a cargar el nivel
	std::ifstream archivo;
	archivo.open(filename.c_str());
	bool ok = (archivo.good() && load(archivo, *(game->getMap())));
	if (!ok){
		std::cout << "No se pudo abrir el archivo del nivel o está corrupto" << std::endl;
		archivo.close();
		exit(1);
	}
	archivo.close();
	
	// Creo la textura del background a partir de la Surface modificada en el proceso de cargado
	background = leonardo->textureFromSurface(backSurface);
	leonardo->freeImage(backSurface); //ya no me sirve la superficie anterior...
	
	//Creo diversas texturas de textos
	if (hardcoreMode){
		testText = leonardo->textureFromText("{DEATH}", 3, 255, 255, 255);
	} else {
		testText = leonardo->textureFromText("{LIVES}", 3, 255, 255, 255);
	}
	
	if (levelnum<10){
		levelNumText = leonardo->textureFromText("Level 0"+std::to_string(levelnum), 3, 255, 255, 255);
	} else{
		levelNumText = leonardo->textureFromText("Level "+std::to_string(levelnum), 3, 255, 255, 255);
	}
	
	pointsText = leonardo->textureFromText("0 1 2 3 4 5 6 7 8 9  ", 2, 255, 255, 255);
	pointsText2 = leonardo->textureFromText("0123456789", 0, 255, 255, 255);
	
	if (lvlType >= 2){
		bossText = leonardo->textureFromText("BOSS", 3, 180, 0, 0);
	} else {
		bossText = NULL;
	}
	
	if (lvlType == 0){
		puerta = new door(doorSheet, 992, 352, false);
	} else if ((lvlType == 1) || (lvlType == 2)){
		puerta = new door(doorSheet, 992, 352, true); //default
	} else if (lvlType == 3){
		puerta = NULL;
	}
	
	llave = NULL;
	shakeTimer = 0;
	
	//bach->playMusic((*musicBank)["levelMusic"], 1, 0);
	
}

level::~level(){
	if (bossText != NULL){
		leonardo->freeTexture(bossText);
		bossText = NULL;
	}
	
	if (levelNumText != NULL){
		leonardo->freeTexture(levelNumText);
		levelNumText = NULL;
	}
	
	if (testText != NULL){
		leonardo->freeTexture(testText);
		testText = NULL;
	}
	
	if (pointsText != NULL){
		leonardo->freeTexture(pointsText);
		pointsText = NULL;
	}
	
	if (pointsText2 != NULL){
		leonardo->freeTexture(pointsText2);
		pointsText2 = NULL;
	}
	
	if (background != NULL){
		leonardo->freeTexture(background);
		background = NULL;
	}

	vector<bullet*>::iterator it = bulletList.begin();
	while (it != bulletList.end()){
		if (*it != NULL){
			delete *it;
			*it = NULL;
			it = bulletList.erase(it);
		}else{
			it++;	
		}
		
	}
	
	vector<enemy*>::iterator it2 = enemyList.begin();
	while (it2 != enemyList.end()){
		if (*it2 != NULL){
			delete *it2;
			*it2 = NULL;
			it2 = enemyList.erase(it2);
		}else{
			it2++;	
		}
		
	}
	
	//enemigos a agregar
	it2 = enemiesToAdd.begin();
	while (it2 != enemiesToAdd.end()){
		if (*it2 != NULL){
			delete *it;
			*it2 = NULL;
			it2 = enemiesToAdd.erase(it2);
		}else{
			it2++;	
		}
	}
	
	vector<enemyBullet*>::iterator it3 = enemyBulletList.begin();
	while (it3 != enemyBulletList.end()){
		if (*it3 != NULL){
			delete *it3;
			*it3 = NULL;
			it3 = enemyBulletList.erase(it3);
		}else{
			it3++;	
		}
	}
	
	//partículas
	vector<emitter*>::iterator itEmitter = emitterList.begin();
	while (itEmitter != emitterList.end()){
		if (*itEmitter != NULL){
				delete *itEmitter;
				(*itEmitter) = NULL;
				itEmitter = emitterList.erase(itEmitter);
		} else{
			itEmitter = emitterList.erase(itEmitter);
		}
	}
	
	//pickups
	vector<pickup*>::iterator itPickup = pickupList.begin();
	while (itPickup != pickupList.end()){
		if (*itPickup != NULL){
				delete *itPickup;
				(*itPickup) = NULL;
				itPickup = pickupList.erase(itPickup);
		} else{
			itPickup = pickupList.erase(itPickup);
		}
	}
	
	//points
	vector<movingPoints*>::iterator itPoints = pointsList.begin();
	while (itPoints != pointsList.end()){
		if (*itPoints != NULL){
			delete *itPoints;
			(*itPoints) = NULL;
			itPoints = pointsList.erase(itPoints);
		} else{
			itPoints = pointsList.erase(itPoints);
		}
	}
	
	if (puerta != NULL){
		delete puerta;
		puerta = NULL;
	}
	
	if (llave != NULL){
		delete llave;
		llave = NULL;
	}

}

bool level::isFinished(){
	return finished;
}

level::lvlState level::getState(){
	return currentState;
}

void level::setHighscore(long int hs){
	highscore = hs;
}

void level::setHardcore(bool h){
	hardcoreMode = h;
	
	if (testText != NULL){
		leonardo->freeTexture(testText);
		testText = NULL;
	}
	
	if (hardcoreMode){
		testText = leonardo->textureFromText("{DEATH}", 3, 255, 255, 255);
	} else {
		testText = leonardo->textureFromText("{LIVES}", 3, 255, 255, 255);
	}
}

void level::addPoints(int p, int x, int y){
	jugador->addPoints(p);
	pointsList.push_back(new movingPoints(pointsText2, x, y-16, p));
}

void level::addLife(){
	jugador->addLives();
}

void level::playSound(const string& str){
	bach->playSound((*soundBank)[str]);
}

void level::updatePlayer(control *c){
	//WIP: meter parte de este código en step() de player
	if (jugador!=NULL){
		jugador->step(this);
		
		int playerX, playerY;
		double spdX, spdY;
		const SDL_Rect colBox = jugador->getColBox();
		player::stateEnum playerState = jugador->getState();
		jugador->getPos(playerX, playerY);
		jugador->getSpeed(spdX, spdY);
		
		//Si presiono "disparo"
		if (c->evShoot){
			jugador->shoot();
		}
		
		//Si presiono derecha
		if (c->evRight && !c->evLeft){
			if (spdX<0)
				spdX = 0;
			spdX += 1; //sumo velocidad
			if (playerState == player::stIdle){
				jugador->setState(player::stRun);
			}
			if (!jugador->isShooting() &&(playerState != player::stDash)){
				jugador->setFlip(1);
			}
		}
		//Si presiono izquierda
		if (c->evLeft && !c->evRight){
			if (spdX>0)
				spdX = 0;
			spdX -= 1; //quito velocidad
			if (playerState == player::stIdle){
				jugador->setState(player::stRun);
			}
			if (!jugador->isShooting() && (playerState != player::stDash)){
				jugador->setFlip(0);
			}
		}
		
		//Si presiono ambos lados, quiero que pare
		if (c->evLeft && c->evRight){
			if (playerState == player::stRun){
				jugador->setState(player::stIdle);
			}
		}
		
		int iesimo; //variable loca
		if (spdY>=0)
			iesimo = int(colBox.x+colBox.w/2-192)/32 + 30*(int(colBox.y+colBox.h+spdY)/32);
		else
			iesimo = int(colBox.x+colBox.w/2-192)/32 + 30*(int(colBox.y+spdY)/32);
		
		//cuando se choca con el piso
		if (solid[iesimo]){
			if (spdY>=0 && playerState == player::stJump){
				jugador->setState(player::stIdle);
				jugador->setPos(playerX, 32*int(playerY/32));
			}
			spdY = 0;
		} else { //cuando está en el aire
			if (playerState != player::stDash)
				jugador->setState(player::stJump);
		}
		
		//presiona el botón de salto
		if (c->evJump){
			if (solid[iesimo] && (playerState != player::stJump) && (playerState != player::stDash)){
				spdY -= 10;
				playSound("jumpSound");
				jugador->setState(player::stJump);
			}
		}
		
		if (spdX>0){
			iesimo = int(colBox.x+colBox.w-192+spdX)/32 + 30*(int((colBox.y+colBox.h)+spdY-1)/32);
		} else {
			iesimo = int(colBox.x-192+spdX)/32 + 30*(int((colBox.y+colBox.h)+spdY-1)/32);
		}
		
		if (c->evDash && !jugador->isDashing()){
			if (c->evRight && !solid[iesimo]){
				//jugador->setPos(playerX+15, playerY);
				jugador->setState(player::stDash);
				spdX = 20;
				spdY = 0;
				c->evDash = false;
			}
			if (c->evLeft && !solid[iesimo]){
				//jugador->setPos(playerX-15, playerY);
				jugador->setState(player::stDash);
				spdX = -20;
				spdY = 0;
				c->evDash = false;
			}
		}
		
		int colSpace;
		
		if (spdX > 0){
			colSpace = hRaySolid(colBox.x+colBox.w, colBox.x+colBox.w+spdX, colBox.y + colBox.h/2 +spdY);
			//spdX -= colBox.x+colBox.w-colSpace;
			if (colSpace != -1){
				spdX = 0;
				jugador->setPos(192+(colSpace*32)+7, playerY);
			}
		} else if (spdX < 0){
			colSpace = hRaySolid(colBox.x, colBox.x+spdX, colBox.y + colBox.h/2 +spdY);
			//spdX += colSpace-colBox.x;
			if (colSpace != -1){
				spdX = 0;
				jugador->setPos(192+(colSpace*32)-7, playerY);
			}
		}
		
		/*
		if ( (spdX > 0 && solid[iesimo]) || 
			((spdX < 0 && solid[iesimo])) ){
			spdX = 0;
			//jugador->setSpeed(0, spdY);	
		}
		*/
		jugador->setSpeed(spdX, spdY);
		
	}
}

void level::updateBullets(){
	vector<bullet*>::iterator it = bulletList.begin();
	while (it != bulletList.end()){
	
		if (*it != NULL){
			if ((*it)->isAlive()){
				(*it)->step(this);
				it++;
			} else{
				delete *it;
				*it = NULL;
				it = bulletList.erase(it);
			}
		}
	}
	
	SDL_Rect* cBox;
	vector<enemyBullet*>::iterator it2 = enemyBulletList.begin();
	while (it2 != enemyBulletList.end()){
	
		if (*it2 != NULL){
			if ((*it2)->isAlive()){
				(*it2)->step(this);
				
				cBox = (*it2)->getColBox();				
				if ((cBox->y+cBox->h > 416) || (cBox->y < 32) || (cBox->x < 224) || (cBox->x+cBox->w > 1120)){
					(*it2)->die();
				}
				
				it2++;
			} else{
				delete *it2;
				*it2 = NULL;
				it2 = enemyBulletList.erase(it2);
			}
		}
	}
}

void level::updateEnemies(){
	
	vector<enemy*>::iterator it = enemyList.begin();
	while (it != enemyList.end()){
		if (*it != NULL){
			if ((*it)->isAlive()){
				(*it)->step(this);
				it++;
			} else{
				
				playSound("explosionSound");
				shake(1, 10);
				
				int x, y;
				(*it)->getPos(x, y);
				
				colourExplosion* exp = new colourExplosion(effectSheet, x, y, leonardo);
				addEmitter(exp);
				
				coin *c;
				int max = (*it)->getMaxLives() * 2+1;
				if (max > 1){
					for (int i = 0; i<max; ++i){
						c = new coin(coinSheet, x, y);
						addPickup(c);
					}
				}
				
				addPoints((*it)->getMaxLives()*100, x, y);

				if (hardcoreMode){
					if (enemyList.size() == 1){
						addKey(x, y);
					}
				} else{
					if ((lvlType == 1) && (llave == NULL) && (rand()%enemyList.size() == 0)){
						addKey(x, y);
					}
				}
				
				delete *it;
				*it = NULL;
				it = enemyList.erase(it);
			}
		}
		
	}

}

void level::updateEmitters(){
	//partículas
	vector<emitter*>::iterator itEmitter = emitterList.begin();
	while (itEmitter != emitterList.end()){
		if (*itEmitter != NULL){
			if ((*itEmitter)->isAlive()){
				(*itEmitter)->step(this);
				itEmitter++;
			}else{
				delete *itEmitter;
				(*itEmitter) = NULL;
				itEmitter = emitterList.erase(itEmitter);
			}
		} else{
			itEmitter = emitterList.erase(itEmitter);
		}
	}
}

void level::updatePickups(){
	//pickups
	vector<pickup*>::iterator itPickup = pickupList.begin();
	while (itPickup != pickupList.end()){
		if (*itPickup != NULL){
			if ((*itPickup)->isAlive()){
				(*itPickup)->step(this);
				itPickup++;
			}else{
				delete *itPickup;
				(*itPickup) = NULL;
				itPickup = pickupList.erase(itPickup);
			}
		} else{
			itPickup = pickupList.erase(itPickup);
		}
	}
}

void level::updatePoints(){
	//points
	vector<movingPoints*>::iterator it = pointsList.begin();
	while (it != pointsList.end()){
		if ((*it) != NULL){
			if ((*it)->isAlive()){
				(*it)->step();
				it++;
			}else{
				delete *it;
				(*it) = NULL;
				it = pointsList.erase(it);
			}
		} else{
			it = pointsList.erase(it);
		}
	}
}

void level::updateDoor(){
	if (puerta != NULL){
		puerta->step();
		
		if ((jugador != NULL) && (jugador->getLives()>0) 
			&& (checkCollision(&jugador->getColBox(), puerta->getColBox()))){
			if (puerta->isOpened()){
				currentState = stWin;
			} else {
				if (llave != NULL && llave->wasPicked() && !llave->wasUsed()){
					puerta->unlock();
					llave->use();
				}
				puerta->open();
			}
		}
	}
}

void level::updateKey(){
	if (llave!=NULL){
		llave->step(this);
		
		if (jugador != NULL){
			int playerX, playerY;
			jugador->getPos(playerX, playerY);
			
			if (llave->wasPicked()){
				llave->setPos(playerX-16, playerY-32);
			} else {
				SDL_Rect* pos = llave->getColBox();
				if ((abs(pos->x+pos->w/2-playerX) <= 48) && (abs(pos->y+pos->h/2-playerY) <= 48)){
					llave->pick();
					playSound("keySound");
				}
			}
		}
		
	}
}

void level::checkBulletCollisions(){
	vector<bullet*>::iterator it = bulletList.begin();
	vector<enemy*>::iterator it2 = enemyList.begin();
	
	while (it != bulletList.end()){
		it2 = enemyList.begin();
		
		if ((*it) != NULL && (*it)->isAlive()){
			while(it2 != enemyList.end()){
				if ((*it2 != NULL) && (*it2)->isAlive() && checkCollision((*it2)->getColBox(), (*it)->getColBox())){
					(*it)->die(this);
					(*it2)->getHurt();
					break;
				}
				it2++;
			}
		}
		it++;
	}
	
	vector<enemyBullet*>::iterator it3 = enemyBulletList.begin();
	while (it3 != enemyBulletList.end()){
		if ((*it3) != NULL && (*it3)->isAlive()){
			if ((jugador != NULL) && (jugador->getLives() > 0) && checkCollision((*it3)->getColBox(), &jugador->getColBox())){
				(*it3)->die();
				jugador->getHurt(this);
			}
		}
		it3++;
	}
	
}

void level::checkPlayerEnemyCollisions(){
	if ((jugador != NULL) && (jugador->getLives()>0)){
		
		SDL_Rect* colBox = new SDL_Rect(jugador->getColBox());
		
		vector<enemy*>::iterator it = enemyList.begin();
	
		while (it != enemyList.end()){
			if ((*it != NULL) && (*it)->isAlive() && checkCollision(colBox, (*it)->getColBox())){
				jugador->getHurt(this);
			}
			it++;
		}
		delete colBox;
	}
	
}

void level::checkPlayerPickup(){
	if ((jugador != NULL) && (jugador->getLives()>0)){
		
		SDL_Rect* colBox = new SDL_Rect(jugador->getColBox());
		
		vector<pickup*>::iterator it = pickupList.begin();
	
		while (it !=  pickupList.end()){
			if ((*it != NULL) && (*it)->isAlive() && (*it)->isPickable() && checkCollision(colBox, (*it)->getColBox())){
				(*it)->onCollisionWithPlayer(this);
			}
			it++;
		}
		delete colBox;
	}
}

void level::update(control* c){
	
	//screen shake
	if (shakeTimer > 0){
		shakeTimer -= 1;
		if (shakeTimer == 0){
			leonardo->setShake(0);
		}
	}
		
	checkBulletCollisions();
	checkPlayerEnemyCollisions();
	checkPlayerPickup();
	updateDoor();
	updateKey();
	
	updateBullets();
	updatePlayer(c);
	updatePickups();
	updateEnemies();
	addEnemies();
	updateEmitters();
	updatePoints();
	
	
	if (jugador->getLives() <= 0){
		currentState = stLose;
	} else if (enemyList.size() == 0){
		//currentState = stWin;
	}
	if ((currentState == stWin) || (currentState == stLose)){
		finished = true;
		shakeTimer = 1;
		leonardo->setShake(0);
	}
}

bool level::checkCollision(const SDL_Rect* A, const SDL_Rect* B){
	return ( ((B->x >= A->x) && (B->x <= A->x+A->w)) || ((A->x >= B->x) && (A->x <= B->x+B->w)) ) && //chequeo x
		( ((B->y >= A->y) && (B->y <= A->y+A->h)) || ((A->y >= B->y) && (A->y <= B->y+B->h)) ); //chequeo y
}

void level::shake(double intensity, int time){
	if (intensity > leonardo->getShake()){
		leonardo->setShake(intensity);
	}
	shakeTimer = std::max(time, shakeTimer);
}


void level::addBullet(bullet* b){
	bulletList.push_back(b);
}

void level::addEnemy(enemy* e){
	enemiesToAdd.emplace_back(e);
}

void level::addEnemies(){
	vector<enemy*>::iterator it = enemiesToAdd.begin();
	
	while (it != enemiesToAdd.end()){
		if ((*it) != NULL){
			enemyList.emplace_back((*it));
		} 
		it++;
	}
	
	it = enemiesToAdd.begin();
	while (it != enemiesToAdd.end()){
		if (*it != NULL){
			*it = NULL;
			it = enemiesToAdd.erase(it);
		}else{
			it++;	
		}
	}
	
}

void level::addEnemyBullet(enemyBullet* b){
	enemyBulletList.push_back(b);
}

void level::addEmitter(emitter* e){
	emitterList.push_back(e);
}

void level::addPickup(pickup* p){
	pickupList.push_back(p);
}

void level::addKey(int X, int Y){
	llave = new key(doorSheet, X, Y);
	playSound("keyAppearsSound");
}

void level::draw(){	
	// background
	leonardo->draw(background, 0, 0, 960, 448, 192, 0);
	// background espejado
	leonardo->drawEx(background, 0, 0, 960, 448, 192, 448, 960, 320, 0, 2);
	
	if (puerta!=NULL){
		puerta->draw(leonardo);
	}
	
	//enemigos
	vector<enemy*>::iterator it2 = enemyList.begin();
	while (it2 != enemyList.end()){
		if (*it2 != NULL){
			(*it2)->draw(leonardo);
		}
			
		it2++;
	}
	
	// disparos jugador
	vector<bullet*>::iterator it = bulletList.begin();
	while (it != bulletList.end()){
		if (*it != NULL)
			(*it)->draw(leonardo);
		it++;
	}
	
		
	//balas enemigos
	vector<enemyBullet*>::iterator it3 = enemyBulletList.begin();
	while (it3 != enemyBulletList.end()){
		if (*it3 != NULL)
			(*it3)->draw(leonardo);
		it3++;
	}
	
	if (llave != NULL){
		llave->draw(leonardo);
	}
	
	//pickups
	vector<pickup*>::iterator itPickup = pickupList.begin();
	while (itPickup != pickupList.end()){
		if (*itPickup != NULL)
			(*itPickup)->draw(leonardo);
		itPickup++;
	}
	
	// jugador
	if (jugador!=NULL)
		jugador->draw(leonardo);
	
	//partículas
	vector<emitter*>::iterator itEmitter = emitterList.begin();
	while (itEmitter != emitterList.end()){
		if (*itEmitter != NULL)
			(*itEmitter)->draw(leonardo, background);
		itEmitter++;
	}
	
	// texto/UI
	if (testText != NULL){
		if (hardcoreMode){
			leonardo->draw(testText, 0, 0, 0, 0, 6, 108);
		} else{
			leonardo->draw(testText, 0, 0, 0, 0, 24, 108);
		}
	}
	
	if ((levelNumText != NULL) && (bossText == NULL)){
		leonardo->draw(levelNumText, 0, 0, 0, 0, 1175, 48);
	}
	
	if (bossText != NULL){
		leonardo->draw(bossText, 0, 0, 0, 0, 1200, 48);
	}
	
	drawPoints();
}

void level::drawPoints(){
	long int points = jugador->getPoints();
	int power;
	for (int i = 0; i<10; i++){
		power = points%10;
		leonardo->drawEx(pointsText, 24*power+power-1, 0, 24, 32, 1330-18*i, 104, 24, 32, 0, 0);
		points/=10;
	}
	
	//highscore
	points = jugador->getPoints();
	pointsText->setColor(200, 0, 0);
	if (highscore > points){
		points = highscore;
	}
	
	for (int i = 0; i<10; i++){
		power = points%10;
		leonardo->drawEx(pointsText, 24*power+power-1, 0, 24, 32, 1330-18*i, 134, 24, 32, 0, 0);
		points/=10;
	}
	pointsText->setColor(255, 255, 255);
	
	//puntos que aparecen cuando se obtiene algo
	vector<movingPoints*>::iterator it = pointsList.begin();
	while (it != pointsList.end()){
		if (*it != NULL)
			(*it)->draw(leonardo);
		it++;
	}
	
}


//determina si un punto está sobre el jugador
bool level::isPlayer(int x, int y) const{
	const SDL_Rect* colBox = &jugador->getColBox();
	
	return ((x >= colBox->x) && (x <= colBox->x+colBox->w) && 
		(y >= colBox->y) && (y <= colBox->y+colBox->h));
	
}

void level::getPlayerPos(int &x, int &y){
	jugador->getPos(x, y);
}

LTexture* level::getEffectSheet(){
	return effectSheet;
}

void level::getRandomColor(unsigned int &R, unsigned int &G, unsigned int &B){
	leonardo->getRandomColor(R, G, B);
}

//determina si un punto está sobre algo sólido
bool level::isSolid(int x, int y) const{
	
	int X = (x-192)/32;
	int Y = y/32;
	
	return solid[X+Y*30];
}
// Determina si hay algo sólido en una línea horizontal del punto x1 al x2
// Si hubiese, devuelve la posición de contacto (en x), sino 0
int level::hRaySolid(int x1, int x2, int y) const{
	int dist = x2-x1;
	int X = (x1-192)/32;
	int Y = y/32;
	
	if (dist > 0){
		for (int i = 0; i <= dist/32; ++i){
			
			if (X+i > 28){
				return 28;
			}
			
			if (solid[X+i+Y*30]){
				return X+i-1;
			}
		}
	} else if (dist < 0){
		dist = -dist;
		for (int i = 0; i <= dist/32; ++i){
			if (X+i < 1){
				return 1;
			}
			if (solid[X+i+Y*30]){
				return X+i+1;
			}
		}
	} else if (isSolid(x1, y)){
		return X;
	}
	
	return -1;
}
// Determina si hay algo sólido en una línea vertical del punto y1 al y2
// Si hubiese, devuelve la posición de contacto en y, sino -1
int level::vRaySolid(int y1, int y2, int x) const{
	int dist = y2-y1;
	int X = (x-192)/32;
	int Y = y1/32;
	
	if (dist > 0){
		for (int i = 0; i <= dist/32; ++i){
			
			if (Y+i > 12){
				return 12;
			}
			
			if (solid[X+(Y+i)*30]){
				return Y+i-1;
			}
		}
	} else if (dist < 0){
		dist = -dist;
		for (int i = 0; i <= dist/32; ++i){
			if (Y+i < 1){
				return 1;
			}
			if (solid[X+(Y+i)*30]){
				return Y+i+1;
			}
		}
	} else if (isSolid(x, y1)){
		return Y;
	}
	
	return -1;
}

//función para ignorar espacios, saltos de línea y comentarios
void level::ignore(std::istream& is){
	string str;	
	while((is.peek() == 10) || (is.peek() == '/') || (is.peek() == ' ')){
			getline(is, str);
	}
}

int level::load(std::istream& is, map<string, pair<int, int> >& posEnSheet){
	char c;
	string str;
	
	//variables para la creación del nivel
	int floor = -1;
	int rug = -1;
	int wall = -1;
	int papering = -1;
	
	while(is.good()){
		do {
			//Salteo todo hasta próxima sección
			is >> c;
			//obtengo el nombre de la sección
			getline(is, str);
		} while (c != '>');

		// muestro el nombre de la sección, por qué no?
		std::cout << str << std::endl;
		if (str == "type"){
			ignore(is);
			is >> lvlType; //Cargo tipo
			std::cout << "tipoNivel: " << lvlType << std::endl;
			if (lvlType > 3){
				std::cout << "Archivo corrupto!\n";
				return 0;
			}
			ignore(is);
		}
		
		if (str == "tiles"){
			ignore(is);
			is >> rug; //Cargo rug
			ignore(is);
			is >> wall; //Cargo wall
			ignore(is);
			is >> papering; //Cargo papering
			ignore(is);
			is >> floor; //Cargo floor
			ignore(is);
			
			//Si no son los valores correctos, falla
			if (rug > 1 || rug < 0 || wall > 4 || wall < 0 
				|| papering > 4 || papering < 0 || floor > 7 || floor < 0){
				std::cout << "Archivo corrupto!\n";
				return 0;
			}
			
			switch(wall){
			case 0:
				//pos tile de "pared"
				rect.x = 96;
				rect.y = 0;
				rect.w = 32;
				rect.h = 32;
				
				for (int i = 0; i<420; ++i){ //30x14
					//rectángulo en pantalla
					pos.x = i%30 * 32;
					pos.y = i/30 * 32;
					//screw encapsulation.
					SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
				}
			break;
			case 1:
				//pos tile de "pared"
				rect.x = 0;
				rect.y = 32;
				rect.w = 64;
				rect.h = 64;
				
				for (int i = 0; i<105; ++i){ //15x7
					//rectángulo en pantalla
					pos.x = i%15 * 64;
					pos.y = i/15 * 64;
					//screw encapsulation.
					SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
				}
			break;
			
			case 2:
				//pos tile de "pared"
				rect.w = 64;
				rect.h = 64;
				rect.x = 64;
				rect.y = 32;
				
				for (int i = 0; i<105; ++i){ //15x7
					//rectángulo en pantalla
					pos.x = i%15 * 64;
					pos.y = i/15 * 64;
					//screw encapsulation.
					SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
				}
			break;
			
			case 3:
				//pos tile de "pared"
				rect.x = 0;
				rect.y = 96;
				rect.w = 32;
				rect.h = 32;
				
				for (int i = 0; i<420; ++i){ //30x14
					//rectángulo en pantalla
					pos.x = i%30 * 32;
					pos.y = i/30 * 32;
					//screw encapsulation.
					SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
				}
			break;
			
			case 4:
				//pos tile de "pared"
				rect.x = 32;
				rect.y = 96;
				rect.w = 32;
				rect.h = 32;
				
				for (int i = 0; i<420; ++i){ //30x14
					//rectángulo en pantalla
					pos.x = i%30 * 32;
					pos.y = i/30 * 32;
					//screw encapsulation.
					SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
				}
			break;

			}
		} //Termina tiles
		
		if (str == "map"){
			ignore(is);
			//tile de "piso"
			rect.w = 32;
			rect.h = 32;
			rugrect.w = 32;
			rugrect.h = 32;
			
			switch(floor){
			case 0:
				rect.x = 64;
				rect.y = 96;
				rugrect.x = 96;
				rugrect.y = 96;
			break;
			case 1:
				rect.x = 96;
				rect.y = 0;
				rugrect.x = 32;
				rugrect.y = 128;
			break;
			case 2:
				rect.x = 96;
				rect.y = 0;
				rugrect.x = 64;
				rugrect.y = 128;
			break;
			case 3:
				rect.x = 96;
				rect.y = 0;
				rugrect.x = 96;
				rugrect.y = 128;
			break;
			case 4:
				rect.x = 0;
				rect.y = 192;
				rugrect.x = 0;
				rugrect.y = 160;
			break;
			case 5:
				rect.x = 32;
				rect.y = 192;
				rugrect.x = 32;
				rugrect.y = 160;
			break;
			case 6:
				rect.x = 64;
				rect.y = 192;
				rugrect.x = 64;
				rugrect.y = 160;
			break;
			case 7:
				rect.x = 96;
				rect.y = 192;
				rugrect.x = 96;
				rugrect.y = 160;
			break;
			}
			
			for (int i = 0; i< 420; ++i){
				is.get(c); //Obtengo el caracter sin formato (o sea, hasta espacios)
				//Si fuera '#', es sólido
				int prevX, prevY;
				if (c == '#'){ 
					solid[i] = 1; 
					//rectángulo en pantalla
					pos.x = i%30 * 32;
					pos.y = i/30 * 32;
					//Arreglar para que se usen texturas
					//screw encapsulation.
					if (i >= 30 && solid[i-30] == 0){
						if (rug == 1){ //Alfombra
							SDL_BlitSurface(tileSheet, &rugrect, backSurface, &pos);
							
						} else {
							SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
						}
						
						if (papering > 0){
							prevX = rect.x;
							prevY = rect.y;
							
							if (papering == 4){
								rect.x = 0;
								rect.y = 128;
							}else{
								rect.x = papering*32-32;
								rect.y = 0;
							}
							
		
							pos.x = i%30 * 32;
							pos.y = i/30 * 32 - 32;
							SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
							
							rect.x = prevX;
							rect.y = prevY;
						}
						
					} else {
						SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
					}			
				} else if (c == 'H'){
					pos.x = i%30 * 32+192;
					pos.y = i/30 * 32;
					pickupList.push_back(new heart(coinSheet, pos.x, pos.y));
				} else if (c == '0'){
					pos.x = i%30 * 32+192;
					pos.y = i/30 * 32;
					pickupList.push_back(new floatingCoin(coinSheet, pos.x, pos.y));
				}
				is.get(c); //salteo el '|' o '\n'
				
			}
			/*
			std::cout << "\n\nQueda así\n";
			for (int j = 0; j< 14; ++j){
				for (int i = 0; i< 30; ++i){
					std::cout << ((solid[j*30+i])? '#' : ' ') << "|";
				}
				std::cout << std::endl;
			}
			*/
			
		} //Termina map
		
		if (str == "props"){
			
			ignore(is);
			while(is.peek() != '>' && is.good()){
				getline(is, str, ' ');
				std::cout << str << " ";
				rect.x = posEnSheet[str].first * 32;
				rect.y = posEnSheet[str].second * 32;
				is >> pos.x;
				is >> pos.y;
				std::cout << "x: " << pos.x << ", y: " << pos.y << std::endl;
				SDL_BlitSurface(propSheet, &rect, backSurface, &pos);
				
				ignore(is);
			}
		}
		
		if (str == "enemies"){
			
			ignore(is);
			enemy* toSpawn = NULL;
			while(is.peek() != '>' && is.good()){

				getline(is, str, ' ');
				std::cout << str << " ";
				
				is >> pos.x;
				pos.x *= 32;
				std::cout << pos.x << " ";

				is >> pos.y;
				pos.y *= 32;
				std::cout << pos.y << std::endl;
			
				if (str == "ghost"){
					toSpawn = new ghost(enemySprites, pos.x, pos.y);
				} else if (str == "skelleton"){
					toSpawn = new skelleton(enemySprites, pos.x, pos.y);
				} else if (str == "imp"){
					toSpawn = new imp(enemySprites, pos.x, pos.y);
				} else if (str == "demobat"){
					toSpawn = new demobat(enemySprites, pos.x, pos.y);
				}else if (str == "mask"){
					toSpawn = new mask(enemySprites, pos.x, pos.y);
				} else if (str == "jack"){
					toSpawn = new jack(enemySprites, pos.x, pos.y);
				} else if (str == "vlad"){
					toSpawn = new vlad(vladSprites, pos.x, pos.y);
				} else if (str == "cherubil"){
					toSpawn = new cherubil(enemySprites, pos.x, pos.y);
				} else if (str == "priest"){
					toSpawn = new priest(priestSprites, pos.x, pos.y);
				}else {
					std::cout << "Falla cargar enemigos. Archivo corrupto.\n";
					exit(1);
				}
				enemyList.push_back(toSpawn);
				ignore(is);
			}
		}
		
		if (str == "items"){
			ignore(is);
			pickup* item = NULL;
			
			while(is.peek() != '>' && is.good()){

				getline(is, str, ' ');
				std::cout << str << " ";
				
				is >> pos.x;
				pos.x = pos.x*32+192;
				std::cout << pos.x << " ";

				is >> pos.y;
				pos.y *= 32;
				std::cout << pos.y << std::endl;
			
				if (str == "tomato"){
					item = new heart(coinSheet, pos.x, pos.y);
				} else if (str == "monedita"){
					item = new floatingCoin(coinSheet, pos.x, pos.y);
				} else{
					std::cout << "Falla cargar items. Archivo corrupto.\n";
					exit(1);
				}
				pickupList.push_back(item);
				ignore(is);
			}
			
		}
		
		if (str == "end"){
			break;
		}
		
	}
	
	return 1;	
}

