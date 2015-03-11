#ifndef _PRIEST_
#define _PRIEST_

#include "enemy.h"
#include "../limbs.h"
#include <cmath>

class demon;
class bossState;

class priest : public enemy{
public:
	priest(LTexture* sprt, int x, int y);
	~priest();
	
	double getTimer();
	void setTimer(double);
	
	void setNextState(bossState*);
	void setDevilAnim(const string&);
	
	LTexture* getSpriteSheet();
	
	virtual void step(level*);
	virtual void draw(painter*);
	
private:
	
	void drawHalo(painter* p, LTexture* cube, int x, int y, double direction, int size, int counter, int colour);
	
	double timer;
	double haloAngle;
	int haloColor;
	
	animation* movingAnim;
	demon* shadowDevil;
	bossState* state;
	bossState* nextState;
};

//BALAS
class sineBullet : public enemyBullet{
public:
	sineBullet(LTexture* sprt, int X, int Y, bool side);
	~sineBullet();
	
	void step(level*);
	void draw(painter*);
private:
	double timer;
};

class regularBullet : public enemyBullet{
public:
	regularBullet(LTexture* sprt, int X, int Y, double spdX, double spdY);
	virtual ~regularBullet();
	
	void setColor(int R, int G, int B);
	void setSize(double s);
	void setVisible(int v);
	
	virtual void step(level*);
	virtual void draw(painter*);
protected:
	unsigned int r, g, b;
	double size;
};

class spinningBullet : public regularBullet{
public:
	spinningBullet(LTexture* sprt, int X, int Y, double dir, double spd);
	~spinningBullet();
	
	void step(level*);
	void draw(painter*);
private:
	double direction, speed;
};

class bossMine : public enemyBullet{
public:
	bossMine(LTexture* sprt, int X, int Y);
	~bossMine();
	
	void step(level*);
	void draw(painter*);
private:
	animation* mineAnim;
	bool exploded;
};


//Estados...
class bossState{
public:
	virtual ~bossState(){}
	virtual void step(level* lvl, priest* p){}
	virtual void enter(level* lvl, priest* p){}
	virtual void exit(level* lvl, priest* p){}
};

class stIdle : public bossState{
public:
	stIdle();
	~stIdle();
	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);
};

class stTeleport : public bossState{
public:
	stTeleport();
	~stTeleport();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);

private:
	int nextX, nextY;
	static unsigned int teleportNum;
};

class stDemonCrush : public bossState{
public:
	stDemonCrush();
	~stDemonCrush();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);

};

class stDemonShoot : public bossState{
public:
	stDemonShoot();
	~stDemonShoot();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);

};

class stDemonCharge : public bossState{
public:
	stDemonCharge();
	~stDemonCharge();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);
private:
	bool charged;

};

class stPillars : public bossState{
public:
	stPillars();
	~stPillars();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);
private:
	bool fire;
	int pattern;
};

class stThrowMine : public bossState{
public:
	stThrowMine();
	~stThrowMine();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);
private:
	int nextX, nextY;
};

class stMultishot : public bossState{
public:
	stMultishot();
	~stMultishot();

	virtual void step(level* lvl, priest* p);
	virtual void enter(level* lvl, priest* p);
	virtual void exit(level* lvl, priest* p);
};
//-----------------------------------------------


//demonio
class demon {
public:
	demon(LTexture* sprt, int X, int Y);
	~demon();
	
	void setAnimation(const string&);
	
	void step(level*);
	void draw(painter*);
private:
	LTexture* spritesheet;
	LTexture* armsTexture;
	int x, y;
	int face;
	
	double timer;
	
	//esqueletos para brazos
	limbSkeleton *brazoIzq, *brazoDer; 
	
	//Animaciones de Idle
	limbAnim *idleIzqAnim, *idleDerAnim;
	//Animaciones de brazos cayendo
	limbAnim *fallingIzqAnim, *fallingDerAnim;
	//Animaciones de brazos cargando
	limbAnim *chargingIzqAnim, *chargingDerAnim;
	//Animaciones de release
	limbAnim *releaseIzqAnim, *releaseDerAnim;
	//Animaciones de disparo
	limbAnim *shootIzqAnim, *shootDerAnim;
	
	//animaciones actuales para brazo izquierdo y derecho respectivamente
	limbAnim *currentIzqAnim, *currentDerAnim; 	
	// Nombre de la animación actual
	// Esto es un extra sin relevancia. Puede ser de utilidad, pero no me sirve ahora
	string currentAnim; 

};

#endif
