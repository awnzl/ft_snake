#include "audiowrapper.h"

AudioWrapper::AudioWrapper()
{
	// Initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);
	//Initialize SDL_mixer
	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	// Load sound effects
	startSound = Mix_LoadWAV("audio/startGame.wav");
	eat = Mix_LoadWAV("audio/bit.wav");
	step = Mix_LoadWAV("audio/step.wav");
	endSound = Mix_LoadMUS("audio/gameOver.wav");
}

AudioWrapper::~AudioWrapper()
{
	// clean up our resources
	Mix_FreeChunk(startSound);
	Mix_FreeChunk(eat);
	Mix_FreeChunk(step);
	Mix_FreeMusic(endSound);

	// quit SDL_mixer
	Mix_CloseAudio();
}

void AudioWrapper::startGame()
{
	Mix_PlayChannel(-1, startSound, 0);
}

void AudioWrapper::soundEat()
{
	Mix_PlayChannel(-1, eat, 0);
}

void AudioWrapper::soundStep()
{
	Mix_PlayChannel(-1, step, 0);
}

void AudioWrapper::endGame()
{
	Mix_PlayMusic( endSound, 0);

	while ( Mix_PlayingMusic() ) ;
}

extern "C" AudioWrapper *createAudioWrapper()
{
    return new AudioWrapper();
}

extern "C" void releaseAudioWrapper(AudioWrapper *ptr)
{
    delete ptr;
}