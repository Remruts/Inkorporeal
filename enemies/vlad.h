#ifndef _VAMPIRE_
#define _VAMPIRE_

#include "enemy.h"
#include <deque>

class vlad : public enemy{
public:
	vlad(LTexture* sprt, int x, int y);
	~vlad();	
	
	virtual void step(level*);
	virtual void draw(painter*);
	
private:

	enum VState {stRun, stIdle, stDash, stJump, stPrepare};
	
	animation* faceSprite;
	animation* runSpriteTop;
	animation* runSpriteBot;
	animation* swordSprite;
	
	double timer;
	bool onGround;
	bool shooting;
	
	int playerX, playerY;
	double normalized; //meh
	VState state;
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
