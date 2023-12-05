#include "Ball.h"

Ball::Circle& Ball::get_collider()
{
    return ballCollider;
}

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
    if ((ballCollider.x - ballCollider.r <= 0))
    {
        //Move back
        mVelX *= -1;

        ballCollider.x = ballCollider.r + 5; //offset just in case
    }
    else if (ballCollider.x + ballCollider.r >= SCREEN_WIDTH)
    {
        //Move back
        mVelX *= -1;

        ballCollider.x = SCREEN_WIDTH - ballCollider.r - 5; //offset just in case
    }

    //If the Ball went too far up or down
    if ((ballCollider.y - ballCollider.r <= 0))
    {
        //Move back
        mVelY *= -1;
    }

    //Ball touched the floor
    if ((ballCollider.y + ballCollider.r >= SCREEN_HEIGHT)) 
    {
        if (ballCollider.x + ballCollider.r < SCREEN_WIDTH / 2 ) 
        {
            p2.points++;
            ballCollider.x = 300;
            ballCollider.y = 50;
        }
        else 
        {
            p1.points++;
            ballCollider.x = 300;
            ballCollider.y = 50;
        }
    }


    check_collision(ballCollider, wall);
    check_collision(ballCollider, p1.pCollider);
    check_collision(ballCollider, p2.pCollider);


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

bool Ball::check_collision(Circle& a, SDL_Rect& b)
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if (a.x < b.x)
    {
        cX = b.x;
    }
    else if (a.x > b.x +b.w)
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
    if (distance_squared(a.x, a.y, cX, cY) < a.r * a.r)
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
            mVelY *= -1;
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
            mVelX *= -1;
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

double Ball::distance_squared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}