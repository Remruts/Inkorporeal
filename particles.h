
#ifndef _PARTICLE_SYSTEMS_
#define _PARTICLE_SYSTEMS_

#include "painter.h"
#include "level.h"
#include "animation.h"
#include "LTexture.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

class particle{
public:
	particle(LTexture* spritesheet, int x, int y, int life);
	~particle();
	
	void setSprite(int);
	void setScale(double);
	double getScale();
	
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
	int x, y;
	double spdX, spdY;
	double scale;
	int sprite;
	
	bool gravity;
	int gravity_dir;
	bool friction;
	
	bool bounce;
	bool blend;
	
	int alpha;
	int angle;
	
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
	colourExplosion(LTexture* sprt, int x, int y);
	~colourExplosion();
	
	void emit();
	void step(level*);
	
private:
	
};

class coinSparkle : public emitter{
public:
	coinSparkle(LTexture* sprt, int x, int y);
	~coinSparkle();
	
	void emit();
	void step(level*);
private:
	
};

#endif
