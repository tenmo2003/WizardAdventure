#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

#include "object.h"
#include "player.h"
#include "defines.h"
#include "meow.h"

class Game
{
public:
    Game();
    ~Game();

    void initSDL(const char *title, int width, int height, bool fullscreen);
    void initGame();
    void clean();

    void menu();
    void update();
    void handleAnimationsAndMovements();
    void handleInputs();
    void render();

    void write(const std::string &msg, int x, int y, SDL_Color color, int size);

    void drawEntity(Object &entity, int camx, int camy);
    void drawEntity(Object &entity, int camx, int camy, bool flip);

    bool collisionPlayer(Object &a, Object &b);
    bool collision(Object &a, Object &b);

    void renderPlayerAttackCdBar(int x, int y, float percent);
    void renderPlayerHpBar(int x, int y, float percent);
    void renderPlayerExpBar(int x, int y, float percent);

    void spawnEnemies();

    void handlePlayerLevel();

private:
    bool isRunning, appRunning = true, fullscreen, paused, won;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *map;
    SDL_Rect camera;
    int mapWidth, mapHeight;

    SDL_Event e;
    int mouseX, mouseY;

    // fps related stuff
    int thisTime, startTime, fps, frameCount, timerFPS, timeInSeconds, lastTime, lastTimeF;

    // control
    bool l, r, u, d, charging, movable, reset, canShoot;

    Player player;
    std::vector<Meow> enemies;
    int killCount;
    std::vector<Object> bullets, hpOrbs;

    Mix_Music *bgMusic;
    Mix_Chunk *hitSound, *atkSound, *bulletSound, *deathSound;
};
