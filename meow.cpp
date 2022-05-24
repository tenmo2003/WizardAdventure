#include "meow.h"

#include <cmath>

void Meow::updateAnimation()
{
    if (imageName == "res/slime.png") {
        setSrc(32 * tick, 32, 32, 32);
        if (begin > speed)
        {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 6)
        {
            tick = 0;
        }
    }
    else if (imageName == "res/dog.png" or imageName == "res/racoon2.png" or imageName == "res/frog2.png") {
        speed = 9;
        setSrc(16 * tick, 0, 16, 16);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 2) {
            tick = 0;
        }
    }
    else if (imageName == "res/flame.png") {
        setSrc(16 * 3, 16 * tick, 16, 16);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 4) {
            tick = 0;
        }
    }
    else if (imageName == "res/fireball.png" or imageName == "res/fireball2.png") {
        setSrc(50 * tick, 0, 50, 50);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 5) {
            tick = 0;
        }
    }
    else if (imageName == "res/frog.png") {
        setSrc(40 * tick, 0, 40, 40);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 6) {
            tick = 0;
        }
    }
    else if (imageName == "res/racoon.png") {
        setSrc(60 * tick, 0, 60, 60);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 5) {
            tick = 0;
        }
    }
}

void Meow::chasePlayer(int x, int y, int playerW, int playerH)
{
    // setting up vector direction
    float xDir = x - getDest().x;
    float yDir = y - getDest().y;
    // adjust so that the meow stop once it reaches the player model position
    xDir += playerW / 4 - 3;
    yDir += playerH / 4 + 20;
    // normalize direction
    float xMove = xDir / sqrt(xDir * xDir + yDir * yDir);
    float yMove = yDir / sqrt(xDir * xDir + yDir * yDir);
    // move the meow object
    if (xDir != 0 or yDir != 0) {
        if (getDest().x < x)
            setDest(getDest().x + xMove * velocity, getDest().y + yMove * velocity / 1.5);
        else setDest(getDest().x + xMove * velocity / 2, getDest().y + yMove * velocity / 1.5);
    }
}

bool Meow::dead()
{
    if (health <= 0)
        return true;
    else
        return false;
}

bool Meow::canAttack()
{
    if (attackCd == 0) {
        attackCd = 2;
        return true;
    }
    else
        return false;
}