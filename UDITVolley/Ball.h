#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Player.h"
#include <iostream>

class Ball
{
private:

	//The velocity of the Ball
	int mVelX, mVelY;


public:	

	struct Circle
	{
		int x, y;
		int r;
	};

	Circle& get_collider();
	void set_position(Ball::Circle _position);

	//The dimensions of the Ball
	static const int RADIUS = 20;
	static const int Ball_WIDTH = 40;
	static const int Ball_HEIGHT = 40;

	//Maximum axis velocity of the Ball
	static const int Ball_VEL = 5;

	//Initializes the variables
	Ball();

	//Moves the Ball
	void move(SDL_Rect& wall, Player& p1, Player& p2);

	//Shows the Ball on the screen
	void render(LTexture& gBallTexture, SDL_Renderer* gRenderer);

	// De verdad me vas a leer todo para funarme? No me funes
	bool check_collision(Circle& a, SDL_Rect& b);
	double distance_squared(int x1, int y1, int x2, int y2);

private:

	Circle ballCollider;
};

