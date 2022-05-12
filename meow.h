#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "object.h"

class Meow : public Object
{
public:
    Meow() {}

    Meow(int x, int y, int w, int h, int hp, int experience, int dmg, const char *image, SDL_Renderer *renderer)
    {
        setDest(x, y, w, h);
        setImage(image, renderer);
        imageName = image;
        maxHealth = hp;
        health = maxHealth;
        exp = experience;
        damage = dmg;
    }

    void setFlip(bool f) { flip = f; }
    bool getFlip() const { return flip; }

    void setHealth(int h) { health = h; }
    int getHealth() const { return health; }

    void setMaxHealth(int h) { maxHealth = h; }
    int getMaxHealth() const { return maxHealth; }

    void chasePlayer(int x, int y, int, int);
    void updateAnimation();

    void setVelocity(float v) { velocity = v; }
    float getVelocity() const { return velocity; }

    int getX() const { return getDest().x; }
    int getY() const { return getDest().y; }

    int getDamage() const { return damage; }

    bool dead();

    bool canAttack();
    void setAttackCd(int cd) { attackCd = cd; }
    int getAttackCd() const { return attackCd; }

    // define == for 2 meow object
    bool operator==(const Meow &other) const
    {
        return (getX() == other.getX() && getY() == other.getY());
    }

    int getExp() const { return exp; }

    bool tookDamage() const { return tookDmg; }
    void setTookDamage(bool t) { tookDmg = t; }

private:
    int health, maxHealth;
    float velocity = 2;

    int attackCd = 2;

    bool tookDmg = false;

    int exp;

    std::string imageName;

    bool flip = 0;
    int begin = 0, tick, speed = 7;
    int damage = 2;
};