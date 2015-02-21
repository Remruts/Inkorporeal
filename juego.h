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
#include <fstream>

using namespace std;

//fd
class player;
class level;
class menu;

class juego{
public:

	enum gameState {stPressStart, stMainMenu, stPlaying, stTransition0, stTransition1, stGameOver, stPaused};
	
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
	LTexture* getVladSprites();
	LTexture* getEffectSheet();
	LTexture* getCoinSheet();
	LTexture* getDoorSheet();
	long int getHighscore();
	
	void setHighscore(long int hs); //no lo creo necesario, pero bue
	
	int getLevelNum();
	painter* getPainter();
	player* getPlayer();
	bool getMode();
	
	gameState getState();
	
private:
	
	void loadHighscore();
	void saveHighscore();
	
	bool running;
	painter* leonardo;
	
	gameState currentScreen;
	
	SDL_Surface* tiles; //puntero a Surface de tiles
	SDL_Surface* props; //puntero a Surface de props
	
	map<string, pair<int, int> > posEnSheet; // mapa de posiciones para props en spritesheet
	
	LTexture* playerSprites; //puntero a Texture de spritesheet de MC
	LTexture* enemySprites; //puntero a Texture de spritesheet de enemigos
	LTexture* vladSprites; //puntero a Texture de spritesheet de vlad
	LTexture* effectSheet; //puntero a texture de effectos
	LTexture* coinSheet; //puntero a texture de moneditas
	LTexture* doorSheet; //puntero a texture de puerta
	
	LTexture* titleScreen; //puntero a texture de título
	LTexture* invertedTitle; //puntero a texture de título invertido. Podría haberlo dibujado invertido y con otro color.
	LTexture* pressStart;	//puntero a texture de texto "press start"
	
	LTexture* gameOverSprite;  	//puntero a texture de texto "GAME OVER"
	LTexture* YNSprite;
	LTexture* continueSprite;
	bool continueSelected; 		//indica si se selecciona "continue"
	
	level* currentLevel; //pointer al nivel actual
	player* jugador;
	
	unsigned int levelNum; //current level number
	unsigned int maxLevel;
	bool hardcoreMode; //Si estamos en modo hardcore
	long int highscore;
	
	double transTimer; //transition timer
	double effectTimer;
	
	menu* mainMenu;
	
};

class menu{
public:
	menu(painter*);
	~menu();
	
	void start(); 	// Función para comenzar
	void end();		// Función para terminar
	
	unsigned int goToNext(); //indica si debería irse a la pantalla de juego y en qué modo
	
	void step(control*);
	void draw(painter*);
	
private:
	bool alive;
	unsigned char selected; 	// cuál opción es seleccionada?
	unsigned char screen; 		// 0=start, 1=play, 2=options
	bool transitioning; 		// en transición?
	double timer;				// un timer para medir tiempo
	
	//Texturas
	LTexture* menuSprites;
	LTexture* cursor;
	LTexture* optionSprites;
};

#endif
