
#ifndef _PARTICLE_SYSTEMS_
#define _PARTICLE_SYSTEMS_

#include "painter.h"
#include "level.h"
#include "animation.h"
#include "LTexture.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>

using std::vector;

class particle{
public:
	particle(LTexture* spritesheet, int x, int y, int life);
	~particle();
	
	void setSprite(int);
	
	void setScale(double);
	void setScaleX(double);
	void setScaleY(double);
	double getScaleX();
	double getScaleY();
	
	void setSpeed(double spdX, double spdY);
	double getSpeedX();
	double getSpeedY();
	void setFriction(double);
	void setBounce(bool);
	
	bool isAlive();
	int getLife();
	int getMaxLife();
	
	void setAlpha(int);
	void setAngle(int);
	void setSpriteSize(int);
	
	void setPermanence(bool);
	
	void setGravity(bool);
	void setGravityDir(int);
	
	void setBlend(bool); //blend or add?
	void setColor(unsigned int, unsigned int, unsigned int);
	
	void step(level*);
	void draw(painter*, LTexture*);
	
private:
	double x, y;
	double spdX, spdY;
	double scaleX, scaleY;
	int sprite;
	
	bool gravity;
	int gravity_dir;
	bool friction;
	
	bool bounce;
	bool blend;
	
	int alpha;
	int angle, invAngle;
	
	unsigned int r, g, b;
	
	int life;
	int maxLife;
	bool permanent;
	bool alive;
	
	int spriteSize;
	
	LTexture* spritesheet;
};

class emitter{
public:
	emitter(LTexture* spritesheet, int life, int x, int y);
	virtual ~emitter();
	
	virtual void emit();
	virtual void step(level*);
	void setPos(int X, int Y);
	void draw(painter*, LTexture*);
	
	bool isAlive();
protected:
	vector <particle*> particles;
	int x, y;
	LTexture* spritesheet;
	
	int life; //-1 == forever
	bool alive;
	int timer;
	int maxTimer;
	int rate;
	int radius;
};

class colourExplosion : public emitter{
public:
	colourExplosion(LTexture* sprt, int x, int y, painter* p);
	~colourExplosion();
	
	void emit();
	void step(level*);
	
private:
	unsigned int r, g, b;
};

class coinSparkle : public emitter{
public:
	coinSparkle(LTexture* sprt, int x, int y);
	~coinSparkle();
	
	void emit();
	void step(level*);
private:
	
};

class dashEmitter : public emitter{
public:
	dashEmitter(LTexture* sprt, int X, int Y);
	~dashEmitter();
	
	void step(level* lvl);
	void emit();
private:
	
};

class afterShot : public emitter{
public:
	afterShot(LTexture* sprt, int X, int Y);
	~afterShot();
	
	void step(level* lvl);
	void emit();
private:
	
};

class hurtEffect : public emitter{
public:
	hurtEffect(LTexture* sprt, int X, int Y);
	~hurtEffect();
	
	void step(level* lvl);
	void emit();
private:
	
};

class starEffect : public emitter{
public:
	starEffect(LTexture* sprt, int X, int Y);
	~starEffect();
	
	void step(level* lvl);
	void emit();
private:
	
};

class plasmaEffect : public emitter{
public:
	plasmaEffect(LTexture* sprt, int X, int Y, int R, int G, int B);
	~plasmaEffect();
	
	void step(level* lvl);
	void emit();
private:
	int r, g, b;
};

class waveEffect : public emitter{
public:
	waveEffect(LTexture* sprt, int X, int Y, int R, int G, int B, double tam, int life);
	waveEffect(LTexture* sprt, int X, int Y, int R, int G, int B, double tam, int life, bool Invert);
	~waveEffect();
	
	void step(level* lvl);
	void emit();
private:
	int r, g, b;
	double size;
	int partLife;
	bool invert;
};

//las explosiones más simples y truchas del mundo
class explosionEffect : public emitter{
public:
	explosionEffect(LTexture* sprt, int X, int Y);
	~explosionEffect();
	
	void step(level* lvl);
	void emit();	
};

#endif
