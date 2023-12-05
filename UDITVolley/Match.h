#pragma once
#pragma warning(disable : 4996)

#include <fstream>

#include "Player.h"
#include "Ball.h"
#include "SQL_Volley.h"

class Match
{
public:

    Player player1 = Player("DEFAULT", 1);
    Player player2 = Player("DEFAULT", 2);
    Ball ball;
    SDL_Rect wall;
    unsigned int points[2] = { 0, 0 }; //TODO: asegurarnos de si debe estar aquí
    int duration = 0; // En segundos
    FILE* fp = NULL;

public:

    Match& operator=(const Match&);         //Asignación de copia
    Match();
    Match(std::string nameOne, int idOne, std::string nameTwo, int idTwo) {}
    bool save_game(sqlite3* db);
    //void write();
    bool file_exists(const char* _filename);
    uint32_t file_size(const char* _filename);
    bool write_to_file(const char* _filename, const char* _line);
    bool read_from_file(const char* _filename);
    void to_string();

    void check_player(sqlite3* db, std::string name, bool _player);
    void beginMatch(sqlite3* db);
    bool init_match();
    void match_main(bool begin);
    void load_match();
    void get_ranks();
    void close();

    void HandleInput(SDL_Event e);
    void Update();

    bool loadMedia();
    bool loadPoints();

    void clear();
    void render();

    bool win_condition();

private:

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The window renderer
    SDL_Renderer* gRenderer = NULL;

    //Globally used fonts
    TTF_Font* gFont = NULL;

    //Scene textures
    LTexture gBallTexture, gPlayerOneTexture, gPlayerTwoTexture, gTextTextureOne, gTextTextureTwo, gTextTextureTimer;
};

