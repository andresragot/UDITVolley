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

Match::Match() {
	
}

//Save function
bool Match::save() {
	char* buffer = NULL;
	buffer = (char*)malloc(128 * sizeof(char));

	sprintf(buffer, "%i,%i,%s;%s;%i;%i;%i", player1.id, player2.id, player1.name.c_str(), player2.name.c_str(), points[0], points[1], duration);

	if (buffer == NULL) {
		return false;
	}

	if (write_to_file("game.txt", buffer)) {
		free(buffer);
		buffer = NULL;
		return true;
	}

	cout << "File does not exist" << endl;

	return false;
}

//
void Match::write() {
	char* buffer = NULL;
	if (fp) {
		while (fgets(buffer, sizeof(buffer), fp)) {
			printf("%s", buffer);
		}
	}
}

void Match::sim() {

	points[0] = rand() % 26;
	points[1] = rand() % 26;

	duration = rand() % 256;
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

void Match::Update(SDL_Rect& wall)
{
    ball.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT, player1, player2);
    player1.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT);
    player1.update();
    player2.move(wall, SCREEN_WIDTH, SCREEN_HEIGHT);
    player2.update();
}