#ifndef CLOCK_DEFINED
#define CLOCK_DEFINED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Esta clase ya no sirve

class Clock{
public:
	Clock();
	~Clock();
	
	unsigned int getDeltaT();
	void reset();
	void update();
	
private:
	unsigned int prevTime;
	unsigned int curTime;
	unsigned int deltaT;
	
};

#endif
