#ifndef _PLAYER_
#define _PLAYER_

#include <iostream>
#include <SDL2/SDL.h>

#include "LTexture.h"
#include "painter.h"
#include "animation.h"
#include "level.h"

#include <cmath>

class level; //forward declaration

class player {
public:
	enum stateEnum {stRun, stIdle, stHurt, stJump, stDash}; //Estados
	
	player(LTexture* sprt);
	~player();
	
	int getLives();
	void addPoints(int);
	int getPoints();
	
	void setPos(int x, int y);
	void getPos(int &x, int &y) const;
	void setSpeed(double x, double y);
	void getSpeed(double &x, double &y) const;
	
	void shoot();
	bool isShooting();
	bool isDashing();
	
	void getHurt();
	
	void setFlip(bool);
	
	stateEnum getState() const;
	void setState(stateEnum);
	
	void setColBox(int x, int y, int w, int h);
	const SDL_Rect& getColBox() const;
	
	void draw(painter*);
	void step(level*);
	
	void reset();
	
private:
	int lives; // vidas
	int points; // puntos
	int x, y;
	double spdX, spdY;
	double maxSpeedX, maxSpeedY;
	bool facingRight; //si veo hacia la derecha, para flip de textura
	bool shooting;
	bool canDash;
	
	bool hurt; // herido?
	float hurtCounter; // cuanto tiempo para dejar de estar herido

	float timeToBlink; //Si es hora de pestañear...
	
	int gun; //Qué arma tiene?
	float gunCounter; //contador para disparos
	
	//animaciones
	animation* blinkSprite;
	animation* runSpriteTop;
	animation* runSpriteBot;
	
	SDL_Rect collisionBox;
	LTexture* spritesheet;
	
	stateEnum state; //estado
};

class bullet{
public:
	bullet(LTexture* sprt,int x, int y, int spd);
	~bullet();
	
	void step(level*);
	void draw(painter*);
	
	SDL_Rect* getColBox();
	
	bool isAlive();
	void die();
	
private:
	int x, y, spd;
	bool alive;
	SDL_Rect collisionBox;
	LTexture* spritesheet;
};

#endif
