// UDITVolley.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#pragma warning(disable : 4996)

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <conio.h>
#include <stdio.h>

//Using SDL, SDL_image
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Match.h"
#include "Player.h"
#include "LTexture.h"
#include "Ball.h"
#include "SQL_Volley.h"

using namespace std;

int menu();

void pressMenu();

void play();

Match match;


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

//Function with the menu and does the option
void play() {

    int option = menu();

    while (option != 6) {
        switch (option) {
        case 1:
            match.beginMatch();
            match.match_main();
            break;
        case 2:
            db_get_table();
            cout << "The match has been load" << endl;
            _getch();
            break;
        case 3:
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