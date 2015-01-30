//header del nivel
#ifndef _LEVEL_
#define _LEVEL_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "juego.h"
#include "painter.h"
#include "LTexture.h"
#include "player.h"
#include "control.h"
#include "particles.h"
#include "pickups.h"

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <stdlib.h> //para random
#include <time.h> 	//idem

//forward declarations ftw
class player;
class bullet;
class juego;
class enemy;
class enemyBullet;
class ghost;
class skelleton;
class emitter;
class pickup;

class level{
public:
	
	enum lvlState {stWin, stLose, stPlaying};
	
	level(const string &, juego* game);
	~level();
	
	void draw();
	void update(control*);
	
	//determina si un punto está sobre el jugador
	bool isPlayer(int x, int y) const;
	//determina si un punto está sobre algo sólido
	bool isSolid(int x, int y) const;
	// Determina si hay algo sólido en una línea horizontal del punto x1 al x2
	// Si hubiese, devuelve la posición de contacto (en x), sino -1
	int hRaySolid(int x1, int x2, int y) const;
	// Determina si hay algo sólido en una línea vertical del punto y1 al y2
	// Si hubiese, devuelve la posición de contacto (en y), sino -1
	int vRaySolid(int y1, int y2, int x) const;
	
	void addBullet(bullet*); 	//agrega una bala del jugador a la escena
	void addEnemyBullet(enemyBullet*); //agrega una bala de enemigo a la escena
	void addEmitter(emitter*); //agrega un emisor de partículas a la escena
	void addPickup(pickup*);	//agrega un pickup a la escena
	
	void addPoints(int);
	
	bool isFinished();
	lvlState getState();

private:
	int load(std::istream& is, map<string, pair<int, int> >& posEnSheet);
	void ignore(std::istream& is); //función auxiliar para ignorar espacios, saltos de línea y comentarios
	
	//rutina de actualización del jugador
	void updatePlayer(control*);
	//rutina de actualización de las balas
	void updateBullets();
	//rutina de actualización de enemigos
	void updateEnemies();
	//rutina de actualización de partículas
	void updateEmitters();
	//rutina de actualización de pickups
	void updatePickups();
	
	void checkBulletCollisions();
	void checkPlayerEnemyCollisions();
	void checkPlayerPickup();
	
	bool checkCollision(const SDL_Rect* A, const SDL_Rect* B);

	
	lvlState currentState; 	//indica si el nivel se está jugando, se ganó o se perdió
	bool finished; 			//indica si terminó el nivel
	
	painter* leonardo;
	
	LTexture *background; //esto es lo que se muestra
	SDL_Surface* backSurface; //buffer
	SDL_Rect rect, pos, rugrect; //Creo rectángulos para blitear cada tile en el background
	
	std::map<string, std::pair<int, int> >* posEnSheet;
	
	//tile/spritesheets
	SDL_Surface *propSheet;
	SDL_Surface *tileSheet;
	LTexture *enemySprites;
	LTexture *effectSheet;
	LTexture *coinSheet;
	
	bool solid[420]; // mapa de 30x14 (=420). Determina si es sólido (true) o no (false)
	player* jugador; //puntero al jugador
	vector<bullet*> bulletList; //vector de punteros a balas de jugador
	vector<enemy*> enemyList; //vector a puntero de enemigos
	vector<enemyBullet*> enemyBulletList; //vector a puntero de balas de enemigos
	vector<emitter*> emitterList; //vector a puntero de emisores de partículas
	vector<pickup*> pickupList; //vector a puntero de pickups
	
	LTexture* testText; //Texto de prueba
	unsigned int levelnum;
	LTexture* levelNumText;
	
	animation* monedita;
	
};

#endif
