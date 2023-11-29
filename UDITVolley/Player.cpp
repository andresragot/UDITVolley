#include "Player.h"

Player& Player::operator=(const Player& c) {
    name = c.name;
    id = c.id;
    return *this;
}

Player::Player() {
	name = "DEFAULT";
	id = 0;
}

void Player::handleEvent(SDL_Event& e) {
    //If a key was pressed
    if (id % 2 == 0) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:if (canJump) { mVelY -= JUMP_SPEED; canJump = false;}break;
            case SDLK_LEFT: mVelX -= mSpeed; break;
            case SDLK_RIGHT: mVelX += mSpeed; break;
            }
        }
        //If a key was released
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {
        
            case SDLK_LEFT: mVelX += mSpeed; break;
            case SDLK_RIGHT: mVelX -= mSpeed; break;
            }
        }
    }
    else {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {
            case SDLK_w:if (canJump) { mVelY -= JUMP_SPEED; canJump = false; }break;
            case SDLK_a: mVelX -= mSpeed; break;
            case SDLK_d: mVelX += mSpeed; break;
            }
        }
        //If a key was released
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            //Adjust the velocity
            switch (e.key.keysym.sym)
            {

            case SDLK_a: mVelX += mSpeed; break;
            case SDLK_d: mVelX -= mSpeed; break;
            }
        }
    }
}

void Player::move(SDL_Rect& wall, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    //Move the Ball left or right
    x += mVelX;
    pCollider.x = x;

    //If the Ball went too far to the left or right
    if ((x < 0) || (x + PLAYER_WIDTH > SCREEN_WIDTH) || checkCollision(wall, pCollider))
    {
        //Move back
        x -= mVelX;
        pCollider.x = x;
    }

    //Move the Ball up or down
    y += mVelY;
    pCollider.y = y;

    //If the Ball went too far up or down
    if ((y < 0) || (y + PLAYER_HEIGHT > SCREEN_HEIGHT) || checkCollision(wall, pCollider))
    {
        //Move back
        y -= mVelY;
        pCollider.y = y;
    }
    if (y + PLAYER_HEIGHT == SCREEN_HEIGHT) {
        mVelY = 0;
        canJump = true;
    }
}

void Player::render(LTexture& gPlayerTexture, SDL_Renderer* gRenderer)
{
    //Show the Ball
    gPlayerTexture.render(gRenderer, x, y);
}

bool Player::checkCollision(SDL_Rect a, SDL_Rect b) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void Player::update() {
    mVelY += GRAVITY;
}