#pragma once
#include "sqlite3/sqlite3.h
#include "Player.h"

class SQL_Volley
{
public:

    int db_get_table();
    bool insert_games(sqlite3* _db, Player p1, Player p2);
    bool insert_player(sqlite3* _db, Player p);
};

