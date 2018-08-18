#include "audiowrapper.h"

AudioWrapper::AudioWrapper() {
    buffer.loadFromFile("audio/StartGame.wav");
    sound.setBuffer(buffer);
}

AudioWrapper::~AudioWrapper() {

}

void AudioWrapper::soundStartGame()
{
    sound.play();
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep(sf::milliseconds(100));
    }
}