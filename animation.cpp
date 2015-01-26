#include "animation.h"

animation::animation(int frmnum, float spd, bool lp, LTexture* sprsh, unsigned int frms[], int w){
	frameNum = frmnum;
	loop = lp;
	width = w;
	height = w;
	spritesheet = sprsh;
	curFrame = 0.0f;
	flip = 0;
	stretched = 0;

	speed = spd;
	if (speed < 0.0f)
		speed = 0.0f;
	
	frames = new unsigned int[frameNum];
	//copiar todo, damnit
	for (int i= 0; i < frameNum; ++i){
		frames[i] = frms[i];
	}
	
}

animation::~animation(){
	delete[] frames;
}
	
void animation::setCurrentFrame(int frm){
	curFrame = frm;
}
const int animation::getCurrentFrame() const{
	return int(curFrame);
}
	
const int& animation::getFrameNum() const{
	return frameNum;
}
	
void animation::setSpeed(float spd){
	if (spd < 0.0f){
		speed = 0.0f;
	}else{
		speed = spd;
	}
}

float animation::getSpeed(){
	return speed;
}

const bool animation::isFinished() const{
	return (curFrame >= frameNum-1 && !loop);
}

void animation::step(){
	curFrame += speed; //* clk->getDeltaT(); //al final, delta no sirve...
	if (int(curFrame) > frameNum-1){
		if (loop){
			curFrame = 0;
		} else {
			curFrame = frameNum-1;
		}
	}
	
}

void animation::draw(painter* disney, int x, int y){
	if (stretched){
		disney->drawEx(spritesheet, width * (frames[int(curFrame)]%(spritesheet->getWidth()/width)), 
			height * (frames[int(curFrame)]/(spritesheet->getWidth()/width)), width, height, 
			x, 448+(320-y*0.7143)-height*0.7143, width, height*0.7143, 0, flip+2);
	} else{
		disney->drawEx(spritesheet, width * (frames[int(curFrame)]%(spritesheet->getWidth()/width)), 
			height * (frames[int(curFrame)]/(spritesheet->getWidth()/width)), width, height, 
			x, y, width, height, 0, flip);
	}
}
	
void animation::setFrameHeight(int h){
	if (h > 0)
		height = h;
}

void animation::setFrameWidth(int w){
	if (w > 0)
		width = w;
}

const int& animation::getFrameWidth(){
	return width;
}

const int& animation::getFrameHeight(){
	return height;
}

void animation::setFlip(bool flp){
	flip = flp;
}

void animation::setStretch(bool strt){
	stretched = strt;
}
