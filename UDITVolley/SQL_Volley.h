#pragma once

#include <vector>
#include "sqlite3/sqlite3.h"
#include "Player.h"

int db_get_table(sqlite3* db);
void get_games();
bool insert_games(sqlite3* _db, Player p1, Player p2, int duration);
bool insert_player(sqlite3* _db, Player p);
static int callback(void* data, int argc, char** argv, char** azColName);
sqlite3* open_table();
void get_table_games(sqlite3* db);
void get_table_players(sqlite3* db);
bool player_exists(sqlite3* db, std::string playerName);
Player get_player(sqlite3* db, std::string playerName);
void update_player(sqlite3* db, Player p);

void get_rankings(sqlite3* db);


struct Game {
	int id						= 0;
	int points_player_1			= 0;
	int points_player_2			= 0;
	std::string name_player_1	= "";
	std::string name_player_2	= "";
	int duration				= 0;
};
