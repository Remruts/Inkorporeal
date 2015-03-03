#include "engine.h"
#include <cstdlib>


/*
//Para qué necesitaría un constructor por defecto?
Engine::Engine(){
	
	screen_width = 1366;
	screen_height = 768;
	
	int error = SDL_Init(SDL_INIT_VIDEO);
	if(error == -1){
		std::cout << "Couldn't initialize SDL...\n";
		exit(1);
    }
	
	int imgFlags = IMG_INIT_PNG; 
	if( (IMG_Init( imgFlags ) & imgFlags) != imgFlags ) { 
		std::cout << "SDL_image fail! SDL_image Error: " << IMG_GetError() << std::endl;
		exit(1);
	}
	
	//Initialize SDL_ttf 
	if( TTF_Init() == -1 ) { 
		std::cout <<"SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (window == NULL){
		std::cout << "Couldn't create window...\n";
		exit(1);
	}
	
	//Create renderer for window 
	gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 
	if( gRenderer == NULL ) { 
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl; 
		exit(1); 
	
	picasso = new painter(gRenderer);
	if (picasso == NULL){
		std::cout << "Picasso's dead. Check your painter.\n";
		exit(1);
	}
	picasso->setColor(0x17, 0x17, 0x17, 0xFF);
	
	controller = new control;
	if (controller == NULL){
		std::cout << "Couldn't initialize controller.\n";
		exit(1);
	}
	
	game = new juego(picasso);
	if (game == NULL){
		std::cout << "Couldn't start the game.\n";
		exit(1);
	}
	
	event = new SDL_Event; // No hay forma de que esto muera
	SDL_ShowCursor(0);
}
*/

Engine::Engine(int w, int h, bool fs){
	
	screen_width = w;
	screen_height = h;
	fullscreen = fs;
	
	int error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if(error == -1){
		std::cout << "Couldn't initialize SDL...\n";
		exit(1);
    }
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { 
		std::cout << "SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
		exit(1);
	}
	
	int imgFlags = IMG_INIT_PNG; 
	if( (IMG_Init( imgFlags ) & imgFlags) != imgFlags ) { 
		std::cout << "SDL_image fail! SDL_image Error: " << IMG_GetError() << std::endl;  
		exit(1);
	}
	
	//Initialize SDL_ttf 
	if( TTF_Init() == -1 ) { 
		std::cout <<"SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		exit(1);
	}
	
	if (fullscreen){
		//window = SDL_CreateWindow("Inkorporeal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		//	1366, 768, SDL_WINDOW_FULLSCREEN);
		window = SDL_CreateWindow("Inkorporeal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	} else{
		window = SDL_CreateWindow("Inkorporeal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			screen_width, screen_height, SDL_WINDOW_SHOWN);
	}
	
	if (window == NULL){
		std::cout << "Couldn't create window...\n";
		exit(1);
	}
	
	//Create renderer for window 
	gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); 
	if( gRenderer == NULL ) { 
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl; 
		exit(1); 
	} 
	/* viejo; no sirve.
	screen = SDL_GetWindowSurface(window);
	if (screen == NULL){
		std::cout << "Couldn't get surface from window...\n";
		exit(1);
	}
	*/
	
	bach = new jukebox();
	if (bach == NULL){
		std::cout << "Bach's dead. Check your jukebox.\n";
		exit(1);
	}
	
	picasso = new painter(gRenderer);
	if (picasso == NULL){
		std::cout << "Picasso's dead. Check your painter.\n";
		exit(1);
	}
	picasso->setColor(0x17, 0x17, 0x17, 0xFF);
	
	controller = new control;
	if (controller == NULL){
		std::cout << "Couldn't initialize controller.\n";
		exit(1);
	}
	
	game = new juego(picasso, bach);
	if (game == NULL){
		std::cout << "Couldn't start the game.\n";
		exit(1);
	}
	
	event = new SDL_Event; // No hay forma de que esto muera
	SDL_ShowCursor(0);
	
	//Para medir fps
	frameCount = 0;
	lastTime = SDL_GetTicks();
	fpsText = 0;
	FPS = 0;
}

Engine::~Engine(){

	if(event != NULL)
		delete event;
		
	if(game != NULL)
		delete game;
		
    if(controller != NULL)
		delete controller;
		
	if(picasso != NULL)
		delete picasso;
	
	if(bach != NULL)
		delete bach;
		
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	gRenderer = NULL;
	window = NULL;
	
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Engine::run(){
	running = true;
	
	while(isRunning()){	
	
		step();
		draw();
		
	}
	
}

void Engine::step(){
	handleEvents();
	game->step(controller);

	if (controller->evScreenshot){
		//código robado de http://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
		SDL_Surface *sshot = SDL_CreateRGBSurface(0, screen_width, screen_height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
		SDL_SaveBMP(sshot, string("screenshot"+std::to_string(time(NULL))+".bmp").c_str());
		SDL_FreeSurface(sshot);
		//
		controller->evScreenshot = false;
	}
}

void Engine::draw(){
	
	//Se omite por efectos
	//picasso->clear();
	
	game->draw();
	
	++frameCount;
	
	if (SDL_GetTicks() > lastTime + 1000){
		FPS = frameCount;
		//std::cout << "fps: " << frameCount << std::endl;
		lastTime = SDL_GetTicks();
		frameCount = 0;
	}
	
	fpsText = picasso->textureFromText(std::to_string(int(FPS)) + " fps", 1, 200, 0, 0);
	if (fpsText != NULL){
		picasso->draw(fpsText, 0, 0, 0, 0, 16, 16);			
		picasso->freeTexture(fpsText);
		fpsText = NULL;
	}
	
	SDL_RenderPresent(gRenderer);	//Update screen 
		
}

bool Engine::isRunning(){
	return(running && game->isRunning());
}

void Engine::handleEvents(){
	
	while(SDL_PollEvent(event)){
		if (event->type == SDL_QUIT){
			running = false;
		}
        else{
			controller->update(event);
		}	
	}
	
}

