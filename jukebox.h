#ifndef __JUKEBOX__
#define __JUKEBOX__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

using std::string;

class jukebox{
public:
	jukebox();
	~jukebox();
	
	Mix_Chunk *loadSound(const string & path);
	void freeSound(Mix_Chunk*);
	void playSound(Mix_Chunk*);
	void soundSetVolume(Mix_Chunk*, float); 		//Sonido específico. De 0=min a 1=max
	void setGeneralSoundVolume(float volume); 		//Todos los sonidos. De 0=min a 1=max
	float getGeneralSoundVolume();
	
	Mix_Music *loadMusic(const string & path);
	void freeMusic(Mix_Music*);
	void playMusic(Mix_Music*, bool loop, bool fadeIn);
	void pauseMusic();
	void resumeMusic();
	void haltMusic(bool fadeOut);
	void musicSetVolume(float); 		//de 0=min a 1=max
	float getMusicVolume();
	bool musicIsPlaying();
	
private:
	float soundVolume, musicVolume;
};

#endif
