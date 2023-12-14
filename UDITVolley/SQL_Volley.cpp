#include "SQL_Volley.h"
#include <conio.h>

/*
    GLOBAL VARS
*/
std::vector<Game> games_played;

/*
    FUNCTIONS
*/

int db_get_table(sqlite3* db) 
{

    if (db == nullptr)
    {
        std::cout << "Pointer is null" << std::endl;
        return 1;
    }

    std::string sql = "SELECT * FROM games WHERE points_player_1 < " + std::to_string(MAX_POINTS) + " AND points_player_2 < " + std::to_string(MAX_POINTS) + ";";

    int num_rows = 0;
    int num_cols = 0;
    char* errMsg = NULL;

    char** records = NULL;

    int res = sqlite3_get_table(db, sql.c_str(), &records, &num_rows, &num_cols, &errMsg);

    if (res != SQLITE_OK) 
    {
        sqlite3_free(errMsg);
        return res;
    }


    for (int i = 0; i < num_rows; i++) 
    {
        Game game;
        for (int j = 0; j < num_cols; j++) 
        {
            const char* key = records[j];
            const char* save = records[(i + 1) * num_cols + j];
            if (strcmp(key, "id") == 0)
            {
                game.id = std::stoi(save);
            }
            else if (strcmp(key, "name_player_1") == 0)
            {
                game.name_player_1 = save;
            }
            else if (strcmp(key, "name_player_2") == 0)
            {
                game.name_player_2 = save;
            }
            else if (strcmp(key, "points_player_1") == 0)
            {
                game.points_player_1 = std::stoi(save);
            }
            else if (strcmp(key, "points_player_2") == 0)
            {
                game.points_player_2 = std::stoi(save);
            }
            else if (strcmp(key, "duration") == 0)
            {
                game.duration = std::stoi(save);
            }
        }
        games_played.push_back(game);
    }

    sqlite3_free_table(records);
    return 0;
}

int get_games()
{
    std::cout << "Select Match to load" << std::endl;

    int i = 1;
    for (const auto& val : games_played)
    {
        std::cout << i << " " << val.name_player_1 << ": " << val.points_player_1 << "\t VS " << val.name_player_2 << ": " << val.points_player_2 << "\tduration: " << val.duration << std::endl;
        i++;
    }

    bool success = false;
    int num = 0;

    do 
    {
        if ((std::cin >> num))
        {
            if (num > games_played.size() || num < 1)
            {
                std::cout << "Invalid number, please try again" << std::endl;
            }
            else
            {
                success = true;
            }
        }
        else
        {
            num = 0;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Descarta la entrada incorrecta
            std::cout << "Invalid option, please write a number from 1 to " << games_played.size() << std::endl;
        }
    } while (!success);

    return num;
}


bool insert_games(sqlite3* _db, Player _p1, Player _p2, int _duration) 
{
    if (_db == NULL) 
    {
        return false;
    }

    sqlite3_stmt* stmt = NULL;

    std::string sql = "INSERT INTO games (name_player_1, name_player_2, points_player_1, points_player_2, duration) VALUES (?, ?, ?, ?, ?)";

    if(sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, _p1.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, _p2.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, _p1.points);
        sqlite3_bind_int(stmt, 4, _p2.points);
        sqlite3_bind_int(stmt, 5, _duration);

        int result = sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);


        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error inserting game" << std::endl;
            return false;
        }

        return true;
    }
    else 
    {
        std::cout << "Error declaring SQL" << std::endl;
        return false;
    }
}

bool update_game(sqlite3* _db, int _id, Player _p1, Player _p2, int _duration)
{
    if (_db == nullptr)
    {
        std::cout << "Pointer to database is null" << std::endl;
        return false;
    }

    sqlite3_stmt* stmt;

    std::string sql = "UPDATE games SET points_player_1 = ?, points_player_2 = ?, duration = ? WHERE id = ?";

    if (sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, _p1.points);
        sqlite3_bind_int(stmt, 2, _p2.points);
        sqlite3_bind_int(stmt, 3, _duration);
        sqlite3_bind_int(stmt, 4, _id);

        int result = sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);

        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error updating match" << std::endl;
            return false;
        }

        return true;
    }
    else
    {
        std::cout << "Error preparing declaration" << std::endl;
    }

    return false;
}

bool insert_player(sqlite3* _db, Player _p)
{
    if (_db == NULL)
    {
        return false;
    }

    sqlite3_stmt* stmt = NULL;

    std::string sql = "INSERT INTO players(name, games, wins) VALUES (?, ?, ?)";

    if (sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, _p.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, _p.games);
        sqlite3_bind_int(stmt, 3, _p.wins);
        int result = sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);


        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error inserting player" << std::endl;
            return false;
        }

        return true;
    }
    else
    {
        std::cout << "Error declaring SQL" << std::endl;
        return false;
    }
}

int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;

    for (i = 0; i < argc; i++) 
    {
        printf("%s = %s", azColName[i], argv[i] ? argv[i] : NULL);
    }

    return 0;
}

sqlite3* open_table()
{
    sqlite3* db = NULL;

    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    sqlite3_initialize();

    // Save the result of opening the file
    rc = sqlite3_open("volley_games.db", &db);

    if (rc) 
    {
        // Show an error message
        std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return nullptr;
    }

    get_table_games(db);
    get_table_players(db);

    return db;
}

void get_table_games(sqlite3* db)
{
    if (db == nullptr)
    {
        std::cout << "Pointer to database is null" << std::endl;
        return;
    }

    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    // Save any SQL
    std::string sql;

    // Save SQL to create a table
    sql = "CREATE TABLE IF NOT EXISTS games ("  \
        "id    INTEGER  PRIMARY KEY NOT NULL," \
        "name_player_1 TEXT,"\
        "name_player_2 TEXT,"\
        "points_player_1 INTEGER,"\
        "points_player_2 INTEGER,"\
        "duration INTEGER); ";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
}

void get_table_players(sqlite3* db)
{
    if (db == nullptr)
    {
        std::cout << "Pointer to database is null" << std::endl;
        return;
    }

    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    // Save any SQL
    std::string sql;

    // Save SQL to create a table
    sql = "CREATE TABLE IF NOT EXISTS players ("\
        "name TEXT PRIMARY KEY NOT NULL,"\
        "games INTEGER,"\
        "wins INTEGER); ";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
}

bool player_exists(sqlite3* db, std::string playerName)
{
    if (db == nullptr)
    {
        std::cout << "Pointer to database is null" << std::endl;
        return false;
    }

    sqlite3_stmt* stmt;
    std::string sql = "SELECT name FROM players WHERE name = ?";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (result == SQLITE_ROW)
        {
            return true;
        }
    }

    return false;

}

Player get_player(sqlite3* db, std::string playerName)
{
    if (db == nullptr)
    {
        std::cout << "Pointer is null" << std::endl;
        return Player();
    }

    Player temp;

    sqlite3_stmt* stmt;
    std::string sql = "SELECT * FROM players WHERE name = ?";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

        int result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {

            temp.name = std::string(reinterpret_cast <const char*> (sqlite3_column_text(stmt, 0)));
            temp.games = sqlite3_column_int(stmt, 1);
            temp.wins = sqlite3_column_int(stmt, 2);

            sqlite3_finalize(stmt);
            return temp;

        }
        sqlite3_finalize(stmt);
    }

    return Player();
}

void update_player(sqlite3* _db, Player _p)
{
    if (_db == nullptr)
    {
        std::cout << "Pointer to database is null" << std::endl;
        return;
    }

    sqlite3_stmt* stmt;

    std::string sql = "UPDATE players SET games = ?, wins = ? WHERE name = ?";

    if (sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, _p.games);
        sqlite3_bind_int(stmt, 2, _p.wins);
        sqlite3_bind_text(stmt, 3, _p.name.c_str(), -1, SQLITE_STATIC);

        int result = sqlite3_step(stmt);

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);

        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error updating player" << std::endl;
        }
    }
    else
    {
        std::cout << "Error preparing declaration" << std::endl;
    }
    
}

void get_rankings(sqlite3* db)
{
    std::string sql = "SELECT * FROM players ORDER BY wins DESC";

    int num_rows = 0;
    int num_cols = 0;
    char* errMsg = NULL;

    char** records = NULL;

    int res = sqlite3_get_table(db, sql.c_str(), &records, &num_rows, &num_cols, &errMsg);

    if (res != SQLITE_OK)
    {
        sqlite3_free(errMsg);
        return;
    }

    std::cout << "Ranking" << std::endl;

    //Imprimir los nombres de las columnas
    for (int i = 0; i < num_cols; i++)
    {
        std::cout << "\t" << records[i];
    }
    std::cout << std::endl;

    for (int i = 0; i < num_rows; i++) 
    {
        std::cout << i + 1 << "\t";
        for (int j = 0; j < num_cols; j++) 
        {
            const char* save = records[(i + 1) * num_cols + j];
            std::cout << save << "\t";
        }
        std::cout << std::endl;
    }

    sqlite3_free_table(records);
    return;

}
