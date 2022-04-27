#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "object.h"

typedef struct Input
{
    int left;
    int right;
    int up;
    int down;
};

class Player : public Object
{
public:
    Player();
    ~Player();

    enum states
    {
        IDLELEFT = 0,
        IDLERIGHT = 1,
        RUNLEFT = 3,
        RUNRIGHT = 4,
        TAKEDAMAGELEFT = 5,
        TAKEDAMAGERIGHT = 6,
        DYINGLEFT = 7,
        DYINGRIGHT = 8,
        CHARGINGLEFT = 9,
        CHARGINGRIGHT = 10,
    };

    bool loadImage(std::string filename, SDL_Renderer *screen);
    void show(SDL_Renderer *des);
    void handleInputs(SDL_Event &e, SDL_Renderer *screen);
    void setClips();

private:
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int frame_width, frame_height;

    SDL_Rect frame_clip[12];
    Input input_type;
    int frame_tick;

    int state;
};
