// UDITVolley.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#pragma warning(disable : 4996)

#include <conio.h>
#include <limits>
#include "Match.h"


using namespace std;

int menu();

void pressMenu();

void play();

Match match;


/* 
    Buenos días Javi, bienvenido a mi querido código 
    Por favor, no me funes.
*/
int main(int argc, char* args[])
{
    play();
    return 0;
}

// Function to get the desired option
int menu() 
{
    pressMenu();

    int num = 0;
    bool success = false;

    do 
    {
        
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
            cin.ignore(10000, '\n'); //Descarta la entrada incorrecta
        }
    } while (!success);
    return num;
}

//Function to print the menu in the console
void pressMenu() 
{
    system("cls");
    cout << "UDIT VOLLEY\n========" << endl;
    cout << "1. Begin Match" << endl;
    cout << "2. Load Match" << endl;
    cout << "3. Match in the web" << endl;
    cout << "4. Rankings" << endl;
    cout << "5. Leave" << endl;
}

//Function with the menu and does the option
void play() 
{

    int option = menu();

    while (option != 5) 
    {
        switch (option) 
        {
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
            match.begin_match_online();
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