#include "Match.h"
#include <stdlib.h>     /* srand, rand */
#pragma warning(disable : 4996)

using namespace std;


/*
	Extern variables
*/
extern std::vector<Game> games_played;

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

void Match::to_string() 
{
	cout << "Player 1 name : " << player1.name << " id: " << player1.id << " points: " << points[0] << endl;
	cout << "Player 2 name : " << player2.name << " id: " << player2.id << " points: " << points[1] << endl;
	cout << "Duration: " << duration << endl;
}

bool Match::check_player(sqlite3* db, std::string _name, bool _player)
{
	std::string _option = "";
	//If bool true = player 1


	while(player_exists(db, _name))
	{
		cout << "There is already a Player with the name: " << _name << endl;
		cout << "If you want to use that player press Y." << endl << "If you want to use another player Press N" << endl;
		cout << "Y Or N" << endl;
		cin >> _option;
		cout << endl;

		if (_option == "Y")
		{
			if (_player)
			{
				player1 = get_player(db, _name);
				return true;
			}
			else
			{
				player2 = get_player(db, _name);
				return true;
			}
		}
		else if (_option == "N")
		{
			cout << "Tell me another name please" << endl;
			cin >> _name;
			cout << endl;
		}
		else
		{
			cout << "Not available option" << endl;
		}
	}

	return false;
}

void Match::begin_match(sqlite3* db)
{
    string _name1, _name2, _option;
	system("cls");
    cout << "Tell me the name of the first player" << endl;
    cin >> _name1;
	cout << endl;
	if (!check_player(db, _name1, true))
	{
		player1 = Player(_name1, 1);
	}

	system("cls");
    cout << endl << "Tell me the name of the second player" << endl;
    cin >> _name2;
	cout << endl;
	if (!check_player(db, _name2, false))
	{
		player2 = Player(_name2, 2);
	}

	insert_player(db, player1); //Si ya están no se van a volver a guardar
	insert_player(db, player2); //Si ya están no se van a volver a guardar
}

void Match::handle_input(SDL_Event e)
{
	player1.handle_event(e);
	player2.handle_event(e);
}

void Match::update()
{
    ball.move(wall, player1, player2);
    player1.move(wall);
    player1.update();
    player2.move(wall);
    player2.update();
}

bool Match::load_points()
{
	bool success = true;
	char timer[20];
	
	sprintf(timer, "%d:%02d", (int)(duration / 60), (static_cast<int>(duration)%60));
	//cout << duration << endl;
	//cout << timer << endl;
	//Render text



	SDL_Color textColor = { 0,0,0 };
	if (!(gTextTextureOne.load_from_renderer_text(std::to_string(player1.points), textColor, gFont, gRenderer) && gTextTextureTwo.load_from_renderer_text(std::to_string(player2.points), textColor, gFont, gRenderer) && gTextTextureTimer.load_from_renderer_text(timer, textColor, gFont, gRenderer))) {
		cout << "Failed to render text texture!" << endl;
		success = false;
	}
	return success;
}

bool Match::load_media()
{
	//Loading success flag
	bool success = true;

	if (!(gBallTexture.load_from_file("dotx2.bmp", gRenderer) && gPlayerOneTexture.load_from_file("Dante.png", gRenderer) && gPlayerTwoTexture.load_from_file("Virgilio.png", gRenderer))) {
		cout << "Failed to load ball texture!" << endl;
		success = false;
	}

	//Open the font 
	gFont = TTF_OpenFont("font/Wigglye.ttf", 20);
	if (gFont == NULL) {
		cout << "Failed to load Wigglye font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	else {
		//Render text
		SDL_Color textColor = { 0,0,0 };
		if (!(gTextTextureOne.load_from_renderer_text("0", textColor, gFont, gRenderer) && gTextTextureTwo.load_from_renderer_text("0", textColor, gFont, gRenderer) && gTextTextureTimer.load_from_renderer_text("0", textColor, gFont, gRenderer))) 
		{
			cout << "Failed to render text texture!" << endl;
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
	return player1.points >= MAX_POINTS || player2.points >= MAX_POINTS;
}

bool Match::save_game(sqlite3* db)
{
	return insert_games(db, player1, player2, duration);
}

bool Match::init_match()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}
		//Create window
		gWindow = SDL_CreateWindow("UDITVolley", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else 
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1) {
					cout << "SDL_ttf could not initialize!SDL_ttf Error : " << TTF_GetError() << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

void Match::match_main(bool begin)
{
	sqlite3* db = open_table();

	if (db == nullptr)
	{
		cout << "Failed to open database" << endl;
	}

	if (begin) // If true begin game, if false resume game.
	{
		begin_match(db);
	}


	system("cls");
	//Start up SDL and create window
	if (!init_match())
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//Load media
		if (!load_media())
		{
			cout << "Failed to load media!\n" << endl;
		}
		else
		{

			Uint64 now = SDL_GetPerformanceCounter();
			Uint64 last = 0;
			double deltaTime = 0;

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			if (begin)
			{
				player1.games++;
				player2.games++;
			}

			//While application is running
			while (!quit)
			{
				last = now;
				now = SDL_GetPerformanceCounter();
				
				deltaTime = ((now - last) / (double)SDL_GetPerformanceFrequency());

				duration += deltaTime;

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						if (begin)
						{
							save_game(db);
						}
						else
						{
							update_game(db, id, player1, player2, duration);
						}
						update_player(db, player1);
						update_player(db, player2);
						quit = true;
					}
					//Handle input
					handle_input(e);
				}

				//Move the ball and check collision
				update();

				//Clear Screen
				clear();

				if (!load_points())
				{
					cout << "Failed to load points!" << endl;
				}


				//Render match
				render();

				if (win_condition()) {
					//Meter base de datos acá
					if (player1.points >= MAX_POINTS)
					{
						player1.wins++;
						cout << player1.name << " has won" << endl;
						cout << "Congratulations" << endl;
					}
					else if (player2.points >= MAX_POINTS)
					{
						player2.wins++;
						cout << player2.name << " has won" << endl;
						cout << "Congratulations" << endl;
					}
					if (begin)
					{
						save_game(db);
					}
					else
					{
						update_game(db, id, player1, player2, duration);
					}
					update_player(db, player1);
					update_player(db, player2);
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

void Match::load_match()
{
	sqlite3* db = open_table();

	games_played.clear();
	db_get_table(db);
	int res = get_games();

	Game temp = games_played[res - 1];

	id = temp.id;

	player1 = get_player(db, temp.name_player_1);
	player1.points = temp.points_player_1;

	player2 = get_player(db, temp.name_player_2);
	player2.points = temp.points_player_2;

	duration = temp.duration;
	
	sqlite3_close(db);

	match_main(false);
}

void Match::get_ranks()
{
	sqlite3* db = open_table();

	get_rankings(db);

	sqlite3_close(db);
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
