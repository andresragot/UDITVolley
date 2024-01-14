#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#pragma warning(disable : 4996)

#pragma comment(lib, "ws2_32.lib")

#define MAX_PENDING					10
#define RECEIVE_BUFFER_SIZE			sizeof(Data)

using namespace std;

extern bool is_online;
extern bool is_server;

extern bool is_playing;

struct Data
{
	struct playerPosition 
	{
		int x = 0;
		int y = 0;
		int points = 0;
	}player_one, player_two;
	
	struct ballPosition
	{
		int x = 0;
		int y = 0;
	}ball;

	string to_string()
	{
		return std::to_string(player_one.x) + "|" + std::to_string(player_one.y) + "|" + std::to_string(player_two.x) + "|" + std::to_string(player_two.y) + "|" + std::to_string(ball.x) + "|" + std::to_string(ball.y) + "|" + std::to_string(player_one.points) + "|" + std::to_string(player_two.points);
	}
};

void fatal_error(char* message);
void close(SOCKET so);

void handle_client(int client_socket, int server_socket);
void handle_server(int server_socket);

void server();
void client();

void from_string(char* _buffer, Data& _data);

extern Data information;