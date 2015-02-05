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
	LTexture* getEffectSheet();
	LTexture* getCoinSheet();
	LTexture* getDoorSheet();
	
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
	LTexture* effectSheet; //puntero a texture de effectos
	LTexture* coinSheet; //puntero a texture de moneditas
	LTexture* doorSheet; //puntero a texture de puerta
	
	LTexture* titleScreen; //puntero a texture de título
	LTexture* invertedTitle; //puntero a texture de título invertido. Podría haberlo dibujado invertido y con otro color.
	LTexture* pressStart;	//puntero a texture de texto "press start"
	
	level* currentLevel; //pointer al nivel actual
	player* jugador;
	
	int levelNum; //current level number
	int maxLevel;
	
	double transTimer; //transition timer
	double effectTimer;
	
};

#endif
