//Implementación del juego
#include "juego.h"
#include <iostream> //debug

juego::juego(painter* p, jukebox* b){
	leonardo = p;
	bach = b;
	running = true;
	
	// posEnSheet.insert(pair<string, pair<int, int> >("mesa1", (std::make_pair(0, 0))));
	// Cargo las posiciones de los props en el mapa
	posEnSheet["mesa0"] = std::make_pair(0, 0);
	posEnSheet["mesa1"] = std::make_pair(1, 0);
	posEnSheet["silla0"] = std::make_pair(2, 0);
	posEnSheet["silla1"] = std::make_pair(3, 0);
	posEnSheet["pared0"] = std::make_pair(4, 0);
	posEnSheet["botella0"] = std::make_pair(0, 1);
	posEnSheet["botella1"] = std::make_pair(1, 1);
	posEnSheet["cuadro0"] = std::make_pair(2, 1);
	posEnSheet["cuadro1"] = std::make_pair(3, 1);
	posEnSheet["cuadro2"] = std::make_pair(4, 1);
	posEnSheet["vela0"] = std::make_pair(0, 2);
	posEnSheet["vela1"] = std::make_pair(1, 2);
	posEnSheet["cuadro3"] = std::make_pair(2, 2);
	posEnSheet["cuadro4"] = std::make_pair(3, 2);
	posEnSheet["cuadro5"] = std::make_pair(4, 2);
	posEnSheet["libro0"] = std::make_pair(0, 3);
	posEnSheet["libro1"] = std::make_pair(1, 3);
	posEnSheet["mueble0"] = std::make_pair(2, 3);
	posEnSheet["florero0"] = std::make_pair(3, 3);
	posEnSheet["florero1"] = std::make_pair(4, 3);
	posEnSheet["varanda0"] = std::make_pair(0, 4);
	posEnSheet["varanda1"] = std::make_pair(1, 4);
	posEnSheet["varanda2"] = std::make_pair(2, 4);
	posEnSheet["columna0"] = std::make_pair(3, 4);
	posEnSheet["columna1"] = std::make_pair(3, 5);
	posEnSheet["columna2"] = std::make_pair(3, 6);
	posEnSheet["butaca0"] = std::make_pair(0, 5);
	posEnSheet["piano0"] = std::make_pair(1, 5);
	posEnSheet["candelabro0"] = std::make_pair(0, 6);
	posEnSheet["cadena0"] = std::make_pair(1, 6);
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
	
	//cargar spritesheet de enemigos
	vladSprites = leonardo->loadTexture("graphics/vlad_sprites.png");
	if (vladSprites == NULL){
		std::cout << "Error al cargar los gráficos de Vlad. " << std::endl;
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
	
	doorSheet = leonardo->loadTexture("graphics/door.png");
	if (doorSheet == NULL){
		std::cout << "Error al cargar los gráficos de las puertas. " << std::endl;
		exit(1);
	}
	
	titleScreen = leonardo->loadTexture("graphics/title.png");
	if (titleScreen == NULL){
		std::cout << "Error al cargar los gráficos del título. " << std::endl;
		exit(1);
	}
	
	invertedTitle = leonardo->loadTexture("graphics/title_inverted.png");
	if (invertedTitle == NULL){
		std::cout << "Error al cargar los gráficos del título. " << std::endl;
		exit(1);
	}
	
	pressStart = leonardo->loadTexture("graphics/press_start.png");
	if (pressStart == NULL){
		std::cout << "Error al cargar los gráficos del título. " << std::endl;
		exit(1);
	}
	
	gameOverSprite = leonardo->loadTexture("graphics/game_over.png");
	if (gameOverSprite == NULL){
		std::cout << "Error al cargar los gráficos de game over. " << std::endl;
		exit(1);
	}
	YNSprite = leonardo->loadTexture("graphics/YN.png");
	if (YNSprite == NULL){
		std::cout << "Error al cargar los gráficos de game over. " << std::endl;
		exit(1);
	}
	continueSprite = leonardo->loadTexture("graphics/continue_sprite.png");
	if (continueSprite == NULL){
		std::cout << "Error al cargar los gráficos de game over. " << std::endl;
		exit(1);
	}
	continueSelected = true;
	
	soundBank["coinSound"] = bach->loadSound("sounds/coin.wav");
	bach->soundSetVolume(soundBank["coinSound"], 0.2);
	if (!soundBank["coinSound"]){
		std::cout << "Error al cargar sonido de moneda. " << std::endl;
	}
	
	soundBank["dashSound"] = bach->loadSound("sounds/firethrow.wav");
	bach->soundSetVolume(soundBank["dashSound"], 0.6);
	if (!soundBank["dashSound"]){
		std::cout << "Error al cargar sonido de dash. " << std::endl;
	}
	
	soundBank["shootSound"] = bach->loadSound("sounds/pchew.wav");
	bach->soundSetVolume(soundBank["shootSound"], 0.4);
	if (!soundBank["shootSound"]){
		std::cout << "Error al cargar sonido de disparo. " << std::endl;
	}
	
	soundBank["shootSound2"] = bach->loadSound("sounds/inazumakick.wav");
	bach->soundSetVolume(soundBank["shootSound2"], 0.6);
	if (!soundBank["shootSound2"]){
		std::cout << "Error al cargar sonido de disparo2. " << std::endl;
	}
	
	soundBank["getHitSound"] = bach->loadSound("sounds/large_footsteps.wav");
	bach->soundSetVolume(soundBank["getHitSound"], 0.8);
	if (!soundBank["getHitSound"]){
		std::cout << "Error al cargar sonido de herido. " << std::endl;
	}
	
	soundBank["explosionSound"] = bach->loadSound("sounds/hit.wav");
	if (!soundBank["explosionSound"]){
		std::cout << "Error al cargar sonido de explosion. " << std::endl;
	}
	
	soundBank["jumpSound"] = bach->loadSound("sounds/jump0.wav");
	if (!soundBank["jumpSound"]){
		std::cout << "Error al cargar sonido de salto. " << std::endl;
	}
	
	soundBank["batSound"] = bach->loadSound("sounds/jump0.wav");
	bach->soundSetVolume(soundBank["batSound"], 0.4);
	if (!soundBank["batSound"]){
		std::cout << "Error al cargar sonido de murciélago. " << std::endl;
	}
	
	soundBank["warningSound"] = bach->loadSound("sounds/suspense.wav");
	if (!soundBank["warningSound"]){
		std::cout << "Error al cargar sonido de warning. " << std::endl;
	}
	
	soundBank["ghostSound"] = bach->loadSound("sounds/aw.wav");
	if (!soundBank["ghostSound"]){
		std::cout << "Error al cargar sonido de fantasma " << std::endl;
	}
	
	soundBank["heartSound"] = bach->loadSound("sounds/uip.wav");
	bach->soundSetVolume(soundBank["heartSound"], 0.4);
	if (!soundBank["heartSound"]){
		std::cout << "Error al cargar sonido de corazón " << std::endl;
	}
	
	soundBank["keySound"] = bach->loadSound("sounds/keyTake.wav");
	//bach->soundSetVolume(soundBank["keySound"], 0.4);
	if (!soundBank["keySound"]){
		std::cout << "Error al cargar sonido de llave " << std::endl;
	}
	
	
	musicBank["levelMusic"] = bach->loadMusic("music/Nightmare.mp3");
	if (!musicBank["levelMusic"]){
		std::cout << "Error al cargar música de nivel. " << std::endl;
	}
	
	hardcoreMode = false;
	highscore = 0;
	
	//Creo un jugador nuevo
	jugador = new player(playerSprites);
	
	levelNum = 0;
	maxLevel = 13;
	transTimer = 3.0;
	effectTimer = 0;
	mainMenu = new menu(leonardo); //creo el menu
	//Cargo el nivel
	currentLevel = new level("levels/level0.lvl", this);
	//currentScreen = stPressStart;
	currentScreen = stTransition0;
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
	
	if (vladSprites != NULL){
		leonardo->freeTexture(vladSprites);
		vladSprites = NULL;
	}
	
	if (effectSheet != NULL){
		leonardo->freeTexture(effectSheet);
		effectSheet = NULL;
	}
	
	if (coinSheet != NULL){
		leonardo->freeTexture(coinSheet);
		coinSheet = NULL;
	}
	
	if (doorSheet != NULL){
		leonardo->freeTexture(doorSheet);
		doorSheet = NULL;
	}
	
	if (titleScreen != NULL){
		leonardo->freeTexture(titleScreen);
		titleScreen = NULL;
	}
	
	if (invertedTitle != NULL){
		leonardo->freeTexture(invertedTitle);
		invertedTitle = NULL;
	}
	
	if (pressStart != NULL){
		leonardo->freeTexture(pressStart);
		pressStart = NULL;
	}
	
	if (gameOverSprite != NULL){
		leonardo->freeTexture(gameOverSprite);
		gameOverSprite = NULL;
	}
	
	if (YNSprite != NULL){
		leonardo->freeTexture(YNSprite);
		YNSprite = NULL;
	}
	
	if (continueSprite != NULL){
		leonardo->freeTexture(continueSprite);
		continueSprite = NULL;
	}
	
	for (map<string, Mix_Chunk*>::iterator itSnd = soundBank.begin(); itSnd!=soundBank.end(); ++itSnd){
		if (itSnd->second != NULL){
			bach->freeSound(itSnd->second);
			itSnd->second = NULL;
		}
	}
	
	for (map<string, Mix_Music*>::iterator itMus = musicBank.begin(); itMus!=musicBank.end(); ++itMus){
		if (itMus->second != NULL){
			bach->freeMusic(itMus->second);
			itMus->second = NULL;
		}
	}
	
	
	if (mainMenu != NULL){
		delete mainMenu;
		mainMenu = NULL;
	}
}

bool juego::isRunning(){
	return running;
}

void juego::step(control* c){
	
	if (c->esc){
		running = false; //For debugging purposes
		return;
		/*
		c->esc = false;
		if ((currentScreen == stPlaying || currentScreen == stPaused)){
			if (currentLevel != NULL){
				delete currentLevel;
				delete jugador;
				jugador = NULL;
				currentLevel = NULL;
			}
			currentScreen = stPressStart;
			mainMenu->start();
			levelNum = 0;
			jugador = new player(playerSprites);
			currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
		} else if (currentScreen == stPressStart || currentScreen == stMainMenu){
			running = false;
			return; 
		}
		*/
	}
	
	bach->update();
	
	if (currentScreen == stPressStart){
		effectTimer += 2;
		effectTimer = int(effectTimer)%360;
		if (c->evStart || c->evShoot){
			currentScreen = stMainMenu;
			mainMenu->start();
			c->evShoot = false;
			c->evStart = false;
		}
	}
	
	if (currentScreen == stMainMenu){
		effectTimer += 2;
		effectTimer = int(effectTimer)%360;
		
		mainMenu->step(c);
		if(mainMenu->goToNext() > 0){
			hardcoreMode = mainMenu->goToNext()-1;
			currentScreen = stTransition0;
			transTimer = 3.0;
			loadHighscore();
			currentLevel->setHighscore(highscore);
			currentLevel->setHardcore(hardcoreMode);
			std::cout << "highscore: " << highscore << std::endl;
		}

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
					currentLevel = new level(string("levels/level")+std::to_string(levelNum)+string(".lvl"), this);
					
					transTimer = 3.0;
					
				} else {
					
					delete currentLevel;
					if (jugador->getPoints() > highscore){
						highscore = jugador->getPoints();
						saveHighscore();
					}
					delete jugador;
					jugador = NULL;
					currentLevel = NULL;
					
					currentScreen = stTransition1;
					transTimer = 3.0;
				}
			}else {
				currentLevel->update(c);
				if (c->evStart){
					currentScreen = stPaused;
					c->evStart = false;
				}
			}
		}
	}
	
	if (currentScreen == stPaused){
		if (c->evStart){
			currentScreen = stPlaying;
			c->evStart = false;
		}
	}
	
	if (currentScreen == stTransition0){
		transTimer -= 0.03;
		if (transTimer <= 0){
			currentScreen = stPlaying;
			transTimer = 3;
		}
	}
	
	if (currentScreen == stTransition1){
		transTimer -= 0.01;
		if (transTimer <= 2.5){
			currentScreen = stGameOver;
			transTimer = 0;
			continueSelected = true;
		}
	}
	
	if (currentScreen == stGameOver){	
		if (hardcoreMode){
			transTimer += 0.02;
			if (transTimer > 1){
				transTimer = 1;
			}
		}
		
		if ((c->evShoot) || (c->evMelee) || (c->evStart) ){
			//currentScreen = stMainMenu;
			if (hardcoreMode){
				currentScreen = stPressStart;
				mainMenu->start();
				levelNum = 0;
				jugador = new player(playerSprites);
				currentLevel = new level(string("levels/level")+std::to_string(levelNum)+string(".lvl"), this);
			} else{
				if (continueSelected){
					jugador = new player(playerSprites);
					currentLevel = new level(string("levels/level")+std::to_string(levelNum)+string(".lvl"), this);
					currentScreen = stTransition0;
					transTimer = 3.0;
				} else{
					leonardo->setColor(120, 0, 0, 255);	
					leonardo->drawRect(0, 0, 1366, 768, 1);
					leonardo->setColor(0x17, 0x17, 0x17, 255);
					hardcoreMode = true;
				}
			}

			c->evShoot = false;
			c->evMelee = false;
			c->evStart = false;
		}
		
		if (c->evLeft || c->evRight){
			continueSelected = !continueSelected;
			c->evLeft = false;
			c->evRight = false;
		}
	}
	
}

void juego::draw(){
	
	if (currentScreen == stPaused){
		leonardo->setColor(255, 255, 255, 255);
		leonardo->drawRect(240, 48, 16, 32, 1);
		leonardo->drawRect(272, 48, 16, 32, 1);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
	}

	if (currentScreen == stGameOver){
		leonardo->setColor(120, 0, 0, 255);	
		leonardo->drawRect(0, 0, 1366, 768, 1);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
			
		if (hardcoreMode){
			gameOverSprite->setAlpha(255*transTimer);
			leonardo->draw(gameOverSprite, 0, 0, 0, 0, 220, 200);
		} else {
			leonardo->draw(continueSprite, 0, 0, 0, 0, 220, 150);
			leonardo->draw(YNSprite, 0, (!continueSelected)*128, 380, 128, 500, 400);
		}
	}
	
	if (currentScreen == stPressStart || currentScreen == stMainMenu){	
		leonardo->setColor(0, 0, 0, 255);
		leonardo->clear();
		invertedTitle->setAlpha(((sin(effectTimer/180*3.1415)+1)/2)*255);
		leonardo->draw(invertedTitle, 0, 0, 0, 0, 220, 283);
		leonardo->draw(titleScreen, 0, 0, 0, 0, 220, 64);
	}
		
	if (currentScreen == stPressStart){
		pressStart->setAlpha((1-((sin(effectTimer/180*3.1415)+1)/2))*255);
		leonardo->draw(pressStart, 0, 0, 0, 0, 530, 590);
	}
	
	if (currentScreen == stMainMenu){
		mainMenu->draw(leonardo);
	}
	
	if (currentScreen == stPlaying){
		leonardo->setColor(0x17, 0x17, 0x17, 255);
		if (currentLevel!=NULL){
			leonardo->clear();
			currentLevel->draw();
		}
	}
	
	if (currentScreen == stTransition1){
		leonardo->setBlendMode(1);
		leonardo->setColor(120, 0, 0, int(255*((3-transTimer)/3.0f)));
		
		leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
		
		leonardo->setBlendMode(0);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
	}
	
	if (currentScreen == stTransition0){
	
		if (transTimer > 2.0){
			leonardo->setBlendMode(1);
			leonardo->setColor(0x17, 0x17, 0x17, int(255*((1.5-transTimer)/1.5f)));
			
			leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
			
			leonardo->setBlendMode(0);
			leonardo->setColor(0x17, 0x17, 0x17, 255);
		}
		
		if ((currentLevel!=NULL) && (transTimer <= 1.5)){
			currentLevel->draw();
			
			leonardo->setBlendMode(1);
			leonardo->setColor(0x17, 0x17, 0x17, int(255*(transTimer/1.5f)));
			
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

LTexture* juego::getVladSprites(){
	return vladSprites;
}

LTexture* juego::getEffectSheet(){
	return effectSheet;
}

LTexture* juego::getCoinSheet(){
	return coinSheet;
}

LTexture* juego::getDoorSheet(){
	return doorSheet;
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

bool juego::getMode(){
	return hardcoreMode;
}

juego::gameState juego::getState(){
	return currentScreen;
}

jukebox* juego::getJukebox(){
	return bach;
}

map<string, Mix_Chunk* >* juego::getSoundBank(){
	return &soundBank;
}

map<string, Mix_Music* >* juego::getMusicBank(){
	return &musicBank;
}

//Termina "juego"

//empieza menú
menu::menu(painter* leonardo){
	menuSprites = leonardo->loadTexture("graphics/menu_sprites.png");
	if (menuSprites == NULL){
		std::cout << "Error al cargar los gráficos del menú. (1/3)" << std::endl;
		exit(1);
	}
	
	cursor = leonardo->loadTexture("graphics/marker.png");
	if (cursor == NULL){
		std::cout << "Error al cargar los gráficos del menú. (2/3)" << std::endl;
		exit(1);
	}
	
	optionSprites = leonardo->loadTexture("graphics/regular_hardcore.png");
	if (optionSprites == NULL){
		std::cout << "Error al cargar los gráficos del menú. (3/3)" << std::endl;
		exit(1);
	}
	
	alive = false;
	selected = 0;
	screen = 0;
	transitioning = false;
	timer = 60.0;
}

menu::~menu(){

	if (menuSprites != NULL){
		delete menuSprites;
		menuSprites = NULL;
	}
	
	if (cursor != NULL){
		delete cursor;
		cursor = NULL;
	}
	
	if (optionSprites != NULL){
		delete optionSprites;
		optionSprites = NULL;
	}
}

void menu::start(){
	alive = true;
	selected = 0;
	screen = 0;
	timer = 60;
}

void menu::end(){
	alive = false;
}
	
unsigned int menu::goToNext(){
	if ((!alive) && (screen == 1)){
		return selected+1;
	} else {
		return 0;
	}
}
	
void menu::step(control* ctrl){
	if (alive){
		timer -= 1;
		
		if (timer <= 0){
			timer = 60.0;
		}
		switch(screen){
		case 0:
			if (ctrl->evDown){
				selected+=1;
				if (selected > 2){
					selected = 0;
				}
				ctrl->evDown = false;
			}
			if (ctrl->evJump){
				selected-=1;
				if (selected > 2){
					selected = 2;
				}
				ctrl->evJump = false;
			}
			if (ctrl->evShoot || ctrl->evStart){
				if (selected == 0){
					screen = 1;
				} else if (selected == 1){
					
				} else if (selected == 2){
					exit(1);
				}
				ctrl->evShoot = false;
				ctrl->evStart = false;
			}
		break;
		case 1:
			if (ctrl->evDown || ctrl->evJump){
				selected = !selected;
				ctrl->evJump = false;
				ctrl->evDown = false;
			}
			if (ctrl->evShoot || ctrl->evStart){
				end();
				ctrl->evShoot = false;
				ctrl->evStart = false;
			}
			if (ctrl->evDash){
				selected = 0;
				screen = 0;
			}
		break;
		default:
		break;
		}
	}
}

void menu::draw(painter* pintor){
	if (alive){
		switch(screen){
		case 0:
			pintor->draw(menuSprites, 0, 0, 0, 0, 560, 500);
			
			switch(selected){
			case 0:
				pintor->draw(cursor, 0, 0, 0, 0, 550+sin(timer/30*3.1415)*4, 510);
				pintor->drawEx(cursor, 0, 0, 0, 0, 763-sin(timer/30*3.1415)*4, 510, 0, 0, 0, 1);
			break;
			case 1:
				pintor->draw(cursor, 0, 0, 0, 0, 500+sin(timer/30*3.1415)*4, 570);
				pintor->drawEx(cursor, 0, 0, 0, 0, 820-sin(timer/30*3.1415)*4, 570, 0, 0, 0, 1);
			break;
			case 2:
				pintor->draw(cursor, 0, 0, 0, 0, 550+sin(timer/30*3.1415)*4, 636);
				pintor->drawEx(cursor, 0, 0, 0, 0, 763-sin(timer/30*3.1415)*4, 636, 0, 0, 0, 1);
			break;
			}

		break;
		case 1:
			if (optionSprites != NULL){
				pintor->draw(optionSprites, 0, 0, 0, 0, 560, 500);
			}
			if (selected){
				pintor->draw(cursor, 0, 0, 0, 0, 500+sin(timer/30*3.1415)*4, 599);
				pintor->drawEx(cursor, 0, 0, 0, 0, 820-sin(timer/30*3.1415)*4, 599, 0, 0, 0, 1);
			} else {
				pintor->draw(cursor, 0, 0, 0, 0, 510+sin(timer/30*3.1415)*4, 540);
				pintor->drawEx(cursor, 0, 0, 0, 0, 810-sin(timer/30*3.1415)*4, 540, 0, 0, 0, 1);
			}

		default:
		break;
		}
	}
}

void juego::loadHighscore(){
	std::ifstream archivo;
	highscore = 0;
	long int regular_score = 0 ^ 0x54A9F23E;
	long int hardcore_score = 0 ^ 0x54A9F23E;
	
	archivo.open("save/highscores.sav");
	
	if (archivo.good())
		archivo >> regular_score;
	
	if (archivo.good())
		archivo >> hardcore_score;
		
	if (hardcoreMode){
		highscore = hardcore_score ^ 0x54A9F23E; //random magic number xor
	} else{
		highscore = regular_score ^ 0x54A9F23E; //random magic number xor
	}
	
	archivo.close();
}

long int juego::getHighscore(){
	return highscore;
}

void juego::setHighscore(long int hs){
	highscore = hs;
}

void juego::saveHighscore(){
		
	// Tantos años... Tantos años Y TODAVÍA NO SÉ MANEJAR ARCHIVOS!!!
	// AAAAAAAAAAAAAHHHHHH!!!
	// POR QUÉ NO PUEDO ABRIR IN/OUT, GENERANDO UN ARCHIVO NUEVO SI NO EXISTIERA?? 
	
	std::fstream* archivo = new std::fstream("save/highscores.sav", std::fstream::in);
	long int regular_score = 0 ^ 0x54A9F23E;
	long int hardcore_score = 0 ^ 0x54A9F23E;

	archivo->seekg(0);
	
	if (archivo->good())
		*archivo >> regular_score;
	
	if (archivo->good())
		*archivo >> hardcore_score;
		
	archivo->close();
	if (archivo != NULL){
		delete archivo;
		archivo = NULL;
	}
	
	//si la carpeta "save" no existe, se pudre todo
	archivo = new std::fstream("save/highscores.sav", std::fstream::out);
	archivo->seekp(0);
	
	if (hardcoreMode){
		*archivo << regular_score;
		*archivo << " ";
		hardcore_score = highscore ^ 0x54A9F23E; //random magic number xor
		*archivo << hardcore_score;
	} else{
		regular_score = highscore ^ 0x54A9F23E; //random magic number xor
		*archivo << regular_score;
		*archivo << " ";
		*archivo << hardcore_score;
	}
	archivo->close();
	delete archivo;
}
