#include "player.h"

void Player::updateAnimation(int state) {
    if (state == IDLELEFT or state == IDLERIGHT)
    {
        setSrc(0, 48 * tick, 48, 48);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 6) {
            tick = 0;
        }
    }
    if (state == RUNLEFT or state == RUNRIGHT) {
        setSrc(0, 48 * tick, 48, 48);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 8) {
            tick = 0;
        }
    }
    if (state == CHARGINGLEFT or state == CHARGINGRIGHT) {
        setSrc(0, 48 * tick, 48, 48);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 5) {
            tick = 0;
        }
    }
    if (state == ATTACKLEFT or state == ATTACKRIGHT or state == ATTACK) {
        speed = 4;
        attacking = 1;
        setSrc(0, 46 * tick, 104, 46);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 9) {
            attacking = 0;
            tick = 0;
            speed = 7;
        }
    }
    if (state == DYINGLEFT or state == DYINGRIGHT) {
        setSrc(0, 48 * tick, 48, 48);
        if (begin > speed) {
            tick++;
            begin = 0;
        }
        begin++;
        if (tick >= 12) {
            tick = 0;
            doneDying = 1;
        }
    }
}

bool Player::canAttack()
{
    if (attackTimer <= 0) {
        attackTimer = attackCd;
        return true;
    }
    else
        return false;
}

bool Player::canShoot()
{
    if (shootTimer <= 0) {
        shootTimer = shootCd;
        return true;
    }
    else
        return false;
}

float Player::getLevelExp() const {
    if (level == 1)
        return 5;
    if (level == 2)
        return 10;
    if (level == 3)
        return 25;
    if (level == 4)
        return 43;
    if (level == 5)
        return 65;
    if (level == 6)
        return 90;
    if (level == 7)
        return 120;
    if (level == 8)
        return 150;
    if (level == 9)
        return 185;
    if (level == 10)
        return 220;
    if (level == 11)
        return 260;
    if (level == 12)
        return 300;
    if (level == 13)
        return 350;
    if (level == 14)
        return 400;
    if (level == 15)
        return 450;
    if (level == 16)
        return 520;
    if (level == 17)
        return 620;
    if (level == 18)
        return 730;
    if (level == 19)
        return 840;
    if (level == 20)
        return 960;
}