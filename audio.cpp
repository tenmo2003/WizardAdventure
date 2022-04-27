#include "audio.h"

Audio::~Audio() {
    SDL_CloseAudioDevice(deviceID);
    SDL_FreeWAV(wavBuffer);
}

void Audio::load(const char* filename) {
    SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength);
    deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

}

void Audio::play() {
    SDL_QueueAudio(deviceID, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceID, 0);
}

void Audio::stop() {
    SDL_PauseAudioDevice(deviceID, 1);
}

bool Audio::donePlaying() {
    return SDL_GetQueuedAudioSize(deviceID) == 0;
}