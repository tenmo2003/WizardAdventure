#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "object.h"
#include "defines.h"

class Player : public Object
{
public:
    Player() {}

    void setHealth(int h) { 
        health = h;
        if (health <= 0)
            health = 0;
        if (health >= maxHealth)
            health = maxHealth;
    }
    int getHealth() const { return health; }

    void setMaxHealth(int h) { maxHealth = h; }
    int getMaxHealth() const { return maxHealth; }

    void setPlayerState(int p) {
        begin = 0;
        tick = 0;
        playerState = p;
    }
    int getPlayerState() const { return playerState; }

    void setVelocity(float v) { velocity = v; }
    float getVelocity() const { return velocity; }

    void updateAnimation(int state);

    void attack() { 
        attacking = 1;
        attackTimer = attackCd;
    }
    bool isAttacking() const { return attacking; }

    void setDamage(int d) { damage = d; }
    int getDamage() const { return damage; }

    bool canAttack();
    void setAttackTimer(float t) { attackTimer = t; }
    float getAttackTimer() const { return attackTimer; }
    void setAttackCd(float cd) { attackCd = cd; }
    float getAttackCd() const { return attackCd; }

    bool canShoot();
    void setShootTimer(float t) { shootTimer = t; }
    float getShootTimer() const { return shootTimer; }
    void setShootCd(float cd) { shootCd = cd; }
    float getShootCd() const { return shootCd; }

    void setBulletDamage(int damage) { bulletDamage = damage; }
    int getBulletDamage() const { return bulletDamage; }

    void setExp(float a) { exp = a; }
    float getExp() const { return exp; }
    
    void setLevel(int l) { level = l; }
    int getLevel() const { return level; }
    float getLevelExp() const;

    bool done() { 
        if (doneDying == true) {
            doneDying = false;
            return true;
        }
        return false;
    }

private:
    int health, maxHealth;
    float velocity;
    int damage = 5, bulletDamage = 5;

    float exp = 0;
    int level = 1;

    float attackTimer = 0, attackCd = 2.3, shootTimer = 1, shootCd = 2;

    int playerState;
    int begin, tick = 0, speed = 7;

    bool attacking = 0;

    bool doneDying = false;
};