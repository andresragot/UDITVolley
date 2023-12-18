#include <cstring> 
#include "sockets.h"


/*
    GLOBAL VARS
*/
Data information;
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

void handle_client(int client_socket)
{
    char buffer[RECEIVE_BUFFER_SIZE];
    char message[200] = "Welcome to the server!\n";
    int receive_message_size;

    send(client_socket, message, strlen(message) + 1, 0);

    while (strcmp(message, "QUIT") != 0)
    {
        if ((receive_message_size = recv(client_socket, buffer, RECEIVE_BUFFER_SIZE, 0)) < 0)
            fatal_error((char*)"Error receiving data.\n");

        buffer[receive_message_size] = '\0';
        //cout << "[Client]: " << buffer << endl;

        /*scanf("%s", message);
        printf("[Server]: %s\n", message);*/
        
        strcpy(message, information.to_string().c_str());
        send(client_socket, message, strlen(message) + 1, 0);
    }
     
    cout << "Closing the connection..." << endl;
    closesocket(client_socket);
    is_online = false;
}

void server()
{
    is_server = true;
    SOCKET server_so, client_so;
    WSADATA wsa;
    string ip = "";
    int bytes_received;
    int client_length;
    unsigned short port;
    struct sockaddr_in server, client;

    cout << "To what IP do you want to connect" << endl;

    if (!(cin >> ip))
    {
        cout << "Error retreiving IP";
        return;
    }

    cout << "Port you want to connect to" << endl;
    if (!(cin >> port))
    {
        cout << "Error retreving Port" << endl;
    }


    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        fatal_error((char*)"WSAStartup failed.\n");

    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
    {
        fatal_error((char*)"Not able to find the right version of Winsock (2.2).\n");
    }
    else
    {
        cout << "The libray Winsock 2.2 has been initialized" << endl;
    }

    if ((server_so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        fatal_error((char*)"Error creating socket.\n");

    cout << "Socket created" << endl;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    if (bind(server_so, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        fatal_error((char*)"Bind failed.\n");

    if (listen(server_so, MAX_PENDING) < 0)
        fatal_error((char*)"Listen failed.\n");

    cout << "Waiting entering connections" << endl;

    is_online = true;

    while (1)
    {
        client_length = sizeof(client);

        if ((client_so = accept(server_so, (struct sockaddr*)&client, &client_length)) < 0)
            fatal_error((char*)"Error in accept.\n");

        cout << "New conecction accepted " << inet_ntoa(client.sin_addr) << endl;
        handle_client(client_so);
    }

    close(server_so);
    is_online = false;
    return;
}

void client()
{
    SOCKET so;
    WSADATA wsa;
    string server_ip = "";
    char message[200];
    char buffer[RECEIVE_BUFFER_SIZE];
    unsigned short server_port;
    int bytes_received;
    struct sockaddr_in server;

    cout << "To what IP do you want to connect" << endl;
    
    if (!(cin >> server_ip))
    {
        cout << "Error retreiving IP";
        return;
    }

    cout << "Port you want to connect to" << endl;
    if (!(cin >> server_port))
    {
        cout << "Error retreving Port" << endl;
    }



    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        fatal_error((char*)"WSAStartup fallido");

    if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
    {
        fatal_error((char*)"Not able to find the right version of Winsock (2.2).\n");
    }
    else
    {
        cout << "The libray Winsock 2.2 has been initialized" << endl;
    }

    if ((so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        fatal_error((char*)"Error creating socket");

    cout << "Socket created" << endl;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);

    if (connect(so, (struct sockaddr*)&server, sizeof server) < 0)
        fatal_error((char*)("Not able to connect to the server %s:%d.", server_ip, server_port));

    cout << "Connecting to " << server_ip << ":" << server_port << endl;

    is_online = true;

    while (strcmp(message, "QUIT") != 0)
    {
        if ((bytes_received = recv(so, buffer, RECEIVE_BUFFER_SIZE, 0)) == SOCKET_ERROR)
            fatal_error((char*)"Error receiving data");

        buffer[bytes_received] = '\0';
        //cout << "[Server]: " << buffer << endl;


        /*scanf("%s", message);
        printf("[Client]: %s\n", message);*/

        //message = information.to_string().c_str();

        strcpy(message, information.to_string().c_str());

        send(so, message, (size_t)(strlen(message) + 1), 0);
    }

    closesocket(so);
    is_online = false;
    WSACleanup();
}