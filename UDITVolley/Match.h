#pragma once
#pragma warning(disable : 4996)

#include <fstream>

#include "Player.h"
#include "Ball.h"
#include "SQL_Volley.h"
#include "sockets.h"

class Match
{
public:

    int id = 0; // Variable solamente util verdaderamente a la hora de actualizar SQL
    Player player1 = Player("DEFAULT", 1);
    Player player2 = Player("DEFAULT", 2);
    Ball ball;
    SDL_Rect wall;
    double duration = 0; // En segundos
    FILE* fp = NULL;

public:

    Match& operator=(const Match&);         // Asignación de copia
    Match();
    Match(std::string nameOne, int idOne, std::string nameTwo, int idTwo) {}
    bool save_game(sqlite3* db);

    void to_string();

    bool check_player(sqlite3* db, std::string name, bool _player);
    void begin_match(sqlite3* db);
    void begin_match_online();
    bool init_match();
    void match_main(bool begin);
    void load_match();
    void get_ranks();
    void close();

    void handle_input(SDL_Event event_handler);
    void update();

    bool load_media();
    bool load_points();

    void clear();
    void render();

    bool win_condition();

private:

    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // The window renderer
    SDL_Renderer* gRenderer = NULL;

    // Globally used fonts
    TTF_Font* gFont = NULL;

    // Scene textures
    LTexture gBallTexture, gPlayerOneTexture, gPlayerTwoTexture, gTextTextureOne, gTextTextureTwo, gTextTextureTimer;
};

