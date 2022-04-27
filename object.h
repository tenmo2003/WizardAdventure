#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "defines.h"

class Object
{
public:
    Object();

    void setSrc(int x, int y, int w, int h);
    SDL_Rect getSrc() const { return src; }

    void setDest(int x, int y, int w, int h);
    void setDest(int x, int y);
    void setDest(int ,int w, int h);
    SDL_Rect getDest() const { return dest; }

    void setDir(float x, float y) { xDir = x; yDir = y; }
    float getXDir() const { return xDir; }
    float getYDir() const { return yDir; }

    void setImage(std::string fileName, SDL_Renderer *renderer);
    SDL_Texture* getObject() const { return tex; }

    bool operator==(const Object &other) const
    {
        return (getDest().x == other.getDest().x && getDest().y == other.getDest().y);
    }

    void draw(SDL_Renderer *renderer);

private:
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Texture *tex;

    float xDir, yDir;
};