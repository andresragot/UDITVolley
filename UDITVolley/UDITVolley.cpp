// UDITVolley.cpp : Este archivo contiene la funci�n "main". La ejecuci�n del programa comienza y termina ah�.
//
#pragma warning(disable : 4996)

#include "Match.h"
#include <conio.h>


using namespace std;

int menu();

void pressMenu();

void play();

Match match;


/* 
    Buenos d�as Javi, bienvenido a mi querido c�digo 
    Por favor, no me funes.
*/
int main(int argc, char* args[])
{
    play();
    return 0;
}

//Function to get the desired option
int menu() {
    pressMenu();

    int num = 0;
    bool success = false;

    do {
        
        if ((cin >> num))
        {
            if (num > 6 && num < 1) 
            {
                cout << "Invalid number, please try again" << endl;
            }
            else 
            {
                success = true;
            }
        }
        else
        {
            num = 0;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Descarta la entrada incorrecta
        }
    } while (!success);
    return num;
}

//Function to print the menu in the console
void pressMenu() {
    system("cls");
    cout << "UDIT VOLLEY\n========" << endl;
    cout << "1. Begin Match" << endl;
    cout << "2. Load Match" << endl;
    cout << "3. Match in the web" << endl;
    cout << "4. Rankings" << endl;
    cout << "5. Leave" << endl;
}

//Function with the menu and does the option
void play() {

    int option = menu();

    while (option != 5) {
        switch (option) {
        case 1:
            match.match_main(true);
            break;
        case 2:
            //db_get_table();
            match.load_match();
            cout << "The match has been load" << endl;
            _getch();
            break;
        case 3:
            cout << "Function not developped yet" << endl;
            _getch();
            break;
        case 4:
            match.get_ranks();
            _getch();
            break;
        case 5:
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

// Ejecutar programa: Ctrl + F5 o men� Depurar > Iniciar sin depurar
// Depurar programa: F5 o men� Depurar > Iniciar depuraci�n

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de c�digo fuente
//   3. Use la ventana de salida para ver la salida de compilaci�n y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de c�digo, o a Proyecto > Agregar elemento existente para agregar archivos de c�digo existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln