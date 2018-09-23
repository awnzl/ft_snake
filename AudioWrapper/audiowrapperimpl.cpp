#include "audiowrapperimpl.h"

AudioWrapperImpl::AudioWrapperImpl()
{
    alutInit(0, NULL);
    alGetError();
	startSound = alutCreateBufferFromFile("audio/startGame.wav");
	eat = alutCreateBufferFromFile("audio/bit.wav");
	step = alutCreateBufferFromFile("audio/step.wav");
	endSound = alutCreateBufferFromFile("audio/gameOver.wav");

    alGenSources(4, sources);
    alSourcei(sources[0], AL_BUFFER, startSound);
    alSourcei(sources[1], AL_BUFFER, eat);
    alSourcei(sources[2], AL_BUFFER, step);
    alSourcei(sources[3], AL_BUFFER, endSound);
}

AudioWrapperImpl::~AudioWrapperImpl()
{
    alDeleteSources(4, sources);
    alDeleteSources(1, &startSound);
    alDeleteBuffers(1, &eat);
    alDeleteBuffers(1, &step);
    alDeleteBuffers(1, &endSound);
    alutExit();
}

void AudioWrapperImpl::startGame()
{
    alSourcePlay(sources[0]);
}

void AudioWrapperImpl::soundEat()
{
    alSourcePlay(sources[1]);
}

void AudioWrapperImpl::soundStep()
{
	alSourcePlay(sources[2]);
}

void AudioWrapperImpl::endGame()
{
	alSourcePlay(sources[3]);
}

extern "C" AudioWrapper *createAudioWrapper()
{
    return new AudioWrapperImpl();
}

extern "C" void releaseAudioWrapper(AudioWrapper *ptr)
{
    delete ptr;
}