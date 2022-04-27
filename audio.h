#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Audio
{
public:
    Audio() {}
    ~Audio();
    void load(const char *filename);
    void play();
    bool donePlaying();
    void stop();

private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceID;
};
