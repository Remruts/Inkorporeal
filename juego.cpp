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
	posEnSheet["candelabro0"] = make_pair(0, 6);
	posEnSheet["cadena0"] = make_pair(1, 6);
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
	
	//Creo un jugador nuevo
	jugador = new player(playerSprites);
	
	levelNum = 0;
	maxLevel = 10;
	transTimer = 2.0;
	effectTimer = 0;
	mainMenu = new menu(leonardo); //creo el menu
	//Cargo el nivel
	currentLevel = new level("levels/level0.lvl", this);
	//currentScreen = stPressStart;
	arcadeMode = false;
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
		running = false;
		return;
	}
	
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
			arcadeMode = mainMenu->goToNext()-1;
			currentScreen = stTransition0;
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
					currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
					
					transTimer = 3.0;
					
				} else {
					delete currentLevel;
					delete jugador;
					jugador = NULL;
					currentLevel = NULL;
					currentScreen = stTransition1;
					transTimer = 3.0;
				}
			}else {
				currentLevel->update(c);
			}
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
			transTimer = 3;
			continueSelected = true;
		}
	}
	
	if (currentScreen == stGameOver){
		if ((c->evShoot) || (c->evMelee) || (c->evStart) ){
			//currentScreen = stMainMenu;
			if (arcadeMode){
				currentScreen = stPressStart;
				mainMenu->start();
				levelNum = 0;
				jugador = new player(playerSprites);
				currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
			} else{
				if (continueSelected){
					jugador = new player(playerSprites);
					currentLevel = new level(string("levels/level")+to_string(levelNum)+string(".lvl"), this);
					currentScreen = stTransition0;
				} else{
					leonardo->setColor(120, 0, 0, 255);	
					leonardo->drawRect(0, 0, 1366, 768, 1);
					leonardo->setColor(0x17, 0x17, 0x17, 255);
					arcadeMode = true;
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

	if (currentScreen == stGameOver){
		if (arcadeMode){
			gameOverSprite->setAlpha(10);
			leonardo->draw(gameOverSprite, 0, 0, 0, 0, 220, 200);
		} else {
			leonardo->setColor(120, 0, 0, 255);	
			leonardo->drawRect(0, 0, 1366, 768, 1);
			leonardo->setColor(0x17, 0x17, 0x17, 255);
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
		
	if ((transTimer > 2.0) && (currentScreen == stTransition0)){
		leonardo->setBlendMode(1);
		leonardo->setColor(0x17, 0x17, 0x17, int(255*((1.5-transTimer)/1.5f)));
		
		leonardo->drawRect(0, 0, 1366, 768, 1); //debería hacer algo con screen_width/screen_height, pero bue
		
		leonardo->setBlendMode(0);
		leonardo->setColor(0x17, 0x17, 0x17, 255);
	}
	
	if (currentScreen == stTransition0){
		
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

juego::gameState juego::getState(){
	return currentScreen;
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
	
	optionSprites = leonardo->loadTexture("graphics/regular_arcade.png");
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
				pintor->draw(cursor, 0, 0, 0, 0, 520+sin(timer/30*3.1415)*4, 605);
				pintor->drawEx(cursor, 0, 0, 0, 0, 800-sin(timer/30*3.1415)*4, 605, 0, 0, 0, 1);
			} else {
				pintor->draw(cursor, 0, 0, 0, 0, 510+sin(timer/30*3.1415)*4, 540);
				pintor->drawEx(cursor, 0, 0, 0, 0, 810-sin(timer/30*3.1415)*4, 540, 0, 0, 0, 1);
			}

		default:
		break;
		}
	}
}
