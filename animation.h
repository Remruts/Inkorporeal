#ifndef _ANIMATION_
#define _ANIMATION_

#include "painter.h"
#include "LTexture.h"
#include <iostream> //debug

class animation{
public:
	animation(int frmnum, float spd, bool lp, LTexture* sprsh, unsigned int frms[], int w);
	~animation();
	
	void setCurrentFrame(int frm);
	const int getCurrentFrame() const;
	
	const int& getFrameNum() const;
	
	void setSpeed(float);
	float getSpeed();
	
	const bool isFinished() const;
	
	void step();
	void draw(painter*, int x, int y);
	
	void setFrameHeight(int);
	void setFrameWidth(int);
	const int& getFrameWidth();
	const int& getFrameHeight();

	void setFlip(bool);
	void setStretch(bool);
	void setAngle(int);
	
private:
	float speed;
	float curFrame;
	int frameNum;
	bool loop;
	unsigned int* frames;
	int width, height;
	int angle;
	bool stretched;
	bool flip;
	
	LTexture* spritesheet;
	
};

#endif
