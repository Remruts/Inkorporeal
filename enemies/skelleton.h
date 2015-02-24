#ifndef _OLD_BONES_
#define _OLD_BONES_

#include "enemy.h"
#include <deque>

using std::deque;

class skelleton : public enemy{
public:
	skelleton(LTexture* sprt, int x, int y);
	~skelleton();	
	
	virtual void step(level*);
	virtual void draw(painter*);
private:
	animation* movingAnim;
	animation* shootingAnim;
	double timer;
	bool onGround;
	bool shooting;
};

class thrownbone : public enemyBullet{
public:
	thrownbone(LTexture*, int x, int y, bool right);
	~thrownbone();
	
	virtual void step(level*);
	virtual void draw(painter*);
	void die();
private:
	animation* boneSprite;
	bool side;
	deque<int> prevX;
	deque<int> prevY;
};

#endif
