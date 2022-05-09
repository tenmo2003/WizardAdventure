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
        return 20;
    if (level == 4)
        return 30;
    if (level == 5)
        return 45;
    if (level == 6)
        return 60;
    if (level == 7)
        return 80;
    if (level == 8)
        return 100;
    if (level == 9)
        return 125;
    if (level == 10)
        return 150;
    if (level == 11)
        return 180;
    if (level == 12)
        return 210;
    if (level == 13)
        return 240;
    if (level == 14)
        return 275;
    if (level == 15)
        return 315;
    if (level == 16)
        return 355;
    if (level == 17)
        return 400;
    if (level == 18)
        return 450;
    if (level == 19)
        return 500;
    if (level == 20)
        return 550;
}
