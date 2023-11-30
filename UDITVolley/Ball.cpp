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

void Ball::move(SDL_Rect& wall, Player& p1, Player& p2)
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


    if (checkCollision(ballCollider, wall))
    {
        // If ball is moving downwards and hits top of collider
        if (mVelY > 0 && ballCollider.y + ballCollider.r >= wall.y)
        {
            mVelY *= -1;
        }
        //If ball is moving upwards and hits bottom of collider
        else if (mVelY < 0 && ballCollider.y - ballCollider.r <= wall.y + wall.h)
        {
            mVelY *= -1;
        }
        // if ball hits sides
        else
        {
            mVelX *= -1;
        }
    }
    checkCollision(ballCollider, p1.pCollider);
    checkCollision(ballCollider, p2.pCollider);


    //Move the Ball left or right
    ballCollider.x += mVelX;

    //Move the Ball up or down
    ballCollider.y += mVelY;

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
        //Calculate the difference in the x and y positions
        int diffX = a.x - cX;
        int diffY = a.y - cY;

        //If the absolute difference in x is less than the absolute difference in y
        //Then the collision happened from the top or bottom
        if (abs(diffX) < abs(diffY))
        {
            //If diffY is less than 0, the collision happened from the top
            //Otherwise, it happened from the bottom.
            //Position correction
            if (diffY < 0)
            {
                ballCollider.y = b.y - ballCollider.r;
            }
            else
            {
                ballCollider.y = b.y + b.h + ballCollider.r;
            }
        }
        else
        {
            //Otherwise, the collision happened from the left or 
            //Position correction
            if (diffX < 0)
            {
                ballCollider.x = b.x - ballCollider.r;
            }
            else
            {
                ballCollider.x = b.x + b.w + ballCollider.r;
            }
        }

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