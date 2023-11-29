#pragma once
#pragma warning(disable : 4996)

#include <fstream>

#include "Player.h"
#include "Ball.h"
#include "SQL_Volley.h"

class Match
{
public:
    Player player1;
    Player player2;
    Ball ball;
    unsigned int points[2] = { 0, 0 }; //TODO: asegurarnos de si debe estar aquí
    unsigned int duration = 0; // En segundos
    FILE* fp = NULL;

public:
    Match& operator=(const Match&);         //Asignación de copia
    Match();
    Match(std::string nameOne, int idOne, std::string nameTwo, int idTwo) {}
    bool save();
    void write();
    void sim();
    bool file_exists(const char* _filename);
    uint32_t file_size(const char* _filename);
    bool write_to_file(const char* _filename, const char* _line);
    bool read_from_file(const char* _filename);
    void to_string();

    void beginMatch();

    void Update(SDL_Rect& wall);
};

