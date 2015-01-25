#ifndef ENGINE_INCLUDED
#define ENGINE_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "painter.h"
#include "control.h"
#include "juego.h"

class Engine{
public:
	//Engine();
	Engine(int, int, bool);
	~Engine();
	
	void run();
	void step();
	void draw();
	bool isRunning();
	void handleEvents();
	
private:
	int screen_width, screen_height;
	bool fullscreen;
	
	bool running;
	
	SDL_Window* window;
	SDL_Renderer* gRenderer;
	
	painter* picasso;
	juego* game;
	control* controller;
	SDL_Event* event;
};

#endif
