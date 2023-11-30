#include "Match.h"
#include <stdlib.h>     /* srand, rand */
#pragma warning(disable : 4996)

using namespace std;

Match& Match::operator=(const Match& c) {
	player1.name = c.player1.name;
	player1.id = c.player1.id;
	player2.name = c.player2.name;
	player2.id = c.player2.id;
	return *this;
}

Match::Match() 
{
	wall.x = 310;
	wall.y = 280;
	wall.w = 20;
	wall.h = 200;
}

bool Match::file_exists(const char* _filename) {
	FILE* fp = fopen(_filename, "r");

	if (fp == NULL) {
		//Creates file
		fp = fopen(_filename, "a");
		fclose(fp);
		return false;
	}

	fclose(fp);

	return true;
}

uint32_t Match::file_size(const char* _filename) {
	struct stat st;
	if (stat(_filename, &st) == 0) {
		return st.st_size;
	}

	return 0;
}

bool Match::write_to_file(const char* _filename, const char* _line) {
	FILE* fp = NULL;

	if (file_exists(_filename)) {
		fp = fopen(_filename, "w");
		fputs(_line, fp);
		fclose(fp);
		return true;
	}

	return false;
}

bool Match::read_from_file(const char* _filename) 
{
    return false;
}

void Match::to_string() 
{
	cout << "Player 1 name : " << player1.name << " id: " << player1.id << " points: " << points[0] << endl;
	cout << "Player 2 name : " << player2.name << " id: " << player2.id << " points: " << points[1] << endl;
	cout << "Duration: " << duration << endl;
}

void Match::beginMatch()
{
    string _name1, _name2;
    cout << "Tell me the name of the first player" << endl;
    cin >> _name1;
    cout << endl << "Tell me the name of the first player" << endl;
    cin >> _name2;

    player1 = Player(_name1, 0);
    player2 = Player(_name2, 1);
}

void Match::HandleInput(SDL_Event e)
{
	player1.handleEvent(e);
	player2.handleEvent(e);
}

void Match::Update()
{
    ball.move(wall, player1, player2);
    player1.move(wall);
    player1.update();
    player2.move(wall);
    player2.update();
}

bool Match::loadPoints()
{
	bool success = true;
	char timer[6] = "00:00";
	
	sprintf(timer, "%i:%i", (duration / 60), (duration % 60));
	//Render text
	SDL_Color textColor = { 0,0,0 };
	if (!(gTextTextureOne.loadFromRendererText(std::to_string(player1.points), textColor, gFont, gRenderer) && gTextTextureTwo.loadFromRendererText(std::to_string(player2.points), textColor, gFont, gRenderer) && gTextTextureTimer.loadFromRendererText(timer, textColor,gFont, gRenderer))) {
		printf("Failed to render text texture!\n");
		success = false;
	}
	return success;
}

bool Match::loadMedia()
{
	//Loading success flag
	bool success = true;

	if (!(gBallTexture.loadFromFile("dotx2.bmp", gRenderer) && gPlayerOneTexture.loadFromFile("Dante.png", gRenderer) && gPlayerTwoTexture.loadFromFile("Virgilio.png", gRenderer))) {
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
		if (!(gTextTextureOne.loadFromRendererText("0", textColor, gFont, gRenderer) && gTextTextureTwo.loadFromRendererText("0", textColor, gFont, gRenderer) && gTextTextureTimer.loadFromRendererText("0", textColor, gFont, gRenderer))) 
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	//Nothing left to load
	return success;
}

void Match::clear()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
}

void Match::render()
{
	ball.render(gBallTexture, gRenderer);
	player1.render(gPlayerOneTexture, gRenderer);
	player2.render(gPlayerTwoTexture, gRenderer);
	gTextTextureOne.render(gRenderer, (SCREEN_WIDTH / 5 * 1), 50);
	gTextTextureTwo.render(gRenderer, (SCREEN_WIDTH / 5 * 4), 50);
	gTextTextureTimer.render(gRenderer, (SCREEN_WIDTH / 2), 50);

	//Render wall
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &wall);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &wall);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

bool Match::win_condition()
{
	return player1.points >= 25 || player2.points >= 25;
}

bool Match::save_game(sqlite3* db)
{
	return insert_games(db, player1, player2);
}

bool Match::init_match()
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

void Match::match_main()
{
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
	if (!init_match())
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

			Uint64 now = SDL_GetPerformanceCounter();
			Uint64 last = 0;
			int deltaTime = 0;

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				last = now;
				now = SDL_GetPerformanceCounter();
				
				deltaTime = (int)((now - last) / (int)SDL_GetPerformanceFrequency());

				duration += deltaTime;

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						save_game(db);
						quit = true;
					}
					//Handle input
					HandleInput(e);
				}

				//Move the ball and check collision
				Update();

				//Clear Screen
				clear();

				if (!loadPoints())
				{
					printf("Failed to load points!\n");
				}


				//Render match
				render();

				if (win_condition()) {
					//Meter base de datos acá
					save_game(db);
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

void Match::close()
{
	//Free loaded images
	gBallTexture.free();
	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();
	gTextTextureOne.free();
	gTextTextureTwo.free();
	gTextTextureTimer.free();

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

int Match::callback(void* data, int argc, char** argv, char** azColName)
{
	int i;

	for (i = 0; i < argc; i++) {
		printf("%s = %s", azColName[i], argv[i] ? argv[i] : NULL);
	}

	return 0;
}