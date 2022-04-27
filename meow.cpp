#include "meow.h"

#include <cmath>

void Meow::updateAnimation()
{
    if (imageName == "res/fireball.png" or imageName == "res/fireball2.png") {
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
        setSrc(40 * (6 - tick), 0, 40, 40);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 6) {
            tick = 0;
        }
    }
    else {
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
}

void Meow::chasePlayer(int x, int y, int playerW, int playerH)
{
    // setting up vector direction
    float xDir = x - getDest().x;
    float yDir = y - getDest().y;
    // adjust so that the meow stop once it reaches the player model not position
    xDir += playerW / 4 - 3;
    yDir += playerH / 4 + 20;
    // normalize direction
    float xMove = xDir / sqrt(xDir * xDir + yDir * yDir);
    float yMove = yDir / sqrt(xDir * xDir + yDir * yDir);
    // move the meow object
    if (xDir != 0 or yDir != 0) {
        if (getDest().x < x)
            setDest(getDest().x + xMove * velocity, getDest().y + yMove * velocity / 1.4);
        else setDest(getDest().x + xMove * velocity / 2, getDest().y + yMove * velocity / 1.4);
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
