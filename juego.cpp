//Implementación del juego
#include "juego.h"
#include <iostream> //debug

juego::juego(painter* p){
	leonardo = p;
	running = true;
	
	// posEnSheet.insert(pair<string, pair<int, int> >("mesa1", (make_pair(0, 0))));
	// Cargo las posiciones de los props en el mapa
	posEnSheet["mesa0"] = make_pair(0, 0);
	posEnSheet["mesa1"] = make_pair(1, 0);
	posEnSheet["silla0"] = make_pair(2, 0);
	posEnSheet["silla1"] = make_pair(3, 0);
	posEnSheet["pared0"] = make_pair(4, 0);
	posEnSheet["botella0"] = make_pair(0, 1);
	posEnSheet["botella1"] = make_pair(1, 1);
	posEnSheet["cuadro0"] = make_pair(2, 1);
	posEnSheet["cuadro1"] = make_pair(3, 1);
	posEnSheet["cuadro2"] = make_pair(4, 1);
	posEnSheet["vela0"] = make_pair(0, 2);
	posEnSheet["vela1"] = make_pair(1, 2);
	posEnSheet["cuadro3"] = make_pair(2, 2);
	posEnSheet["cuadro4"] = make_pair(3, 2);
	posEnSheet["cuadro5"] = make_pair(4, 2);
	posEnSheet["libro0"] = make_pair(0, 3);
	posEnSheet["libro1"] = make_pair(1, 3);
	posEnSheet["mueble0"] = make_pair(2, 3);
	posEnSheet["florero0"] = make_pair(3, 3);
	posEnSheet["florero1"] = make_pair(4, 3);
	posEnSheet["varanda0"] = make_pair(0, 4);
	posEnSheet["varanda1"] = make_pair(1, 4);
	posEnSheet["varanda2"] = make_pair(2, 4);
	posEnSheet["columna0"] = make_pair(3, 4);
	posEnSheet["columna1"] = make_pair(3, 5);
	posEnSheet["columna2"] = make_pair(3, 6);
	posEnSheet["butaca0"] = make_pair(0, 5);
	posEnSheet["piano0"] = make_pair(1, 5);
	/*
	std::cout << "Tamaño mapa: " << posEnSheet.size() << std::endl;
	
	for( map<string, pair<int, int> >::iterator ii=posEnSheet.begin(); ii!=posEnSheet.end(); ++ii){
		std::cout << (*ii).first << ": " << (*ii).second.first << ", " << (*ii).second.second << std::endl;
	}
	std::cout << "(" << posEnSheet["silla1"].first << ", " << posEnSheet["silla1"].second << ")" << std::endl;
	*/
	
	//Cargo tilesheet
	tiles = leonardo->loadImage("graphics/tiles.png");
	if (tiles == NULL){
		std::cout << "Error al cargar los gráficos de tiles. " << std::endl;
		exit(1);
	}
	
	props = leonardo->loadImage("graphics/props.png"); //cargar Surface de props
	if (props == NULL){
		std::cout << "Error al cargar los gráficos de props. " << std::endl;
		exit(1);
	}
	
	//cargar spritesheet del MC
	playerSprites = leonardo->loadTexture("graphics/player_sprites.png");
	if (playerSprites == NULL){
		std::cout << "Error al cargar los gráficos del MC. " << std::endl;
		exit(1);
	}
	
	//cargar spritesheet de enemigos
	enemySprites = leonardo->loadTexture("graphics/enemies1.png");
	if (enemySprites == NULL){
		std::cout << "Error al cargar los gráficos de los enemigos. " << std::endl;
		exit(1);
	}
	
	effectSheet = leonardo->loadTexture("graphics/effects.png");
	if (effectSheet == NULL){
		std::cout << "Error al cargar los gráficos de los efectos. " << std::endl;
		exit(1);
	}
	
	coinSheet = leonardo->loadTexture("graphics/coin.png");
	if (coinSheet == NULL){
		std::cout << "Error al cargar los gráficos de las moneditas. " << std::endl;
		exit(1);
	}
	
	//Creo un jugador nuevo
	jugador = new player(playerSprites);
	
	levelNum = 0;
	maxLevel = 1;
	transTimer = 4.0;
	//Cargo el nivel
	currentLevel = new level("levels/level0.lvl", this);
	//currentScreen = stTransition0;
	currentScreen = stPlaying;
}

juego::~juego(){
	
	if (currentLevel != NULL){
		delete currentLevel;
		currentLevel = NULL;
	}
	
	if (jugador != NULL){
		delete jugador;
		jugador = NULL;
	}
	
	if (tiles != NULL){
		leonardo->freeImage(tiles);
		tiles = NULL;
	}
	
	if (props != NULL){
		leonardo->freeImage(props);
		props = NULL;
	}
	
	if (playerSprites != NULL){
		leonardo->freeTexture(playerSprites);
		playerSprites = NULL;
	}
	
	if (enemySprites != NULL){
		leonardo->freeTexture(enemySprites);
		enemySprites = NULL;
	}
	
	if (effectSheet != NULL){
		leonardo->freeTexture(effectSheet);
		effectSheet = NULL;
	}
	
	if (coinSheet != NULL){
		leonardo->freeTexture(coinSheet);
		coinSheet = NULL;
	}
}

bool juego::isRunning(){
	return running;
}

void juego::step(control* c){
	
	if (c->esc){
		running = false;
		return;
	}
	if (currentScreen == stPlaying){
		if (currentLevel != NULL){
			if(currentLevel->isFinished()){
				if (currentLevel->getState() == level::stWin){

					currentScreen = stTransition0;
					
					levelNum += 1;
					if (levelNum > maxLevel)
						levelNum = 0;
					
					jugador->reset();
					jugador->step(currentLevel);
					delete currentLevel;
					currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
					
					transTimer = 4.0;
					
				} else {
					delete currentLevel;
					delete jugador;
					jugador = NULL;
					currentLevel = NULL;
					currentScreen = stTransition1;
					transTimer = 6.0;
				}
			}else {
				currentLevel->update(c);
			}
		}
	}
	
	if (currentScreen == stTransition0){
		transTimer -= 0.02;
		if (transTimer <= 0){
			currentScreen = stPlaying;
			transTimer = 4;
		}
	}
	
	if (currentScreen == stTransition1){
		transTimer -= 0.02;
		if (transTimer <= 0){
			currentScreen = stGameOver;
			transTimer = 6;
		}
	}
	
	if (currentScreen == stGameOver){
		if ((c->evJump) || (c->evShoot) || (c->evMelee) || (c->evDash)){
			//currentScreen = stMainMenu;
			levelNum = 0;
			jugador = new player(playerSprites);
			currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
			currentScreen = stTransition0;
		}
	}
	
}

void juego::draw(){
	if (currentScreen == stPlaying){
		leonardo->setColor(0x17, 0x17, 0x17, 255);
		if (currentLevel!=NULL){
			leonardo->clear();
			currentLevel->draw();
		}
	}
	
	if (currentScreen == stTransition1){
		leonardo->setBlendMode(1);
		leonardo->setColor(120, 0, 0, int(255*((6-transTimer)/6.0f)));
		
		leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
		
		leonardo->setBlendMode(0);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
	}
		
	if ((transTimer > 2.0) && (currentScreen == stTransition0)){
		leonardo->setBlendMode(1);
		leonardo->setColor(0x17, 0x17, 0x17, int(255*((2-transTimer)/2.0f)));
		
		leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
		
		leonardo->setBlendMode(0);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
	}
	
	if (currentScreen == stTransition0){
		
		if ((currentLevel!=NULL) && (transTimer <= 2.5)){
			currentLevel->draw();
			
			leonardo->setBlendMode(1);
			leonardo->setColor(0x17, 0x17, 0x17, int(255*(transTimer/2.5f)));
			
			leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
			
			leonardo->setBlendMode(0);
			leonardo->setColor(0x17, 0x17, 0x17, 255);
		}
	}
}

/*
bool juego::checkCollision(SDL_Rect* A, SDL_Rect* B){
	return ( ((B->x >= A->x) && (B->x <= A->w)) || ((A->x >= B->x) && (A->x <= B->w)) ) && //chequeo x
		( ((B->y >= A->y) && (B->y <= A->h)) || ((A->y >= B->y) && (A->y <= B->h)) ); //chequeo y
}
*/

map<string, pair<int, int> >* juego::getMap(){
	return &posEnSheet;
}

SDL_Surface* juego::getPropsheet(){
	return props;
}

SDL_Surface* juego::getTilesheet(){
	return tiles;
}

LTexture* juego::getPlayerSprites(){
	return playerSprites;
}

LTexture* juego::getEnemySprites(){
	return enemySprites;
}

LTexture* juego::getEffectSheet(){
	return effectSheet;
}

LTexture* juego::getCoinSheet(){
	return coinSheet;
}

int juego::getLevelNum(){
	return levelNum;
}

painter* juego::getPainter(){
	return leonardo;
}

player* juego::getPlayer(){
	return jugador;
}

juego::gameState juego::getState(){
	return currentScreen;
}
