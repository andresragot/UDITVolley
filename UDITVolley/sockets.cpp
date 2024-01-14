#include <cstring> 
#include <thread>
#include "sockets.h"


/*
    GLOBAL VARS
*/
Data information;
bool is_playing;
bool quit;

/*
    EXTERN VARS
*/


/*
    FUNCTIONS
*/

void fatal_error(char* message)
{
    fprintf(stderr, "ERROR %d: %s\n", WSAGetLastError(), message);
    exit(1);
    return;
}

void close(SOCKET so)
{
    closesocket(so);
    WSACleanup();
    return;
}

void handle_client(int client_socket, int server_socket)
{
    char buffer[RECEIVE_BUFFER_SIZE];
    char message[RECEIVE_BUFFER_SIZE] = "Welcome to the server!\n";
    int receive_message_size;
    Data information_recv;

    send(client_socket, message, strlen(message) + 1, 0);

    while (is_playing && strcmp(buffer, "QUIT") != 0)
    {
        if ((receive_message_size = recv(client_socket, buffer, RECEIVE_BUFFER_SIZE, 0)) < 0)
        {
            fatal_error((char*)"Error receiving data.\n");
        }

        buffer[receive_message_size] = '\0';
        // Debug
        std::cout << "[Client]: " << buffer << endl;

        //std::cout << information.player_two.name;

        from_string(buffer, information_recv);
        //std::cout << information_recv.to_string() << endl;

        information.player_two.x = information_recv.player_two.x;
        information.player_two.y = information_recv.player_two.y;


        
        if (strcmp(buffer, "QUIT") == 0)
        {
            quit = true;
        }

        if (is_playing)
        {
            strcpy(message, information.to_string().c_str());
        }
        else
        {
            strcpy(message, "QUIT");
        }


        send(client_socket, message, strlen(message) + 1, 0);
    }
    
    std::cout << "Closing the connection..." << endl;
    closesocket(client_socket);
    is_online = false;

    close(server_socket);
}

void handle_server(int server_socket)
{
    char message[RECEIVE_BUFFER_SIZE];
    char buffer[RECEIVE_BUFFER_SIZE];
    int bytes_received;
    Data information_recv;

    while (is_playing && strcmp(buffer, "QUIT") != 0)
    {
        if ((bytes_received = recv(server_socket, buffer, RECEIVE_BUFFER_SIZE, 0)) == SOCKET_ERROR)
            fatal_error((char*)"Error receiving data");

        buffer[bytes_received] = '\0';
        // Debug
        std::cout << "[Server]: " << buffer << endl;


        from_string(buffer, information_recv);
        std::cout << information_recv.to_string() << endl;

        information.player_one.x = information_recv.player_one.x;
        information.player_one.y = information_recv.player_one.y;

        information.ball.x = information_recv.ball.x;
        information.ball.y = information_recv.ball.y;

        information.player_one.points = information_recv.player_one.points;
        information.player_two.points = information_recv.player_two.points;

        /*scanf("%s", message);
        printf("[Client]: %s\n", message);*/

        //message = information.to_string().c_str();

        if (strcmp(buffer, "QUIT") == 0)
        {
            quit = true;
        }

        if (is_playing)
        {
            strcpy(message, information.to_string().c_str());
        }
        else
        {
            strcpy(message, "QUIT");
        }
        
        send(server_socket, message, (size_t)(strlen(message) + 1), 0);
    }
    std::cout << "Closing the connection..." << endl;
    closesocket(server_socket);
    is_online = false;
    WSACleanup();
}

void server()
{
    system("cls");
    is_server = true;
    SOCKET server_so, client_so;
    WSADATA wsa;
    string ip = "";
    int bytes_received;
    int client_length;
    unsigned short port;
    struct sockaddr_in server, client;

    std::cout << "To what IP do you want to connect" << endl;

    if (!(cin >> ip))
    {
        std::cout << "Error retreiving IP";
        return;
    }

    std::cout << "Port you want to connect to" << endl;
    if (!(cin >> port))
    {
        std::cout << "Error retreving Port" << endl;
    }


    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        fatal_error((char*)"WSAStartup failed.\n");

    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
    {
        fatal_error((char*)"Not able to find the right version of Winsock (2.2).\n");
    }
    else
    {
        std::cout << "The libray Winsock 2.2 has been initialized" << endl;
    }

    if ((server_so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        fatal_error((char*)"Error creating socket.\n");

    std::cout << "Socket created" << endl;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    if (bind(server_so, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        fatal_error((char*)"Bind failed.\n");

    if (listen(server_so, MAX_PENDING) < 0)
        fatal_error((char*)"Listen failed.\n");

    std::cout << "Waiting entering connections" << endl;

    is_online = true;

    do {
        client_length = sizeof(client);

        if ((client_so = accept(server_so, (struct sockaddr*)&client, &client_length)) < 0)
            fatal_error((char*)"Error in accept.\n");

        std::cout << "New conecction accepted " << inet_ntoa(client.sin_addr) << endl;

    } while (!client_so);

    is_playing = true;
    std::thread server_thread(handle_client, client_so, server_so);
    server_thread.detach();
}

void client()
{
    system("cls");
    SOCKET so;
    WSADATA wsa;
    string server_ip = "";
    unsigned short server_port;
    struct sockaddr_in server;

    std::cout << "To what IP do you want to connect" << endl;
    
    if (!(cin >> server_ip))
    {
        std::cout << "Error retreiving IP";
        return;
    }

    std::cout << "Port you want to connect to" << endl;
    if (!(cin >> server_port))
    {
        std::cout << "Error retreving Port" << endl;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        fatal_error((char*)"WSAStartup fallido");

    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
    {
        fatal_error((char*)"Not able to find the right version of Winsock (2.2).\n");
    }
    else
    {
        std::cout << "The libray Winsock 2.2 has been initialized" << endl;
    }

    if ((so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        fatal_error((char*)"Error creating socket");

    std::cout << "Socket created" << endl;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);

    if (connect(so, (struct sockaddr*)&server, sizeof server) < 0)
        fatal_error((char*)("Not able to connect to the server %s:%d.", server_ip, server_port));

    std::cout << "Connecting to " << server_ip << ":" << server_port << endl;

    is_online = true;
    is_playing = true;
    std::thread client_thread(handle_server, so);    
    client_thread.detach();
}

void from_string(char* _buffer, Data& _data)
{
    sscanf(_buffer, "%d|%d|%d|%d|%d|%d|%d|%d", &_data.player_one.x, &_data.player_one.y,&_data.player_two.x, &_data.player_two.y, &_data.ball.x, &_data.ball.y, &_data.player_one.points, &_data.player_two.points);
}
