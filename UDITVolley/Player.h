#pragma once

#include <string.h>
#include <iostream>
#include <SDL.h>

#include "sockets.h"
#include "LTexture.h"


class Player
{

public:
    unsigned int id = 0;
    std::string name = "";
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
    Player& operator=(const Player&);         // Asignación de copia

    Player();
    Player(std::string n, int i) : name(n), id(i) { pCollider.h = PLAYER_HEIGHT; pCollider.w = PLAYER_WIDTH; mVelX = 0; mVelY = 0; if (id % 2) { pCollider.x = 100; pCollider.y = 280; } else { pCollider.x = 400; pCollider.y = 280; } }

    SDL_Rect& get_collider();
    void set_collider(SDL_Rect _position);

    // Takes key presses and adjusts the Player's velocity
	void handle_event(SDL_Event& event_handler);

	// Moves the Player
	void move(SDL_Rect& wall);

	// Shows the Player on the screen
	void render(LTexture& gBallTexture, SDL_Renderer* gRenderer);

    // Viste que te tengo los nombres como te gustan
    bool check_collision(SDL_Rect a, SDL_Rect b);
    
    void update();
};

