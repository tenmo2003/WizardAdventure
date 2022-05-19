#include "game.h"
#include "object.h"

Game::Game()
{
    initSDL("Wizard Adventure: The Weird Invasion!!!", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    while (appRunning)
    {
        isRunning = true;
        fullscreen = false;
        paused = false;
        canShoot = false;
        won = false;

        srand(time(NULL));

        l = r = u = d = charging = 0;
        movable = 1, reset = 1;

        menu();

        lastTime = 0;
        lastTimeF = 0;

        killCount = 0;
        timeInSeconds = 0;
        startTime = SDL_GetTicks();

        initGame();

        Mix_PlayMusic(bgMusic, -1);

        while (isRunning and !won)
        {
            thisTime = SDL_GetTicks() - startTime;
            if (thisTime >= (lastTime + 1000))
            {
                srand(time(NULL));
                if (!paused)
                    timeInSeconds++;
                lastTime = thisTime;
                fps = frameCount;
                frameCount = 0;

                if (!paused) {
                    for (Meow &p : enemies)
                    {
                        if (p.getAttackCd() > 0)
                            p.setAttackCd(p.getAttackCd() - 1);
                    }

                    spawnEnemies();
                }
            }

            if (thisTime >= (lastTimeF + 100))
            {
                lastTimeF = thisTime;
                if (!paused) {
                    if (player.getAttackTimer() > 0 and !player.isAttacking())
                    {
                        player.setAttackTimer(player.getAttackTimer() - 0.1);
                    }
                    if (canShoot)
                    {
                        if (player.getShootTimer() > 0)
                        {
                            player.setShootTimer(player.getShootTimer() - 0.1);
                        }
                    }
                }
            }

            if (Mix_PlayingMusic() == 0)
            {
                
                Mix_PlayMusic(bgMusic, -1);
            }

            if (!paused) 
                update();
            handleInputs();
            render();
        }

        if (appRunning) {
            if (!won and !paused) {
                write("You died! Press any key to continue.", WINDOW_WIDTH / 6 + 25, WINDOW_HEIGHT / 2 - 50, {255, 255, 255}, 50);
                SDL_RenderPresent(renderer);
                Mix_Chunk *lose;
                lose = Mix_LoadWAV("res/lose.wav");
                Mix_PlayChannel(-1, lose, 0);
            }
            else if (won and !paused) {
                write("You won!!! Congratulations!!!", WINDOW_WIDTH / 6 + 110, WINDOW_HEIGHT / 2 - 70, {255, 255, 255}, 50);
                write("You are now qualified as a legendary wizard!", WINDOW_WIDTH / 6 - 100, WINDOW_HEIGHT / 2, {255, 255, 255}, 50);
                SDL_RenderPresent(renderer);
                Mix_Chunk *win;
                win = Mix_LoadWAV("res/win.wav");
                Mix_PlayChannel(-1, win, 0);
            }
        }
        while (appRunning and !paused) {
            bool pressed = false;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    appRunning = false;
                }
                if (e.type == SDL_KEYDOWN)
                    pressed = true;
            }
            if (pressed)
                break;
        }
        Mix_HaltMusic();
        Mix_HaltChannel(-1);
    }
}

Game::~Game()
{
    clean();
}

void Game::initSDL(const char *title, int width, int height, bool fullscreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Initialization failed" << std::endl;
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) < 0)
        std::cout << "Window creation failed" << std::endl;
    SDL_SetWindowTitle(window, title);
    
    TTF_Init();
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Game::initGame() {
    std::string filename = "res/track" + std::to_string(rand() % 7 + 1) + ".wav";
    bgMusic = Mix_LoadMUS(filename.c_str());
    atkSound = Mix_LoadWAV("res/atk.wav");
    bulletSound = Mix_LoadWAV("res/fire.wav");
    deathSound = Mix_LoadWAV("res/oof.wav");
    hitSound = Mix_LoadWAV("res/hit.wav");

    map = IMG_LoadTexture(renderer, "res/mapr.png");
    SDL_QueryTexture(map, NULL, NULL, &mapWidth, &mapHeight);
    camera = {0, 0, 480, 480 * 9 / 16};

    player.setImage("res/player_idleright.png", renderer);
    player.setDest(0, 0, 32 * 2, 48 * 2);
    player.setDest(1995, 1490, 32 * 2, 48 * 2);
    player.setSrc(0, 0, 48, 48);
    player.setPlayerState(IDLERIGHT);
    player.setVelocity(2.5);
    player.setExp(0);
    player.setLevel(1);
    player.setMaxHealth(100);
    player.setHealth(100);

    enemies.clear();
    bullets.clear();
    //enemies.push_back(Meow(1600, 1900, 32 * 1.5, 32 * 1.5, 10, 1, 100, "res/slime.png", renderer));

    for (int i = 0; i < MAX_ORBS; i++) {
        Object orb;
        orb.setImage("res/heart.png", renderer);
        orb.setSrc(0, 0, 16, 16);
        orb.setDest(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 16, 16);
        hpOrbs.push_back(orb);
    }
}

void Game::menu()
{
    Object startButton, exitButton, credit, arrow;

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
        while (!chose)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    isRunning = false;
                    appRunning = false;
                    chose = true;
                    break;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_RETURN or e.key.keysym.sym == SDLK_SPACE)
                    {
                        if (choice == 1)
                        {
                            chose = true;
                            break;
                        }
                        if (choice == 2)
                        {
                            isRunning = false;
                            appRunning = false;
                            chose = true;
                            break;
                        }
                    }
                    if (e.key.keysym.sym == SDLK_UP or e.key.keysym.sym == SDLK_w)
                    {
                        if (choice == 1)
                        {
                            choice = 2;
                            arrow.setDest(exitButton.getDest().x - 140, exitButton.getDest().y, 213, 154);
                        }
                        else if (choice == 2)
                        {
                            choice = 1;
                            arrow.setDest(startButton.getDest().x - 140, startButton.getDest().y, 213, 154);
                        }
                    }
                    if (e.key.keysym.sym == SDLK_DOWN or e.key.keysym.sym == SDLK_s)
                    {
                        if (choice == 2)
                        {
                            choice = 1;
                            arrow.setDest(startButton.getDest().x - 140, startButton.getDest().y, 213, 154);
                        }
                        else if (choice == 1)
                        {
                            choice = 2;
                            arrow.setDest(exitButton.getDest().x - 140, exitButton.getDest().y, 213, 154);
                        }
                    }
                }
            }

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
        if (e.type == SDL_QUIT)
        {
            isRunning = false;
            appRunning = false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_p)
            {
                paused = !paused;
                movable = !movable;
            }
            if (paused) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
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
                        Mix_PlayChannel(-1, atkSound, 0);
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
        if (player.getPlayerState() != DYINGRIGHT)
        {
            player.setPlayerState(DYINGRIGHT);
            player.setImage("res/player_dyingright.png", renderer);
            Mix_PlayChannel(-1, deathSound, 0);
        }
        movable = 0;
        player.updateAnimation(DYINGRIGHT);
        if (player.done())
        {
            isRunning = 0;
        }
        return;
    }

    if (timeInSeconds > PHASE18 and enemies.empty()) {
        won = true;
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

    if (hpOrbs.size() < MAX_ORBS) {
        Object temp;
        temp.setImage("res/heart.png", renderer);
        temp.setSrc(0, 0, 16, 16);
        temp.setDest(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 16, 16);
        hpOrbs.push_back(temp);
    }

    if (player.canShoot())
    {
        Object bullet;
        bullet.setImage("res/cat.png", renderer);
        bullet.setSrc(0, 0, 16, 16);
        bullet.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
        // bullet.setDir(mouseX - bullet.getDest().x, mouseY - bullet.getDest().y);
        bullet.setAngle(atan2(mouseY - bullet.getDest().y, mouseX - bullet.getDest().x));
        bullets.push_back(bullet);
        if (player.getLevel() >= 5) {
            Object bullet2;
            bullet2.setImage("res/cat.png", renderer);
            bullet2.setSrc(0, 0, 16, 16);
            bullet2.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet2.setDir(mouseX - 50 - bullet2.getDest().x, mouseY - 50 - bullet2.getDest().y);
            bullet2.setAngle(bullet.getAngle() + 0.2);
            bullets.push_back(bullet2);
        }
        if (player.getLevel() >= 7) {
            Object bullet3;
            bullet3.setImage("res/cat.png", renderer);
            bullet3.setSrc(0, 0, 16, 16);
            bullet3.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet3.setDir(mouseX + 50 - bullet3.getDest().x, mouseY + 50 - bullet3.getDest().y);
            bullet3.setAngle(bullet.getAngle() - 0.2);
            bullets.push_back(bullet3);
        }
        if (player.getLevel() >= 9) {
            Object bullet4;
            bullet4.setImage("res/cat.png", renderer);
            bullet4.setSrc(0, 0, 16, 16);
            bullet4.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet4.setDir(mouseX - 100 - bullet4.getDest().x, mouseY - 100 - bullet4.getDest().y);
            bullet4.setAngle(bullet.getAngle() + 0.4);
            bullets.push_back(bullet4);
        }
        if (player.getLevel() >= 11) {
            Object bullet5;
            bullet5.setImage("res/cat.png", renderer);
            bullet5.setSrc(0, 0, 16, 16);
            bullet5.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet5.setDir(mouseX + 100 - bullet5.getDest().x, mouseY + 100 - bullet5.getDest().y);
            bullet5.setAngle(bullet.getAngle() - 0.4);
            bullets.push_back(bullet5);
        }
        if (player.getLevel() >= 13) {
            Object bullet6;
            bullet6.setImage("res/cat.png", renderer);
            bullet6.setSrc(0, 0, 16, 16);
            bullet6.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet6.setDir(mouseX - 150 - bullet6.getDest().x, mouseY - 150 - bullet6.getDest().y);
            bullet6.setAngle(bullet.getAngle() + 0.6);
            bullets.push_back(bullet6);
        }
        if (player.getLevel() >= 15) {
            Object bullet7;
            bullet7.setImage("res/cat.png", renderer);
            bullet7.setSrc(0, 0, 16, 16);
            bullet7.setDest(player.getDest().x + player.getDest().w / 2 - 8, player.getDest().y + player.getDest().h / 2 - 8, 16, 16);
            // bullet7.setDir(mouseX + 150 - bullet7.getDest().x, mouseY + 150 - bullet7.getDest().y);
            bullet7.setAngle(bullet.getAngle() - 0.6);
            bullets.push_back(bullet7);
        }
        Mix_PlayChannel(-1, bulletSound, 0);
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
        for (Meow &p : enemies)
        {
            p.setTookDamage(false);
        }
    }

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;

    if (camera.x + camera.w >= mapWidth)
        camera.x = mapWidth - camera.w;
    if (camera.y + camera.h >= mapHeight)
        camera.y = mapHeight - camera.h;

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

        for (Object &b : bullets)
        {
            if (collision(p, b))
            {
                p.setHealth(p.getHealth() - player.getBulletDamage());
                bullets.erase(std::remove(bullets.begin(), bullets.end(), b), bullets.end());
            }
        }

        if (collisionPlayer(player, p) and player.isAttacking() and !p.tookDamage())
        {
            p.setHealth(p.getHealth() - player.getDamage());
            // // knockback p a bit
            // if (player.getPlayerState() == ATTACKLEFT)
            // {
            //     p.setDest(p.getDest().x - 30, p.getDest().y);
            // }
            // else if (player.getPlayerState() == ATTACKRIGHT)
            // {
            //     p.setDest(p.getDest().x + 30, p.getDest().y);
            // }
            p.setTookDamage(true);
        }
        else if (collisionPlayer(player, p) and p.canAttack() and !player.isAttacking())
        {
            player.setHealth(player.getHealth() - p.getDamage());
            Mix_PlayChannel(-1, hitSound, 0);
        }

        if (p.dead())
        {
            player.setExp(player.getExp() + p.getExp());
            enemies.erase(std::remove(enemies.begin(), enemies.end(), p), enemies.end());
            killCount++;
        }
    }

    for (Object &h : hpOrbs) {
        if (collisionPlayer(player, h)) {
            hpOrbs.erase(std::remove(hpOrbs.begin(), hpOrbs.end(), h), hpOrbs.end());
            player.setHealth(player.getHealth() + player.getMaxHealth() * 0.1);
        }
    }

    handlePlayerLevel();

    player.updateAnimation(player.getPlayerState());
}

void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, map, &camera, NULL);

    for (Object &h : hpOrbs) {
        drawEntity(h, camera.x, camera.y);
    }

    for (Meow &p : enemies)
    {
        drawEntity(p, camera.x, camera.y, p.getFlip());
    }

    for (Object &b : bullets)
    {
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

    // get exp bar on the upper side of the screen
    renderPlayerExpBar(0, 0, (float)player.getExp() / player.getLevelExp());

    write(std::to_string(timeInSeconds / 60) + " : " + std::to_string(timeInSeconds % 60), WINDOW_WIDTH / 2 - 40, 15, {255, 255, 255, 255}, 40);
    write("Player level: " + std::to_string(player.getLevel()), WINDOW_WIDTH - 5 - 200, 15, {255, 255, 255, 255}, 25);
    write("Slayed: " + std::to_string(killCount), 5, 15, {255, 255, 255, 255}, 25);
    write("P to pause", 5, WINDOW_HEIGHT - 33, {255, 255, 255, 255}, 25);

    if (paused) {
        write("Paused!", WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 120, {255, 255, 255, 255}, 40);
        write("Press P again to resume", WINDOW_WIDTH / 2 - 220, WINDOW_HEIGHT / 2 - 50, {255, 255, 255, 255}, 40);
        write("Press ESC to go back to menu", WINDOW_WIDTH / 2 - 280, WINDOW_HEIGHT / 2 + 30, {255, 255, 255, 255}, 40);
    }

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
    SDL_DestroyTexture(map);
    map = NULL;

    Mix_FreeChunk(hitSound);
    hitSound = NULL;
    Mix_FreeChunk(bulletSound);
    bulletSound = NULL;
    Mix_FreeChunk(deathSound);
    deathSound = NULL;
    Mix_FreeChunk(atkSound);
    atkSound = NULL;

    Mix_FreeMusic(bgMusic);
    bgMusic = NULL;

    enemies.clear();
    bullets.clear();

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

bool Game::collision(Object &a, Object &b)
{
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

    for (Object &b : bullets)
    {
        b.setDest(b.getDest().x + cos(b.getAngle()) * 5, b.getDest().y + sin(b.getAngle()) * 5);

        if (b.getDest().x > mapWidth + 1000 or b.getDest().x < -1000 or b.getDest().y > mapHeight + 1000 or b.getDest().y < -1000)
        {
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
            if (player.getDest().x <= mapWidth + 800 - player.getDest().w)
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
            if (player.getDest().y <= mapHeight + 440 - player.getDest().h)
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
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE1)
    {
        for (int i = 0; i < 20; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE2)
    {
        for (int i = 0; i < 30; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE3)
    {
        for (int i = 0; i < 40; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 5, 1, 2, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE4)
    {
        for (int i = 0; i < 50; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 10, 1, 4, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE5)
    {
        for (int i = 0; i < 60; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 10, 2, 4, "res/slime.png", renderer));
        }
        enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 40 * 1.5, 40 * 1.5, 300, 40, 25, "res/frog.png", renderer));
    }
    if (timeInSeconds == PHASE6)
    {
        for (int i = 0; i < 70; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 10, 2, 4, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE7)
    {
        for (int i = 0; i < 80; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 17, 2, 6, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE8)
    {
        for (int i = 0; i < 90; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 17, 2, 6, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE9)
    {
        for (int i = 0; i < 100; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 17, 2, 6, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE10)
    {
        for (int i = 0; i < 110; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 25, 3, 8, "res/slime.png", renderer));
        }
        enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 50 * 1.5, 50 * 1.5, 800, 125, 60, "res/fireball.png", renderer));
    }
    if (timeInSeconds == PHASE11)
    {
        for (int i = 0; i < 120; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 25, 3, 8, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE12)
    {
        for (int i = 0; i < 130; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 25, 3, 8, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE13)
    {
        for (int i = 0; i < 140; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 30, 3, 10, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE14)
    {
        for (int i = 0; i < 150; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 30, 3, 15, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE15)
    {
        for (int i = 0; i < 160; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 30, 4, 15, "res/slime.png", renderer));
        }
        enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 50 * 1.5, 50 * 1.5, 1500, 250, 100, "res/fireball2.png", renderer));
    }
    if (timeInSeconds == PHASE16)
    {
        for (int i = 0; i < 170; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 35, 4, 20, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE17)
    {
        for (int i = 0; i < 180; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 35, 4, 20, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE18)
    {
        for (int i = 0; i < 190; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 50, 4, 30, "res/slime.png", renderer));
        }
    }
    if (timeInSeconds == PHASE19)
    {
        for (int i = 0; i < 200; i++)
        {
            enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 32 * 1.5, 32 * 1.5, 50, 4, 30, "res/slime.png", renderer));
        }
        enemies.push_back(Meow(rand() % (mapWidth + 750) , rand() % (mapHeight + 350), 60 * 2, 60 * 2, 3000, 300, 130, "res/racoon.png", renderer));
    }
}

void Game::handlePlayerLevel()
{
    if (player.getLevel() == 1 and player.getExp() >= player.getLevelExp())
    {
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

    if (player.getLevel() == 3 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(4);
        player.setMaxHealth(player.getMaxHealth() + 10);
    }

    if (player.getLevel() == 4 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(5);
        player.setAttackCd(player.getAttackCd() - 0.3);
        player.setBulletDamage(player.getBulletDamage() + 5);
    }

    if (player.getLevel() == 5 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(6);
        player.setDamage(player.getDamage() + 5);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.1);
    }

    if (player.getLevel() == 6 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(7);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setHealth(player.getHealth() + 10);
        player.setAttackCd(player.getAttackCd() - 0.3);
    }

    if (player.getLevel() == 7 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(8);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setBulletDamage(player.getBulletDamage() + 5);
    }

    if (player.getLevel() == 8 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(9);
        player.setDamage(player.getDamage() + 5);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.1);
    }

    if (player.getLevel() == 9 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(10);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setHealth(player.getHealth() + 10);
        player.setVelocity(player.getVelocity() + 0.2);
    }

    if (player.getLevel() == 10 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(11);
        player.setAttackCd(player.getAttackCd() - 0.3);
        player.setBulletDamage(player.getBulletDamage() + 4);
    }

    if (player.getLevel() == 11 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(12);
        player.setDamage(player.getDamage() + 5);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.1);
    }

    if (player.getLevel() == 12 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(13);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setHealth(player.getHealth() + 10);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.2);
    }

    if (player.getLevel() == 13 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(14);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setBulletDamage(player.getBulletDamage() + 5);
    }

    if (player.getLevel() == 14 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(15);
        player.setDamage(player.getDamage() + 5);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.2);
    }

    if (player.getLevel() == 15 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(16);
        player.setMaxHealth(player.getMaxHealth() + 20);
        player.setHealth(player.getHealth() + 10);
        player.setVelocity(player.getVelocity() + 0.2);
    }

    if (player.getLevel() == 16 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(17);
        player.setAttackCd(player.getAttackCd() - 0.3);
        player.setBulletDamage(player.getBulletDamage() + 5);
    }

    if (player.getLevel() == 17 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(18);
        player.setDamage(player.getDamage() + 5);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.1);
    }

    if (player.getLevel() == 18 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(19);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setHealth(player.getHealth() + 10);
        player.setShootCd(player.getShootCd() - player.getShootCd() * 0.1);
    }

    if (player.getLevel() == 19 and player.getExp() >= player.getLevelExp())
    {
        player.setExp(player.getExp() - player.getLevelExp());
        player.setLevel(20);
        player.setMaxHealth(player.getMaxHealth() + 10);
        player.setBulletDamage(player.getBulletDamage() + 5);
        player.setDamage(player.getDamage() + 5);
        player.setAttackCd(player.getAttackCd() - 0.3);
    }
}
