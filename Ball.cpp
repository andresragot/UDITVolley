#include "Ball.h"

Ball::Ball()
{
    //Initialize the offsets
    ballCollider.x = 300;
    ballCollider.y = 50;

    //Set collision box dimension
    ballCollider.r = RADIUS;

    //Initialize the velocity
    mVelX = Ball_VEL;
    mVelY = Ball_VEL;
}

void Ball::move(SDL_Rect& wall, int SCREEN_WIDTH, int SCREEN_HEIGHT, Player& p1, Player& p2)
{
    //If the Ball went too far to the left or right
    if ((ballCollider.x - ballCollider.r < 0) || (ballCollider.x + ballCollider.r > SCREEN_WIDTH))
    {
        //Move back
        mVelX *= -1;
    }

    //If the Ball went too far up or down
    if ((ballCollider.y - ballCollider.r < 0))
    {
        //Move back
        mVelY *= -1;
    }

    //Ball touched the floor
    if ((ballCollider.y + ballCollider.r > SCREEN_HEIGHT)) 
    {
        printf("Toque el suelo\n");
        if (ballCollider.x + ballCollider.r < SCREEN_WIDTH / 2 ) {
            p2.points++;
            mVelY *= -1;
        }
        else {
            p1.points++;
            mVelY *= -1;
        }
    }

    if (checkCollision(wall, pos)) {
        if (wall.x <= pos.x && (wall.x + wall.h >= pos.x)) {
            if (wall.y <= pos.y+pos.y) {
                mVelY *= -1;
            }
        }
        else {
            mVelX *= -1;
        }
    }

    if (checkCollision(p1.pCollider, pos)) {
        if (p1.x <= pos.x && (p1.x + p1.pCollider.h >= pos.x)) {
            if (p1.pCollider.y <= pos.y+pos.h) {
                mVelY *= -1;
            }
        }
        else {
            mVelX *= -1;
        }
    }
    if (checkCollision(p2.pCollider, pos)) {
        if (p2.x <= pos.x && (p2.x + p2.pCollider.h >= pos.x)) {
            if (p2.pCollider.y <= pos.y+pos.h) {
                mVelY *= -1;
            }
        }
        else {
            mVelX *= -1;
        }
    }

    //Move the Ball left or right
    pos.x +=mVelX;

    //Move the Ball up or down
    pos.y += mVelY;

}

void Ball::render(LTexture& gBallTexture, SDL_Renderer* gRenderer)
{
    //Show the Ball
    gBallTexture.render(gRenderer, ballCollider.x, ballCollider.y);
}

bool Ball::checkCollision(Circle& a, SDL_Rect& b)
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if (a.x < b.x)
    {
        cX = b.x;
    }
    else if(a.x > b.x +b.w)
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest  y offset
    if (a.y < b.y)
    {
        cY = b.y;
    }
    else if (a.y > b.y + b.h)
    {
        cY = b.y + b.h;
    }
    else
    {  
        cY = a.y;
    }

    //If the closest point is inside the circle
    if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
    {
        //This box and the circle have collided
        return true;
    }

    //If the shape have not collided
    return false;
}

double Ball::distanceSquared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}

/*bool Ball::checkCollision(SDL_Rect a, SDL_Rect b) {
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
}*/