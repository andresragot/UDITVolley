#include "SQL_Volley.h"


int SQL_Volley::db_get_table() {

    sqlite3* db = NULL;


    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    sqlite3_initialize();

    //TODO : Cambiar el open
    // Save the result of opening the file
    rc = sqlite3_open("volley_games.db", &db);

    if (rc) {
        // Show an error message
        std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return 1;
    }

    const char* sql = "SELECT * FROM games;";

    /*sql = "
        SELECT p.id, u.id as user_id, p.points, u.name
        FROM partidos p
            LEFT OUTER JOIN users u ON p.user_id = u.id
        ORDER BY p.points DESC
        ";*/

    int num_rows = 0;
    int num_cols = 0;
    char* errMsg = NULL;

    char** records = NULL;

    int res = sqlite3_get_table(db, sql, &records, &num_rows, &num_cols, &errMsg);

    if (res != SQLITE_OK) {
        sqlite3_free(errMsg);
        return res;
    }


    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            const char* key = records[j];
            const char* save = records[(i + 1) * num_cols + j];
            if (strcmp(key, "id") == 0) {
                printf("\n%s :", key);
            }
            printf("%s\t", save);
        }
    }

    sqlite3_free_table(records);
    return 0;
}


bool SQL_Volley::insert_games(sqlite3* _db, Player p1, Player p2) 
{
    if (_db == NULL) 
    {
        return false;
    }

    sqlite3_stmt* stmt = NULL;

    const char* sql = "INSERT INTO games (name_player_1, name_player_2, points_player_1, points_player_2) VALUES (?, ?, ?, ?)";

    sqlite3_prepare_v2(_db, sql, -1, &stmt, NULL);


    sqlite3_bind_text(stmt, 1, p1.name.c_str(), 1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, p2.name.c_str(), 1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, p1.points);
    sqlite3_bind_int(stmt, 4, p2.points);
    sqlite3_step(stmt);

    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);


    sqlite3_finalize(stmt);

    return true;
}

bool SQL_Volley::insert_player(sqlite3* _db, Player p)
{
    if (_db == NULL)
    {
        return false;
    }

    sqlite3_stmt* stmt = NULL;

    const char* sql = "INSERT INTO player (name_player_1) VALUES (?)";

    sqlite3_prepare_v2(_db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, p.name.c_str(), 1, SQLITE_TRANSIENT);


    return false;
}