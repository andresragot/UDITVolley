#include "Player.h"

/*
    FUNCTIONS
*/

Player& Player::operator=(const Player& c) 
{
    name = c.name;
    games = c.games;
    wins = c.wins;
    return *this;
}

Player::Player() 
{
	name = "DEFAULT";
	id = 0;
}

SDL_Rect& Player::get_collider()
{
    return pCollider;
}

void Player::set_collider(SDL_Rect _position)
{
    pCollider.x = _position.x;
    pCollider.y = _position.y;
}

void Player::handle_event(SDL_Event& event_handler)
{
    if (is_online)
    {
        if (event_handler.type == SDL_KEYDOWN && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
            case SDLK_w:if (canJump) { mVelY -= JUMP_SPEED; canJump = false; }break;
            case SDLK_a: mVelX -= mSpeed; break;
            case SDLK_d: mVelX += mSpeed; break;
            }
        }
        // If a key was released
        else if (event_handler.type == SDL_KEYUP && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
            case SDLK_a: mVelX += mSpeed; break;
            case SDLK_d: mVelX -= mSpeed; break;
            }
        }
    }

    // If a key was pressed
    else if (id % 2 == 0) 
    {
        if (event_handler.type == SDL_KEYDOWN && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
                case SDLK_UP:if (canJump) { mVelY -= JUMP_SPEED; canJump = false;}break;
                case SDLK_LEFT: mVelX -= mSpeed; break;
                case SDLK_RIGHT: mVelX += mSpeed; break;
            }
        }
        // If a key was released
        else if (event_handler.type == SDL_KEYUP && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
                case SDLK_LEFT: mVelX += mSpeed; break;
                case SDLK_RIGHT: mVelX -= mSpeed; break;
            }
        }
    }
    else 
    {
        if (event_handler.type == SDL_KEYDOWN && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
                case SDLK_w:if (canJump) { mVelY -= JUMP_SPEED; canJump = false; }break;
                case SDLK_a: mVelX -= mSpeed; break;
                case SDLK_d: mVelX += mSpeed; break;
            }
        }
        // If a key was released
        else if (event_handler.type == SDL_KEYUP && event_handler.key.repeat == 0)
        {
            // Adjust the velocity
            switch (event_handler.key.keysym.sym)
            {
                case SDLK_a: mVelX += mSpeed; break;
                case SDLK_d: mVelX -= mSpeed; break;
            }
        }
    }
}

void Player::move(SDL_Rect& wall) 
{
    // Move the Player left or right
    pCollider.x += mVelX;

    // If the Player went too far to the left or right
    if ((pCollider.x < 0) || (pCollider.x + PLAYER_WIDTH > SCREEN_WIDTH) || check_collision(wall, pCollider))
    {
        // Move back
        pCollider.x -= mVelX;
    }

    // Move the Player up or down
    pCollider.y += mVelY;

    // If the Player went too far up or down
    if ((pCollider.y < 0) || (pCollider.y + PLAYER_HEIGHT > SCREEN_HEIGHT) || check_collision(wall, pCollider))
    {
        // Move back
        pCollider.y -= mVelY;
    }
    if (pCollider.y + PLAYER_HEIGHT == SCREEN_HEIGHT) {
        mVelY = 0;
        canJump = true;
    }
}

void Player::render(LTexture& gPlayerTexture, SDL_Renderer* gRenderer)
{
    // Show the Ball
    gPlayerTexture.render(gRenderer, pCollider.x, pCollider.y);
}

bool Player::check_collision(SDL_Rect a, SDL_Rect b) 
{
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // If any of the sides from A are outside of B
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

    // If none of the sides from A are outside B
    return true;
}

void Player::update() 
{
    mVelY += GRAVITY;
}