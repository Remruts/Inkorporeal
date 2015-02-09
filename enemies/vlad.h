#ifndef _VAMPIRE_
#define _VAMPIRE_

#include "enemy.h"
#include "../particles.h"
#include <deque>

class vlad : public enemy{
public:
	vlad(LTexture* sprt, int x, int y);
	~vlad();	
	
	virtual void step(level*);
	virtual void draw(painter*);
	
private:

	enum VState {stRun, stIdle, stDash, stJump, stPrepare, stUppercut, stQuake};
	
	animation* faceSprite;
	animation* runSpriteTop;
	animation* runSpriteBot;
	animation* swordSprite;
	
	double timer;
	double timeFactor;
	
	bool onGround;
	//bool shooting;
	
	int playerX, playerY;
	double normalized; //meh
	VState state;
	VState nextState;
};

class upSmoke: public emitter{
public:
	upSmoke(LTexture* sprt, int x, int y);
	~upSmoke();
	
	void emit();
	void step(level*);
private:
	
};

class circleEmitter : public emitter{
public:
	circleEmitter(LTexture* sprt, int x, int y, bool izq);
	~circleEmitter();
	
	void emit();
	void step(level*);
private:
	bool izq;
};

/*
class thrownbone : public enemyBullet{
public:
	thrownbone(LTexture*, int x, int y, bool right);
	~thrownbone();
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* boneSprite;
	bool side;
	deque<int> prevX;
	deque<int> prevY;
};

*/
#endif
