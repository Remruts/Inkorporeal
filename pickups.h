#ifndef _PICKUPS_
#define _PICKUPS_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "LTexture.h"
#include "painter.h"
#include "animation.h"
#include "level.h"
#include "particles.h"
#include "math.h"

// DEBERÍA HABER HECHO UNA CLASE "OBJETO/ENTIDAD" Y HEREDAR TODAS LAS COSAS!
// PERO NOOOOO!
// teodioyodelpasadoportuvagancia
// teodioyodelfuturoporquenovasaarreglarnada

class pickup{
public:
	pickup(LTexture* sprt, int x, int y, int lives);
	virtual ~pickup();
	
	virtual void step(level*);
	virtual void draw(painter*);
	
	void getPos(int &x, int &y) const;
	SDL_Rect* getColBox();
	bool isPickable();
	
	bool isAlive();
	virtual void onCollisionWithPlayer(level*);
	virtual void die();
	
protected:
	int x, y;
	double spdX, spdY;
	double accelX, accelY;
	bool onGround;
	bool pickable;
	
	int visible;
	int alpha;
	int angle;
	
	int lives;
	int maxLives;
	bool alive;
	
	SDL_Rect colBox;
	LTexture* spritesheet;
	animation* currentAnim;
	
};

class coin : public pickup{
public:
	coin(LTexture* sprt, int X, int Y);
	~coin();
	
	void step(level*);
	void draw(painter*);
	
	void onCollisionWithPlayer(level*);
	
private:
	animation* monedita;	
};

class heart : public pickup{
public:
	heart(LTexture* sprt, int X, int Y);
	~heart();
	
	void step(level*);
	void draw(painter*);
	
	void onCollisionWithPlayer(level*);
private:
	animation* corazoncito;
	int prevY;
};

#endif
