//Implementación del level de niveles
#include "level.h"
#include "ghost.h"
#include <iostream> //debug

using namespace std;

// map<string, pair<int, int> > & posEnSheet
level::level(const string & filename, juego* game){

	finished = false;
	currentState = stPlaying;
	
	propSheet = game->getPropsheet(); 		// paso puntero a surface de props
	tileSheet = game->getTilesheet();		// paso puntero a surface de tiles
	leonardo = game->getPainter();			// paso puntero a painter
	jugador = game->getPlayer(); 			// paso puntero a jugador
	enemySprites = game->getEnemySprites();	// paso puntero a sprites de enemigos
	effectSheet = game->getEffectSheet();	// paso puntero a sprites de effectos
	
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
	ifstream archivo;
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
	testText = leonardo->textureFromText("{LIVES}", 2, 255, 255, 255);
	if (levelnum<10){
		levelNumText = leonardo->textureFromText("Level 0"+to_string(levelnum), 2, 255, 255, 255);
	} else{
		levelNumText = leonardo->textureFromText("Level "+to_string(levelnum), 2, 255, 255, 255);
	}
	
}

level::~level(){
	if (levelNumText != NULL){
		leonardo->freeTexture(levelNumText);
		levelNumText = NULL;
	}
	
	if (testText != NULL){
		leonardo->freeTexture(testText);
		testText = NULL;
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

}

bool level::isFinished(){
	return finished;
}

level::lvlState level::getState(){
	return currentState;
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
			if (playerState != player::stDash)
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
}

void level::updateEnemies(){
	vector<enemy*>::iterator it = enemyList.begin();
	while (it != enemyList.end()){
		if (*it != NULL){
			if ((*it)->isAlive()){
				(*it)->step(this);
				it++;
			} else{
				int x, y;
				(*it)->getPos(x, y);
				srand(time(NULL)*(x+y));
				
				int effect;
				int scale;
				
				leonardo->setRenderTarget(background);
				effectSheet->setBlendMode(1);
				effectSheet->setAlpha(200);
				for (int i = 0; i<4; ++i){
					effect = rand()%9;
					scale = 32*(rand()%3+1);
					effectSheet->setColor(rand()%256, rand()%256, rand()%256);
						leonardo->drawEx(effectSheet, 32*(effect%3), 32*(effect/3), 32, 32, 
						x+rand()%32-16 - 192, y+rand()%32-16, scale, scale, rand()%360, rand()%4);
				}
				leonardo->resetRenderTarget();
				delete *it;
				*it = NULL;
				it = enemyList.erase(it);
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
					(*it)->die();
					(*it2)->getHurt();
					break;
				}
				it2++;
			}
		}
		it++;
	}
}

void level::checkPlayerEnemyCollisions(){
	if ((jugador != NULL) && (jugador->getLives()>0)){
		
		SDL_Rect* colBox = new SDL_Rect(jugador->getColBox());
		
		vector<enemy*>::iterator it = enemyList.begin();
	
		while (it != enemyList.end()){
			if ((*it != NULL) && (*it)->isAlive() && checkCollision(colBox, (*it)->getColBox())){
				jugador->getHurt();
			}
			it++;
		}
		delete colBox;
	}
	
}

void level::update(control* c){
	checkBulletCollisions();
	checkPlayerEnemyCollisions();
	updateBullets();
	updatePlayer(c);
	updateEnemies();
	
	if (jugador->getLives() <= 0){
		currentState = stLose;
	} else if (enemyList.size() == 0){
		currentState = stWin;
	}
	if ((currentState == stWin) ||(currentState == stLose)){
		finished = true;
	}
}

bool level::checkCollision(SDL_Rect* A, SDL_Rect* B){
	return ( ((B->x >= A->x) && (B->x <= A->x+A->w)) || ((A->x >= B->x) && (A->x <= B->x+B->w)) ) && //chequeo x
		( ((B->y >= A->y) && (B->y <= A->y+A->h)) || ((A->y >= B->y) && (A->y <= B->y+B->h)) ); //chequeo y
}


void level::addBullet(bullet* b, bool type){
	if (type)
		bulletList.push_back(b);
}

void level::draw(){
	// background
	leonardo->draw(background, 0, 0, 960, 448, 192, 0);
	// background espejado
	leonardo->drawEx(background, 0, 0, 960, 448, 192, 448, 960, 320, 0, 2);
	
	// disparos jugador
	vector<bullet*>::iterator it = bulletList.begin();
	while (it != bulletList.end()){
		if (*it != NULL)
			(*it)->draw(leonardo);
		it++;
	}
	
	//enemigos
	vector<enemy*>::iterator it2 = enemyList.begin();
	while (it2 != enemyList.end()){
		if (*it2 != NULL)
			(*it2)->draw(leonardo);
		it2++;
	}
	
	// jugador
	if (jugador!=NULL)
		jugador->draw(leonardo);
	
	// texto/UI
	leonardo->draw(testText, 0, 0, 0, 0, 24, 64);
	leonardo->draw(levelNumText, 0, 0, 0, 0, 1175, 64);
}


//determina si un punto está sobre el jugador
bool level::isPlayer(int x, int y) const{
	const SDL_Rect* colBox = &jugador->getColBox();
	
	return ((x >= colBox->x) && (x <= colBox->x+colBox->w) && 
		(y >= colBox->y) && (y <= colBox->y+colBox->h));
	
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
			
			if (solid[X+i+Y*30]){
				return X+i-1;
			}
		}
	} else if (dist < 0){
		dist = -dist;
		for (int i = 0; i <= dist/32; ++i){
			std::cout << i << std::endl;
			if (Y+i < 1){
				return 1;
			}
			if (solid[X+i+Y*30]){
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
	
		if (str == "tiles"){
			ignore(is);
			is >> rug; //Cargo rug
			ignore(is);
			is >> wall; //Cargo wall
			ignore(is);
			is >> papering; //Cargo papering
			ignore(is);
			
			//Si no son los valores correctos, falla
			if (rug > 1 || wall > 4 || papering > 3){
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
			rect.x = 64;
			rect.y = 96;
			
			for (int i = 0; i< 420; ++i){
				is.get(c); //Obtengo el caracter sin formato (o sea, hasta espacios)
				//Si fuera '#', es sólido
				if (c == '#'){ 
					solid[i] = 1; 
					//rectángulo en pantalla
					pos.x = i%30 * 32;
					pos.y = i/30 * 32;
					//screw encapsulation.
					if (i >= 30 && solid[i-30] == 0){
						if (rug == 1){ //Alfombra
						
							rect.x = 96; //posición en tilesheet de alfombra
							SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);							
							rect.x = 64; //vuelvo a la del piso
							
						} else {
							SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
						}
						
						if (papering > 0){
							rect.x = papering*32-32;
							rect.y = 0;
		
							pos.x = i%30 * 32;
							pos.y = i/30 * 32 - 32;
							SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
							
							rect.x = 64;
							rect.y = 96;
						}
						
					} else {
						SDL_BlitSurface(tileSheet, &rect, backSurface, &pos);
					}			
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
				cout << str << " ";
				rect.x = posEnSheet[str].first * 32;
				rect.y = posEnSheet[str].second * 32;
				is >> pos.x;
				is >> pos.y;
				cout << "x: " << pos.x << ", y: " << pos.y << endl;
				SDL_BlitSurface(propSheet, &rect, backSurface, &pos);
				
				ignore(is);
			}
		}
		
		if (str == "enemies"){
			
			ignore(is);
			enemy* toSpawn = NULL;
			while(is.peek() != '>' && is.good()){

				getline(is, str, ' ');
				cout << str << " ";
				
				is >> pos.x;
				pos.x *= 32;
				cout << pos.x << " ";

				is >> pos.y;
				pos.y *= 32;
				cout << pos.y << endl;
			
				if (str == "ghost"){
					toSpawn = new ghost(enemySprites, pos.x, pos.y);
				}
				enemyList.push_back(toSpawn);
				ignore(is);
			}
		}
		
		if (str == "end"){
			break;
		}
		
	}
	
	return 1;	
}


