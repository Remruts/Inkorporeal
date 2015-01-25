#include "clock.h"

Clock::Clock(){
	deltaT = 0;
	curTime = 0;
	prevTime = 0;
}

Clock::~Clock(){
	
}
	
unsigned int Clock::getDeltaT(){
	return deltaT;
}

void Clock::reset(){
	prevTime = 0;
}

void Clock::update(){
	curTime = SDL_GetTicks();
	deltaT = curTime - prevTime;
	prevTime = curTime;
}
