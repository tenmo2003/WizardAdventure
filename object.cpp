#include "object.h"

Object::Object() {
    
}

void Object::setSrc(int x, int y, int w, int h) {
    src.x = x;
    src.y = y;
    src.w = w;
    src.h = h;
}

void Object::setDest(int x, int y, int w, int h) {
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
}

void Object::setDest(int x, int y) {
    dest.x = x;
    dest.y = y;
}

void Object::setDest(int , int w, int h) {
    dest.w = w;
    dest.h = h;
}

void Object::setImage(std::string filename, SDL_Renderer * renderer) {
    tex = IMG_LoadTexture(renderer, filename.c_str());
}

void Object::draw(SDL_Renderer * renderer) {
    SDL_RenderCopy(renderer, tex, &src, &dest);
}