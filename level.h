//header del nivel
#ifndef _LEVEL_
#define _LEVEL_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "juego.h"
#include "painter.h"
#include "jukebox.h"
#include "LTexture.h"
#include "player.h"
#include "control.h"
#include "particles.h"
#include "pickups.h"
#include "door.h"
#include "points.h"

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
class emitter;
class pickup;
class door;
class key;

using std::vector;
using std::map;
using std::pair;

class level{
public:
	
	enum lvlState {stWin, stLose, stPlaying};
	
	level(const string &, juego* game);
	~level();
	
	void draw();
	void update(control*);
	
	//Función para que otros llamen a screen shake
	void shake(double intensity, int time);
	
	//determina si un punto está sobre el jugador
	bool isPlayer(int x, int y) const;
	void getPlayerPos(int &x, int &y);
	//determina si un punto está sobre algo sólido
	bool isSolid(int x, int y) const;
	// Determina si hay algo sólido en una línea horizontal del punto x1 al x2
	// Si hubiese, devuelve la posición de contacto (en x), sino -1
	int hRaySolid(int x1, int x2, int y) const;
	// Determina si hay algo sólido en una línea vertical del punto y1 al y2
	// Si hubiese, devuelve la posición de contacto (en y), sino -1
	int vRaySolid(int y1, int y2, int x) const;
	
	// Estas funciones deberían haber tenido un parámetro "tipo" y que acá se creen, 
	// en vez de que se les pase un puntero. Así el nivel maneja los recursos y no las otras cosas... 
	// Soy un idiota.
	void addBullet(bullet*); 			//agrega una bala del jugador a la escena
	void addEnemy(enemy*);				//agrega un enemigo a la escena
	void addEnemyBullet(enemyBullet*); 	//agrega una bala de enemigo a la escena
	void addEmitter(emitter*); 			//agrega un emisor de partículas a la escena
	void addPickup(pickup*);			//agrega un pickup a la escena
	void addKey(int x, int y);			//agrega una llave en la posición correspondiente
	
	// No lo pienso arreglar.
	// En vez de eso, vamos a crear una función que me devuelva el spritesheet de efectos.
	LTexture* getEffectSheet();
	
	void addPoints(int points, int x, int y);
	void addLife();
	
	bool isFinished();
	lvlState getState();
	
	void setHighscore(long int);
	void setHardcore(bool);
	
	void playSound(const string &);

private:
	int load(std::istream& is, map<string, pair<int, int> >& posEnSheet);
	void ignore(std::istream& is); //función auxiliar para ignorar espacios, saltos de línea y comentarios
	
	//rutina de actualización del jugador
	void updatePlayer(control*);
	//rutina de actualización de las balas
	void updateBullets();
	//rutina de actualización de enemigos
	void updateEnemies();
	//agrega los enemigos en la lista de enemigos a agregar
	void addEnemies();
	//rutina de actualización de partículas
	void updateEmitters();
	//rutina de actualización de pickups
	void updatePickups();
	//rutina de actualización de puerta
	void updateDoor();
	//rutina de actualización de la llave
	void updateKey();
	//rutina de actualización de los puntos que se dibujan
	void updatePoints();
	
	void checkBulletCollisions();
	void checkPlayerEnemyCollisions();
	void checkPlayerPickup();
	
	bool checkCollision(const SDL_Rect* A, const SDL_Rect* B);
	
	void drawPoints(); //dibuja los puntos

	
	lvlState currentState; 	//indica si el nivel se está jugando, se ganó o se perdió
	bool finished; 			//indica si terminó el nivel
	
	jukebox* bach;
	map<string, Mix_Chunk*>* soundBank; //sonidos
	map<string, Mix_Music*>* musicBank; //música
	
	painter* leonardo;
		
	LTexture *background; //esto es lo que se muestra
	SDL_Surface* backSurface; //buffer
	SDL_Rect rect, pos, rugrect; //Creo rectángulos para blitear cada tile en el background
	
	std::map<string, std::pair<int, int> >* posEnSheet;
	
	//tile/spritesheets
	SDL_Surface *propSheet;
	SDL_Surface *tileSheet;
	LTexture *enemySprites;
	LTexture *vladSprites; //Sprites de Vlad, duh?
	LTexture *priestSprites;
	LTexture *effectSheet;
	LTexture *coinSheet;
	LTexture *doorSheet;
	
		
	bool solid[420]; // mapa de 30x14 (=420). Determina si es sólido (true) o no (false)
	player* jugador; //puntero al jugador
	vector<bullet*> bulletList; //vector de punteros a balas de jugador
	vector<enemy*> enemyList; //vector a puntero de enemigos
	vector<enemy*> enemiesToAdd; //vector a puntero de enemigos a agregar
	vector<enemyBullet*> enemyBulletList; //vector a puntero de balas de enemigos
	vector<emitter*> emitterList; //vector a puntero de emisores de partículas
	vector<pickup*> pickupList; //vector a puntero de pickups
	vector<movingPoints*> pointsList; //vector a puntero de puntos que se ven
	
	door* puerta;
	key* llave;
	
	int shakeTimer;
	
	LTexture* testText; //Texto de prueba
	LTexture* pointsText; //texto de puntos
	LTexture* pointsText2; //texto de puntos2
	
	unsigned int levelnum;
	LTexture* levelNumText;
	LTexture* bossText;
	long int highscore;
	
	unsigned int lvlType;
	bool hardcoreMode;
};

#endif
