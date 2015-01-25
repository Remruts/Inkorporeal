//header del juego
#ifndef JUEGO_INCLUDED
#define JUEGO_INCLUDED

#include "control.h"
#include "painter.h"
#include "level.h"
#include "LTexture.h"
#include "player.h"
#include <map>
#include <string>
#include <utility>

using namespace std;

//fd
class player;
class level;

class juego{
public:

	enum gameState {stMainMenu, stPlaying, stTransition0, stTransition1, stGameOver};
	
	juego(painter*);
	~juego();
	
	bool isRunning();
	void step(control*);
	void draw();
	
	map<string, pair<int, int> >* getMap();
	SDL_Surface* getPropsheet();
	SDL_Surface* getTilesheet();
	LTexture* getPlayerSprites();
	LTexture* getEnemySprites();
	int getLevelNum();
	painter* getPainter();
	player* getPlayer();
	
	gameState getState();
	
private:
	bool running;
	painter* leonardo;
	
	gameState currentScreen;
	
	SDL_Surface* tiles; //puntero a Surface de tiles
	SDL_Surface* props; //puntero a Surface de props
	map<string, pair<int, int> > posEnSheet; // mapa de posiciones para props en spritesheet
	
	LTexture* playerSprites; //puntero a Texture de spritesheet de MC
	LTexture* enemySprites; //puntero a Texture de spritesheet de enemigos
	
	level* currentLevel; //pointer al nivel actual
	player* jugador;
	
	int levelNum; //current level number
	int maxLevel;
	
	double transTimer; //transition timer
	
};

#endif
