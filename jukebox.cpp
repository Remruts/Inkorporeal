#include "jukebox.h"

jukebox::jukebox(){
	Mix_AllocateChannels(24); //24 canales es suficiente?
	setGeneralSoundVolume(0.6);
	musicSetVolume(0.5);
	delay = 0;
	fadeTime = 1500;
}

jukebox::~jukebox(){
	if (musicIsPlaying()){
		haltMusic(0);
	}
}

//sound
Mix_Chunk* jukebox::loadSound(const string & path){
	return Mix_LoadWAV(path.c_str());
}

void jukebox::freeSound(Mix_Chunk* sound){
	Mix_FreeChunk(sound);
}

void jukebox::playSound(Mix_Chunk* sound){
	
	if(delay<5){
		if (Mix_PlayChannel(-1, sound, 0) == -1){
			std::cout << "Couldn't play sound. Error: " << Mix_GetError() << std::endl;
		}
		delay +=1;
	}
}

void jukebox::soundSetVolume(Mix_Chunk* sound, float volume){
	
	if (volume < 0){
		volume = 0;
	} else if (volume > 1){
		volume = 1;
	}
	
	Mix_VolumeChunk(sound, MIX_MAX_VOLUME*volume);
}

float jukebox::getGeneralSoundVolume(){
	return soundVolume;
}

void jukebox::setGeneralSoundVolume(float volume){
	if (volume < 0){
		volume = 0;
	}else if (volume > 1){
		volume = 1;
	}
	
	soundVolume = volume;
	Mix_Volume(-1, MIX_MAX_VOLUME*volume);
}

//music
Mix_Music* jukebox::loadMusic(const string & path){
	return Mix_LoadMUS(path.c_str());
}

void jukebox::freeMusic(Mix_Music* music){
	Mix_FreeMusic(music);
}

void jukebox::playMusic(Mix_Music* music, bool loop, bool fadeIn){
	
	int error;
	
	if (fadeIn){
		error = Mix_FadeInMusic(music, (loop ? -1: 1), fadeTime);
	} else{
		error = Mix_PlayMusic(music, (loop ? -1: 1));
	}
		
	if (error == -1){
		std::cout << "Couldn't play music. Error: " << Mix_GetError() << std::endl;
	}
	
}

void jukebox::rewindMusic(){
	Mix_RewindMusic();
}

void jukebox::pauseMusic(){
	if (!Mix_PausedMusic()){
		Mix_PauseMusic();
	}
}

void jukebox::resumeMusic(){
	if (Mix_PausedMusic()){
		Mix_ResumeMusic();
	}
}

void jukebox::haltMusic(bool fadeOut){
	if (fadeOut){
		Mix_FadeOutMusic(fadeTime);
	} else{
		Mix_HaltMusic();
	}
	
}

void jukebox::musicSetVolume(float volume){
	
	if (volume < 0){
		volume = 0;
	}else if (volume > 1){
		volume = 1;
	}
	
	musicVolume = volume;
	
	Mix_VolumeMusic(MIX_MAX_VOLUME*volume);
}

float jukebox::getMusicVolume(){
	return musicVolume;
}

bool jukebox::musicIsPlaying(){
	return (Mix_PlayingMusic() && !Mix_PausedMusic());
}

void jukebox::setFade(int fade){
	if (fade<0){
		fade = 0;
	}
	fadeTime = fade;
}

void jukebox::update(){
	if (delay>0)
		delay -= 1;
}
