// UDITVolley.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#pragma warning(disable : 4996)

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include "Match.h"
#include "Player.h"
#include "LTexture.h"
#include "Ball.h"
#include <conio.h>

#include "sqlite3/sqlite3.h"

#include <stdio.h>

//Using SDL, SDL_image
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

//Screen dimension constants
const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 480;

Player player1("DEFAULT", 1);
Player player2("DEFAULT", 2);
Ball ball;

int menu();

void pressMenu();

void beginMatch(string& _name1, string& _name2);

void play();

//Function for main the sdl
void SDLMain();

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Loads actual value of points
bool loadPoints(std::string _pointsPlayerOne, std::string _pointsPlayerTwo);

//Frees media and shuts down SDL
void close();

static int callback(void* NotUsed, int argc, char** argv, char** azColName);

int db_get_table();

bool insert_games(sqlite3* _db);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used fonts
TTF_Font* gFont = NULL;

//Scene textures
LTexture gBallTexture, gPlayerOneTexture, gPlayerTwoTexture, gTextTextureOne, gTextTextureTwo;

// Vamos a hacer el metodo para guardar una partida DONE
// EXTRA: leer sobre sdl, descargar y aprender a compilar y vamos a pintar un rectangulo, una pelota roja y una verde
// EXTRA PLUS: hacer que salte el jugador y que se mueva y que la pelota tenga logica
int main(int argc, char* args[])
{
    play();
    //SDLMain();
    return 0;
}

//Function to get the desired option
int menu() {
    pressMenu();

    int num = 0;
    bool success = false;

    do {
        if (scanf("%i", &num) == 1) {
            if (num > 6 && num < 1) {
                printf("Invalid number, please try again");
            }
            else {
                success = true;
            }
        }
        else {
            num = 0;
        }
    } while (!success);
    return num;
}

//Function to print the menu in the console
void pressMenu() {
    system("cls");
    printf("UDIT VOLLEY\n========\n");
    printf("1. Begin Match\n");
    printf("2. Load Match\n");
    printf("3. Simulate Match\n");
    printf("4. Match in the web\n");
    printf("5. Rankings\n");
    printf("6. Leave\n");
}

//Function called to define the names of the players
void beginMatch(string& _name1, string& _name2) {
    cout << "Tell me the name of the first player" << endl;
    cin >> _name1;
    cout << endl << "Tell me the name of the first player" << endl;
    cin >> _name2;
}

//Function with the menu and does the option
void play() {
    Match match;

    string name1 = "";
    string name2 = "";

    static int idAnterior = 0;

    int option = menu();

    while (option != 6) {
        switch (option) {
        case 1:
            beginMatch(name1, name2);
            player1 = Player(name1, ++idAnterior);
            player2 = Player(name2, idAnterior + 1);
            printf("id player 1: %d, id player 2: %d", player1.id, player2.id);
            match = Match();
            idAnterior++;
            SDLMain();
            break;
        case 2:
            db_get_table();
            cout << "The match has been load" << endl;
            _getch();
            break;
        case 3:
            match.sim();
            match.save();
            match.to_string();
            cout << "The match has been saved" << endl;
            _getch();
            break;
        case 4:
            cout << "Function not developped yet" << endl;
            _getch();
            break;
        case 5:
            cout << "Function not developped yet" << endl;
            _getch();
            break;
        case 6:
            cout << "Function not developped yet" << endl;
            _getch();
            break;
        default:
            cout << "Function not developped yet" << endl;
            _getch();
            break;
        }
        option = menu();
    }
}

void SDLMain() {

    // Pointer to SQLite connection
    sqlite3* db;

    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    // Save any SQL
    std::string sql;

    sqlite3_initialize();
    // Save the result of opening the file
    rc = sqlite3_open("volley_games.db", &db);

    if (rc) {
        // Show an error message
        std::cout << "DB Error: " << sqlite3_errmsg(db) << std::endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return;
    }

    // Save SQL to create a table
    sql = "CREATE TABLE IF NOT EXISTS games ("  \
        "id    INTEGER  PRIMARY KEY NOT NULL," \
        "name_player_1 TEXT,"\
        "name_player_2 TEXT,"\
        "points_player_1 INTEGER,"\
        "points_player_2 INTEGER); ";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            SDL_Rect wall;
            wall.x = 310;
            wall.y = 280;
            wall.w = 20;
            wall.h = 200;

            //While application is running
            while (!quit)
            {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        insert_games(db);
                        quit = true;
                    }
                    //Handle input for the ball
                    player1.handleEvent(e);
                    player2.handleEvent(e);
                }

                //Move the ball and check collision
                ball.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT, player1, player2);
                player1.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT);
                player1.update();
                player2.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT);
                player2.update();

                //Clear Screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

				if (!loadPoints(std::to_string(player1.points), std::to_string(player2.points)))
                {
					printf("Failed to load points!\n");
				}

                //Render wall
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &wall);
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderDrawRect(gRenderer, &wall);

                //Render dot
                ball.render(gBallTexture, gRenderer);
                player1.render(gPlayerOneTexture, gRenderer);
                player2.render(gPlayerTwoTexture, gRenderer);
				gTextTextureOne.render(gRenderer, (SCREEN_WIDTH/5*1), 50);
                gTextTextureTwo.render(gRenderer, (SCREEN_WIDTH/5*4), 50);

                //Update screen
                SDL_RenderPresent(gRenderer);
                
                if (player1.points >= 25 || player2.points >= 25) {
                    //Meter base de datos acá
                    insert_games(db);
                    quit = true;
                }
            }
        }
    }

    // Close the SQL connection
    sqlite3_close(db);

    //Free resources and close SDL
    close();
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    if (!(gBallTexture.loadFromFile("dotx2.bmp", gRenderer)&&gPlayerOneTexture.loadFromFile("Dante.png", gRenderer)&&gPlayerTwoTexture.loadFromFile("Virgilio.png", gRenderer))) {
        printf("Failed to load ball texture!\n");
        success = false;
    }

	//Open the font 
	gFont = TTF_OpenFont("font/Wigglye.ttf", 20);
	if (gFont == NULL) {
		printf("Failed to load Wigglye font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		//Render text
		SDL_Color textColor = { 0,0,0 };
		if (!(gTextTextureOne.loadFromRendererText("0", textColor, gFont, gRenderer)&&gTextTextureTwo.loadFromRendererText("0", textColor, gFont, gRenderer))) {
			printf("Failed to render text texture!\n");
			success = false;
		}
	}


    //Load PNG surface
    /*gPNGSurface = loadSurface("06_extension_libraries_and_loading_other_image_formats/loaded.png");
    if (gPNGSurface == NULL)
    {
        printf("Failed to load PNG image!\n");
        success = false;
    }*/

    //Nothing to load
    return success;
}

bool loadPoints(std::string _pointsPlayerOne, std::string _pointsPlayerTwo) 
{
    bool success = true;
	//Render text
	SDL_Color textColor = { 0,0,0 };
    if (!(gTextTextureOne.loadFromRendererText(_pointsPlayerOne, textColor, gFont, gRenderer) && gTextTextureTwo.loadFromRendererText(_pointsPlayerTwo, textColor, gFont, gRenderer))) {
        printf("Failed to render text texture!\n");
        success = false;
    }
	return success;
}

void close()
{
    //Free loaded images
    gBallTexture.free();
    gPlayerOneTexture.free();
    gPlayerTwoTexture.free();
	gTextTextureOne.free();
	gTextTextureTwo.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

static int callback(void* data, int argc, char** argv, char** azColName) {
    int i;

    for (i = 0; i < argc; i++) {
        printf("%s = %s", azColName[i], argv[i] ? argv[i] : NULL);
    }

    return 0;
}

int db_get_table() {
   
    sqlite3* db = NULL;

   
    // Save any error messages
    char* zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    sqlite3_initialize();

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

bool insert_games(sqlite3* _db) {
    if (_db == NULL) {
        return false;
    }

    sqlite3_stmt* stmt = NULL;

    const char* sql = "INSERT INTO games (name_player_1, name_player_2, points_player_1, points_player_2) VALUES (?, ?, ?, ?)";

    sqlite3_prepare_v2(_db, sql, -1, &stmt, NULL);

    
    sqlite3_bind_text(stmt, 1, player1.name.c_str(), 1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, player2.name.c_str(), 1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, player1.points);
    sqlite3_bind_int(stmt, 4, player2.points);
    sqlite3_step(stmt);

    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
    

    sqlite3_finalize(stmt);

    return true;
}

//Clase jugador y pelota
//Atrivutos y metodos

// vamos a guardar los datos de guardar los ficheros texto

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln