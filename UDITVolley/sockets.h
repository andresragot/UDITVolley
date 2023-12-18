#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#pragma warning(disable : 4996)

#pragma comment(lib, "ws2_32.lib")

#define MAX_PENDING 10
#define RECEIVE_BUFFER_SIZE 32

using namespace std;

extern bool is_online;
extern bool is_server;

void fatal_error(char* message);
void close(SOCKET so);

void handle_client(int client_socket);

void server();
void client();

struct Data
{
	struct playerPosition 
	{
		int x = 0;
		int y = 0;
	}player;
	struct ballPosition
	{
		int x = 0;
		int y = 0;
	}ball;
	
	int playerPoints = 0;

	string to_string()
	{
		return std::to_string(player.x) + "|" + std::to_string(player.y) + "|" + std::to_string(ball.x) + "|" + std::to_string(ball.y) + "|" + std::to_string(playerPoints);
	}
};

extern Data information;