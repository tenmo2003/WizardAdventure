#include "game.h"
#include "object.h"

Game::Game()
{
    init("Wizard Adventure: The Cuteness Invasion!!!", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    while(appRunning) {
        isRunning = true;
        fullscreen = false;
        paused = false;
        canShoot = false;

        srand(time(NULL));

        l = r = u = d = charging = 0;
        movable = 1, reset = 1;

        menu();

        if (!first) lastTime = 0;
        startTime = SDL_GetTicks();

        TTF_Init();

        bgMusic.load("res/track.wav");
        atkSound.load("res/atk.wav");
        bulletSound.load("res/meow.wav");
        deathSound.load("res/oof.wav");
        hitSound.load("res/hit.wav");

        map = IMG_LoadTexture(renderer, "res/mapr.png");
        SDL_QueryTexture(map, NULL, NULL, &levelWidth, &levelHeight);
        camera = {0, 0, 480, 480 * 9 / 16};

        player.setImage("res/player_idleright.png", renderer);
        player.setDest(0, 0, 32 * 2, 48 * 2);
        player.setDest(1995, 1490, 32 * 2, 48 * 2);
        player.setSrc(0, 0, 48, 48);
        player.setPlayerState(IDLERIGHT);
        player.setVelocity(2.5);
        player.setMaxHealth(100);
        player.setHealth(100);

        enemies.clear();
        enemies.push_back(Meow(0, 0, 32 * 1.5, 32 * 1.5, 10, 1, 2, "res/slime.png", renderer));
        enemies.push_back(Meow(1600, 1900, 32 * 1.5, 32 * 1.5, 10, 1, 100, "res/slime.png", renderer));

        bgMusic.play();

        while (isRunning)
        {
            thisTime = SDL_GetTicks() - startTime;
            if (thisTime >= (lastTime + 1000))
            {
                timeInSeconds = thisTime / 1000;
                lastTime = thisTime;
                fps = frameCount;
                frameCount = 0;

                if (player.getAttackTimer() > 0 and !player.isAttacking())
                {
                    player.setAttackTimer(player.getAttackTimer() - 1);
                }

                for (Meow &p : enemies)
                {
                    if (p.getAttackCd() > 0)
                        p.setAttackCd(p.getAttackCd() - 1);
                    // std::cout << p.getAttackCd() << std::endl;
                }

                // std::cout << timeInSeconds << std::endl;
                spawnEnemies();
            }

            if (thisTime >= (lastTimeF + 100)) {
                lastTimeF = thisTime;
                if (player.getAttackTimer() > 0 and !player.isAttacking())
                {
                    player.setAttackTimer(player.getAttackTimer() - 0.1);
                }
                if (canShoot) {
                    if (player.getShootTimer() > 0)
                    {
                        player.setShootTimer(player.getShootTimer() - 0.1);
                    }
                }
            }

            if (bgMusic.donePlaying())
            {
                bgMusic.play();
            }

            if (!paused)
                update();
            handleInputs();
            render();
        }
        bgMusic.stop();
        first = false;
    }
}

Game::~Game()
{
    clean();
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Initialization failed" << std::endl;
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) < 0)
        std::cout << "Window creation failed" << std::endl;
    SDL_SetWindowTitle(window, title);
}

void Game::menu() {
    startButton.setSrc(0, 0, 195, 160);
    startButton.setDest(WINDOW_WIDTH / 2 - startButton.getSrc().w / 2, WINDOW_HEIGHT / 2 - startButton.getSrc().h / 2 - 70, 195, 160);
    startButton.setImage("res/start.png", renderer);
    exitButton.setSrc(0, 0, 156, 160);
    exitButton.setDest(WINDOW_WIDTH / 2 - exitButton.getSrc().w / 2, WINDOW_HEIGHT / 2 - exitButton.getSrc().h / 2 + 70, 156, 160);
    exitButton.setImage("res/exit.png", renderer);
    arrow.setSrc(0, 0, 213, 154);
    arrow.setDest(startButton.getDest().x - 140, startButton.getDest().y, 213, 154);
    arrow.setImage("res/arrow.png", renderer);
    credit.setSrc(0, 0, 441, 123);
    credit.setDest(0, 0, 441 / 1.5, 123 / 1.5);
    credit.setImage("res/credit.png", renderer);

    {
        int choice = 1;
        bool chose = false;
        SDL_Texture *bg = IMG_LoadTexture(renderer, "res/bg.png");
        while (true) {
            //get input for selected choice
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    isRunning = false;
                    appRunning = false;
                    chose = true;
                    break;
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_RETURN or e.key.keysym.sym == SDLK_SPACE) {
                        if (choice == 1) {
                            chose = true;
                            break;
                        }
                        if (choice == 2) {
                            isRunning = false;
                            appRunning = false;
                            chose = true;
                            break;
                        }
                    }
                    if (e.key.keysym.sym == SDLK_UP or e.key.keysym.sym == SDLK_w) {
                        if (choice == 1) {
                            choice = 2;
                            arrow.setDest(exitButton.getDest().x - 140, exitButton.getDest().y, 213, 154);
                        }
                        else if (choice == 2) {
                            choice = 1;
                            arrow.setDest(startButton.getDest().x - 140, startButton.getDest().y, 213, 154);
                        }
                    }
                    if (e.key.keysym.sym == SDLK_DOWN or e.key.keysym.sym == SDLK_s) {
                        if (choice == 2) {
                            choice = 1;
                            arrow.setDest(startButton.getDest().x - 140, startButton.getDest().y, 213, 154);
                        }
                        else if (choice == 1) {
                            choice = 2;
                            arrow.setDest(exitButton.getDest().x - 140, exitButton.getDest().y, 213, 154);
                        }
                    }
                }
            }
            if (chose) break;
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bg, NULL, NULL);
            startButton.draw(renderer);
            exitButton.draw(renderer);
            arrow.draw(renderer);
            credit.draw(renderer);
            thisTime = SDL_GetTicks();
            timerFPS = SDL_GetTicks() - thisTime;
            if (timerFPS < (1000 / 30))
            {
                SDL_Delay((1000 / 30) - timerFPS);
            }
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyTexture(bg);
    }
}

void Game::handleInputs()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) {
            isRunning = false;
            appRunning = false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                paused = !paused;
                movable = !movable;
            }
            if (e.key.keysym.sym == SDLK_F11)
                fullscreen = !fullscreen;
            if (movable)
            {
                if (e.key.keysym.sym == SDLK_a)
                {
                    l = 1;
                    r = 0;
                }
                if (e.key.keysym.sym == SDLK_d)
                {
                    r = 1;
                    l = 0;
                }
                if (e.key.keysym.sym == SDLK_w)
                {
                    u = 1;
                    d = 0;
                }
                if (e.key.keysym.sym == SDLK_s)
                {
                    d = 1;
                    u = 0;
                }
                if (e.key.keysym.sym == SDLK_l)
                {
                    charging = 1;
                }
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    if (player.canAttack())
                    {
                        player.attack();
                        atkSound.play();
                        if (player.getPlayerState() == IDLERIGHT or player.getPlayerState() == RUNRIGHT)
                            player.setDest(player.getDest().x - 49, player.getDest().y, 104 * 2, 48 * 2);
                        else if (player.getPlayerState() == IDLELEFT or player.getPlayerState() == RUNLEFT)
                            player.setDest(player.getDest().x - 95, player.getDest().y, 104 * 2, 48 * 2);
                    }
                }
            }
        }

        if (e.type == SDL_KEYUP)
        {
            if (movable)
            {
                if (e.key.keysym.sym == SDLK_a)
                {
                    l = 0;
                    player.setPlayerState(IDLELEFT);
                    player.setImage("res/player_idleleft.png", renderer);
                }
                if (e.key.keysym.sym == SDLK_d)
                {
                    r = 0;
                    player.setPlayerState(IDLERIGHT);
                    player.setImage("res/player_idleright.png", renderer);
                }
                if (e.key.keysym.sym == SDLK_w)
                {
                    u = 0;
                    if (player.getPlayerState() == RUNLEFT)
                    {
                        player.setPlayerState(IDLELEFT);
                        player.setImage("res/player_idleleft.png", renderer);
                    }
                    else if (player.getPlayerState() == RUNRIGHT)
                    {
                        player.setPlayerState(IDLERIGHT);
                        player.setImage("res/player_idleright.png", renderer);
                    }
                }
                if (e.key.keysym.sym == SDLK_s)
                {
                    d = 0;
                    if (player.getPlayerState() == RUNLEFT)
                    {
                        player.setPlayerState(IDLELEFT);
                        player.setImage("res/player_idleleft.png", renderer);
                    }
                    else if (player.getPlayerState() == RUNRIGHT)
                    {
                        player.setPlayerState(IDLERIGHT);
                        player.setImage("res/player_idleright.png", renderer);
                    }
                }
                if (e.key.keysym.sym == SDLK_l)
                {
                    charging = 0;
                    if (player.getPlayerState() == CHARGINGRIGHT)
                    {
                        player.setPlayerState(IDLERIGHT);
                        player.setImage("res/player_idleright.png", renderer);
                    }
                    else if (player.getPlayerState() == CHARGINGLEFT)
                    {
                        player.setPlayerState(IDLELEFT);
                        player.setImage("res/player_idleleft.png", renderer);
                    }
                }
            }
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseX += camera.x;
        mouseY += camera.y;
    }
}

void Game::update()
{
    if (fullscreen)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
    }

    if (player.getHealth() <= 0)
    {
        if (player.getPlayerState() != DYINGRIGHT) {
            player.setPlayerState(DYINGRIGHT);
            player.setImage("res/player_dyingright.png", renderer);
            deathSound.play();
        }
        movable = 0;
        player.updateAnimation(DYINGRIGHT);
        if (player.done()) {
            isRunning = 0;
        }
        return;
    }

    if (player.isAttacking())
    {
        movable = 0;
        reset = 0;
        l = r = u = d = 0;
    }
    else
    {
        movable = 1;
    }

    if (movable and !reset)
    {
        if (player.getPlayerState() == ATTACKRIGHT)
            player.setDest(player.getDest().x + 49, player.getDest().y, 32 * 2, 48 * 2);
        else
            player.setDest(player.getDest().x + 95, player.getDest().y, 32 * 2, 48 * 2);
        reset = 1;
    }

    if (player.canShoot()) {
        Object bullet;
        bullet.setImage("res/cat.png", renderer);
        bullet.setSrc(0, 0, 16, 16);
        bullet.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
        bullet.setDir(mouseX - bullet.getDest().x, mouseY - bullet.getDest().y);
        bullets.push_back(bullet);
        bulletSound.play();
    }

    handleAnimationsAndMovements();

    if (player.getPlayerState() == ATTACKLEFT or player.getPlayerState() == ATTACKRIGHT)
        player.setDest(0, 104 * 2, 46 * 2);
    else
        player.setDest(0, 32 * 2, 48 * 2);

    if (!player.isAttacking())
    {
        camera.x = (player.getDest().x + player.getDest().w / 2) - WINDOW_WIDTH / 2;
        camera.y = (player.getDest().y + player.getDest().h / 2) - WINDOW_HEIGHT / 2;
        for (Meow&p : enemies) {
            p.setTookDamage(false);
        }
    }

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;

    if (camera.x + camera.w >= levelWidth)
        camera.x = levelWidth - camera.w;
    if (camera.y + camera.h >= levelHeight)
        camera.y = levelHeight - camera.h;

    for (Meow &p : enemies)
    {
        p.chasePlayer(player.getDest().x, player.getDest().y, player.getDest().w, player.getDest().h);

        if (p.getDest().x < player.getDest().x)
        {
            p.setFlip(false);
        }
        if (p.getDest().x > player.getDest().x)
            p.setFlip(true);

        if (!collisionPlayer(player, p))
        {
            p.setVelocity(2);
        }
        else
        {
            if (player.getPlayerState() == IDLELEFT or player.getPlayerState() == IDLERIGHT)
                p.setVelocity(0);
            else
                p.setVelocity(-3);
        }

        p.updateAnimation();

        for (Object &b : bullets) {
            if (collision(p, b)) {
                p.setHealth(p.getHealth() - player.getBulletDamage());
                bullets.erase(std::remove(bullets.begin(), bullets.end(), b), bullets.end());
            }
        }

        if (collisionPlayer(player, p) and player.isAttacking() and !p.tookDamage())
        {
            p.setHealth(p.getHealth() - player.getDamage());
            //knockback p a bit
            if (player.getPlayerState() == ATTACKLEFT)
            {
                p.setDest(p.getDest().x - 30, p.getDest().y);
            }
            else if (player.getPlayerState() == ATTACKRIGHT)
            {
                p.setDest(p.getDest().x + 30, p.getDest().y);
            }
            p.setTookDamage(true);
        }
        else if (collisionPlayer(player, p) and p.canAttack() and !player.isAttacking())
        {
            player.setHealth(player.getHealth() - p.getDamage());
            hitSound.play();
        }

        if (p.dead())
        {
            player.setExp(player.getExp() + p.getExp());
            enemies.erase(std::remove(enemies.begin(), enemies.end(), p), enemies.end());
        }
    }

    handlePlayerLevel();

    player.updateAnimation(player.getPlayerState());

}

void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, map, &camera, NULL);

    write(std::to_string(timeInSeconds / 60) + " : " + std::to_string(timeInSeconds % 60), 600, 20, {255, 255, 255, 255}, 50);

    for (Meow &p : enemies)
    {
        drawEntity(p, camera.x, camera.y, p.getFlip());
    }

    for (Object &b : bullets) {
        drawEntity(b, camera.x, camera.y);
    }

    if (!player.isAttacking())
        drawEntity(player, camera.x, camera.y);
    else if (player.getPlayerState() == ATTACKRIGHT)
        drawEntity(player, camera.x - 32, camera.y);
    else if (player.getPlayerState() == ATTACKLEFT)
        drawEntity(player, camera.x + 32, camera.y);

    if (!player.isAttacking())
        renderPlayerHpBar(player.getDest().x - camera.x + 11, player.getDest().y - 10 + player.getDest().h - camera.y, (float)player.getHealth() / player.getMaxHealth());
    else if (player.getPlayerState() == ATTACKRIGHT)
        renderPlayerHpBar(player.getDest().x - camera.x + 11 + 38, player.getDest().y - 5 + player.getDest().h - camera.y, (float)player.getHealth() / player.getMaxHealth());
    else if (player.getPlayerState() == ATTACKLEFT)
        renderPlayerHpBar(player.getDest().x - camera.x + 11 + 105, player.getDest().y - 5 + player.getDest().h - camera.y, (float)player.getHealth() / player.getMaxHealth());

    //get exp bar on the upper side of the screen
    renderPlayerExpBar(0, 0, (float)player.getExp() / player.getLevelExp());

    frameCount++;
    timerFPS = SDL_GetTicks() - startTime - thisTime;
    if (timerFPS < (1000 / 60))
    {
        SDL_Delay((1000 / 60) - timerFPS);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

void Game::write(const std::string &msg, int x, int y, SDL_Color color, int size)
{
    TTF_Font *font = TTF_OpenFont("res/font.ttf", size);
    SDL_Surface *surf;
    SDL_Texture *tex;
    SDL_Rect rect;
    surf = TTF_RenderText_Solid(font, msg.c_str(), color);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    rect.x = x;
    rect.y = y;
    rect.w = surf->w;
    rect.h = surf->h;
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    SDL_DestroyTexture(tex);
    TTF_CloseFont(font);
}

void Game::draw(Object &object)
{
    SDL_Rect src = object.getSrc();
    SDL_Rect dest = object.getDest();
    SDL_Texture *tex = object.getObject();
    SDL_RenderCopy(renderer, tex, &src, &dest);
}

void Game::draw(Object &object, bool flip)
{
    SDL_Rect src = object.getSrc();
    SDL_Rect dest = object.getDest();
    SDL_Texture *tex = object.getObject();
    if (!flip)
        SDL_RenderCopyEx(renderer, tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
    else
        SDL_RenderCopyEx(renderer, tex, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void Game::drawEntity(Object &entity, int camX, int camY)
{
    SDL_Rect src = entity.getSrc();
    SDL_Rect dest = entity.getDest();
    SDL_Texture *tex = entity.getObject();
    dest.x -= camX;
    dest.y -= camY;
    SDL_RenderCopy(renderer, tex, &src, &dest);
}

void Game::drawEntity(Object &entity, int camX, int camY, bool flip)
{
    SDL_Rect src = entity.getSrc();
    SDL_Rect dest = entity.getDest();
    SDL_Texture *tex = entity.getObject();
    dest.x -= camX;
    dest.y -= camY;
    if (!flip)
        SDL_RenderCopyEx(renderer, tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
    else
        SDL_RenderCopyEx(renderer, tex, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
}

bool Game::collisionPlayer(Object &a, Object &b)
{
    if (a.getDest().x + 25 <= b.getDest().x + b.getDest().w and a.getDest().x + a.getDest().w >= b.getDest().x + 25 and a.getDest().y <= b.getDest().y + b.getDest().h - 40 and a.getDest().y + a.getDest().h - 20 >= b.getDest().y)
        return true;
    else
        return false;
}

bool Game::collision(Object &a, Object &b) {
    if (a.getDest().x <= b.getDest().x + b.getDest().w and a.getDest().x + a.getDest().w >= b.getDest().x and a.getDest().y <= b.getDest().y + b.getDest().h and a.getDest().y + a.getDest().h >= b.getDest().y)
        return true;
    else
        return false;
}

void Game::handleAnimationsAndMovements()
{

    if (player.isAttacking())
    {
        if (player.getPlayerState() == RUNLEFT or player.getPlayerState() == IDLELEFT)
        {
            player.setPlayerState(ATTACKLEFT);
            player.setImage("res/player_attackleft.png", renderer);
        }
        else if (player.getPlayerState() == RUNRIGHT or player.getPlayerState() == IDLERIGHT)
        {
            player.setPlayerState(ATTACKRIGHT);
            player.setImage("res/player_attackright.png", renderer);
        }
    }
    else
    {
        if (player.getPlayerState() == ATTACKLEFT)
        {
            player.setPlayerState(IDLELEFT);
            player.setImage("res/player_idleleft.png", renderer);
        }
        else if (player.getPlayerState() == ATTACKRIGHT)
        {
            player.setPlayerState(IDLERIGHT);
            player.setImage("res/player_idleright.png", renderer);
        }
    }

    for (Object& b : bullets) {
        float angle = atan2(b.getYDir(), b.getXDir());
        b.setDest(b.getDest().x + cos(angle) * 5, b.getDest().y + sin(angle) * 5);

        if (b.getDest().x > levelWidth + 1000 or b.getDest().x < -1000 or b.getDest().y > levelHeight + 1000 or b.getDest().y < -1000) {
            bullets.erase(std::remove(bullets.begin(), bullets.end(), b), bullets.end());
        }
    }

    if (movable)
    {
        if (l)
        {
            if (player.getPlayerState() != RUNLEFT)
            {
                player.setImage("res/player_runleft.png", renderer);
                player.setPlayerState(RUNLEFT);
            }
            if (player.getDest().x >= 0)
                player.setDest(player.getDest().x - player.getVelocity() / 1.3, player.getDest().y, player.getDest().w, player.getDest().h);
        }

        if (r)
        {
            if (player.getPlayerState() != RUNRIGHT)
            {
                player.setImage("res/player_runright.png", renderer);
                player.setPlayerState(RUNRIGHT);
            }
            if (player.getDest().x <= levelWidth + 800 - player.getDest().w)
                player.setDest(player.getDest().x + player.getVelocity(), player.getDest().y, player.getDest().w, player.getDest().h);
        }

        if (u)
        {
            if (player.getPlayerState() == IDLERIGHT or player.getPlayerState() == CHARGINGRIGHT)
            {
                player.setImage("res/player_runright.png", renderer);
                player.setPlayerState(RUNRIGHT);
            }
            else if (player.getPlayerState() == IDLELEFT or player.getPlayerState() == CHARGINGLEFT)
            {
                player.setImage("res/player_runleft.png", renderer);
                player.setPlayerState(RUNLEFT);
            }
            if (player.getDest().y >= 0)
                player.setDest(player.getDest().x, player.getDest().y - player.getVelocity() / 1.45, player.getDest().w, player.getDest().h);
        }

        if (d)
        {
            if (player.getPlayerState() == IDLERIGHT or player.getPlayerState() == CHARGINGRIGHT)
            {
                player.setImage("res/player_runright.png", renderer);
                player.setPlayerState(RUNRIGHT);
            }
            else if (player.getPlayerState() == IDLELEFT or player.getPlayerState() == CHARGINGLEFT)
            {
                player.setImage("res/player_runleft.png", renderer);
                player.setPlayerState(RUNLEFT);
            }
            if (player.getDest().y <= levelHeight + 800 * 9 / 16 - player.getDest().h)
                player.setDest(player.getDest().x, player.getDest().y + player.getVelocity() / 1.25, player.getDest().w, player.getDest().h);
        }

        if (charging)
        {
            if (player.getPlayerState() == IDLERIGHT)
            {
                player.setImage("res/chargingright.png", renderer);
                player.setPlayerState(CHARGINGRIGHT);
            }
            else if (player.getPlayerState() == IDLELEFT)
            {
                player.setImage("res/chargingleft.png", renderer);
                player.setPlayerState(CHARGINGLEFT);
            }
        }
    }
}

void Game::renderPlayerHpBar(int x, int y, float percent)
{
    SDL_Rect hpBar;
    hpBar.x = x;
    hpBar.y = y;
    hpBar.w = 40;
    hpBar.h = 8;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpBar);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    hpBar.w = hpBar.w * percent;
    SDL_RenderFillRect(renderer, &hpBar);
}

void Game::renderPlayerExpBar(int x, int y, float percent)
{
    SDL_Rect expBar;
    expBar.x = x;
    expBar.y = y;
    expBar.w = WINDOW_WIDTH;
    expBar.h = 13;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &expBar);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    expBar.w = expBar.w * percent;
    SDL_RenderFillRect(renderer, &expBar);
}

void Game::spawnEnemies()
{
    if (timeInSeconds == PHASE0)
    {
        for (int i = 0; i < 10; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE1)
    {
        for (int i = 0; i < 20; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE2)
    {
        for (int i = 0; i < 30; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE3)
    {
        for (int i = 0; i < 40; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE4)
    {
        for (int i = 0; i < 50; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 10, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE5)
    {
        for (int i = 0; i < 60; i++)
        {
            enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
        enemies.push_back(Meow(rand() % levelWidth, rand() % levelHeight, 50 * 1.5, 50 * 1.5, 50, 20, 10, "res/frog.png", renderer));
    }
}

void Game::handlePlayerLevel()
{
    if (player.getLevel() == 1 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(2);
        player.setMaxHealth(player.getMaxHealth() + 10);
    }

    if (player.getLevel() == 2 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(3);
        canShoot = true;
        player.setHealth(player.getHealth() + 10);
    }

    if (player.getLevel() == 3 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(4);
        player.setShootCd(player.getShootCd() - 0.3);
    }

    if (player.getLevel() == 4 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(5);
        player.setAttackCd(player.getAttackCd() - 0.3);
        player.setBulletDamage(player.getBulletDamage() + 3);
    }

    if (player.getLevel() == 5 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(6);
        player.setDamage(player.getDamage() + 2);
        player.setShootCd(player.getShootCd() - 0.3);
    }

    if (player.getLevel() == 6 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(7);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setHealth(player.getHealth() + 10);
    }

    if (player.getLevel() == 7 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(8);

    }

    if (player.getLevel() == 8 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(9);
    }

    if (player.getLevel() == 9 and player.getExp() >= player.getLevelExp()) {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(10);
    }
}