#pragma once

#include <string.h>
#include <iostream>
#include <SDL.h>
#include "LTexture.h"


class Player
{

public:
    unsigned int id = 0;
    std::string name = "";
    short x = 0;
    short y = 0;
    float mVelX = 0;
    float mVelY = 0;
    unsigned int points = 0;
    unsigned int games = 0;
    unsigned int wins = 0;

    const float GRAVITY = 0.5f;
    const float JUMP_SPEED = 12.f;
    const float mSpeed = 5;
    const int PLAYER_HEIGHT = 128;
    const int PLAYER_WIDTH = 45;

    bool canJump = true;

    SDL_Rect pCollider;

public:
    Player& operator=(const Player&);         //Asignación de copia

    Player();
    Player(std::string n, int i) : name(n), id(i) { pCollider.h = PLAYER_HEIGHT; pCollider.w = PLAYER_WIDTH; mVelX = 0; mVelY = 0; if (id % 2) { x = 100; y = 280; } else { x = 400; y = 280; } }

    //Takes key presses and adjusts the Ball's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Ball
	void move(SDL_Rect& wall, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	//Shows the Ball on the screen
	void render(LTexture& gBallTexture, SDL_Renderer* gRenderer);

    bool checkCollision(SDL_Rect a, SDL_Rect b);
    
    void update();
    //    void Move();
    //    void Jump();
};

