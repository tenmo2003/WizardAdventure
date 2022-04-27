#include "playerObject.h"

Player::Player() {
    x_val = 0;
    y_val = 0;
    x_pos = 0;
    y_pos = 0;
    frame_width = 0;
    frame_height = 0;
    state = -1;
}

Player::~Player() {

}

bool Player::loadImage(std::string filename, SDL_Renderer *screen) {
    bool ret = Object::setImage(filename, screen);

    if (ret) {
        if (state == IDLELEFT) {
            frame_width = rect_.w / 6;
            frame_height = rect_.h / 6;
        }
        if (state == IDLERIGHT) {
            frame_width = rect_.w / 6;
            frame_height = rect_.h / 6;
        }
        if (state == RUNLEFT) {
            frame_width = rect_.w / 8;
            frame_height = rect_.h / 8;
        }
        if (state == RUNRIGHT) {
            frame_width = rect_.w / 8;
            frame_height = rect_.h / 8;
        }
        if (state == TAKEDAMAGELEFT) {
            frame_width = rect_.w / 3;
            frame_height = rect_.h / 3;
        }
        if (state == TAKEDAMAGERIGHT) {
            frame_width = rect_.w / 3;
            frame_height = rect_.h / 3;
        }
        if (state == DYINGLEFT) {
            frame_width = rect_.w / 12;
            frame_height = rect_.h / 12;
        }
        if (state == DYINGRIGHT) {
            frame_width = rect_.w / 12;
            frame_height = rect_.h / 12;
        }
        if (state == CHARGINGLEFT) {
            frame_width = rect_.w / 5;
            frame_height = rect_.h / 5;
        }
        if (state == CHARGINGRIGHT) {
            frame_width = rect_.w / 5;
            frame_height = rect_.h / 5;
        }
    }
    return ret;
}

void Player::setClips() {
    if (state == IDLELEFT or state == IDLERIGHT) {
        frame_width = rect_.w / 6;
        frame_height = rect_.h / 6;
        for (int i = 0; i < 6; i++) {
            frame_clip[i].x = 0;
            frame_clip[i].y = i * frame_height;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
    if (state == RUNLEFT or state == RUNRIGHT) {
        for (int i = 0; i < 8; i++) {
            frame_clip[i].x = 0;
            frame_clip[i].y = i * frame_height;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
    if (state == TAKEDAMAGELEFT or state == TAKEDAMAGERIGHT) {
        for (int i = 0; i < 3; i++) {
            frame_clip[i].x = 0;
            frame_clip[i].y = i * frame_height;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
    if (state == DYINGLEFT or state == DYINGRIGHT) {
        for (int i = 0; i < 12; i++) {
            frame_clip[i].x = 0;
            frame_clip[i].y = i * frame_height;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
    if (state == CHARGINGLEFT or state == CHARGINGRIGHT) {
        for (int i = 0; i < 5; i++) {
            frame_clip[i].x = 0;
            frame_clip[i].y = i * frame_height;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
}

void Player::show(SDL_Renderer *des) {
    if (state == RUNLEFT) {
        loadImage("res/player_runleft.png", des);
    }else if (state == RUNRIGHT) {
        loadImage("res/player_runright.png", des);
    }else if (state == IDLELEFT) {
        loadImage("res/player_idleleft.png", des);
    }else if (state == IDLERIGHT) {
        loadImage("res/player_idleright.png", des);
    }else if (state == TAKEDAMAGELEFT) {
        loadImage("res/player_takedamageleft.png", des);
    }else if (state == TAKEDAMAGERIGHT) {
        loadImage("res/player_takedamageright.png", des);
    }else if (state == DYINGLEFT) {
        loadImage("res/player_dyingleft.png", des);
    }else if (state == DYINGRIGHT) {
        loadImage("res/player_dyingright.png", des);
    }else if (state == CHARGINGLEFT) {
        loadImage("res/player_chargingleft.png", des);
    }else if (state == CHARGINGRIGHT) {
        loadImage("res/player_chargingright.png", des);
    }

    if (input_type.left == 1 or input_type.right == 1 or input_type.up == 1 or input_type.down == 1) {
        frame_tick++;
    }else
        frame_tick = 0;

    if (state == IDLELEFT or state == IDLERIGHT) {
        if (frame_tick >= 6) {
            frame_tick = 0;
        }
    }
    if (state == RUNLEFT or state == RUNRIGHT) {
        if (frame_tick >= 8) {
            frame_tick = 0;
        }
    }
    if (state == TAKEDAMAGELEFT or state == TAKEDAMAGERIGHT) {
        if (frame_tick >= 3) {
            frame_tick = 0;
        }
    }
    if (state == DYINGLEFT or state == DYINGRIGHT) {
        if (frame_tick >= 12) {
            frame_tick = 0;
        }
    }
    if (state == CHARGINGLEFT or state == CHARGINGRIGHT) {
        if (frame_tick >= 5) {
            frame_tick = 0;
        }
    }

    rect_.x = x_pos;
    rect_.y = y_pos;

    SDL_Rect* currentTick = &frame_clip[frame_tick];

    SDL_Rect renderQuad = {rect_.x, rect_.y, frame_width, frame_height};

    SDL_RenderCopy(des, tex, currentTick, &renderQuad);
}

void Player::handleInputs(SDL_Event &e, SDL_Renderer *screen) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
            {
                state = RUNLEFT;
                input_type.left == 1;
            }
                break;
            case SDLK_d:
            {
                state = RUNRIGHT;
                input_type.right == 1;
            }
                break;
            case SDLK_w:
            {
                state = RUNLEFT;
                input_type.up == 1;
            }
                break;
            case SDLK_s:
            {
                state = RUNLEFT;
                input_type.down == 1;
            }
                break;
            default:
                break;
        }
    }if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
            {
                state = IDLELEFT;
                input_type.left == 0;
            }
                break;
            case SDLK_d:
            {
                state = IDLERIGHT;
                input_type.right == 0;
            }
                break;
            case SDLK_w:
            {
                state = IDLELEFT;
                input_type.up == 0;
            }
                break;
            case SDLK_s:
            {
                state = IDLELEFT;
                input_type.down == 0;
            }
                break;
            default:
                break;
        }
    }
}
