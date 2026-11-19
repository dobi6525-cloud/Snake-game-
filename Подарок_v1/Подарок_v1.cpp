#include <iostream>
#include <conio.h>
#include <cstdlib>
#include "Network_Client.h"
#include "Network_Server.h"
#include "Snake.h"

using namespace std;

//void TestMessage() {
//    Message msg(2);
//
//}


static int InformationGameSingle() {
    system("cls");
    Snake snake;
    snake.Play();

    return 3;
}

static int InformationGameMultiUser() {
    uint16_t ServerOrClient;
    system("cls");
    std::cout << "Создаем сервер >>> 1\nПодключаемся >>> 2\nВернуться >>> 3\nВыйти >>> 4\n>>> ";
    do {
        std::cin >> ServerOrClient;
    } while (ServerOrClient != 1 && ServerOrClient != 2 && ServerOrClient != 3 && ServerOrClient != 4);

    switch (ServerOrClient)
    {
    case 1:{ 
        Network_Server server; 
        break; 
    }

    case 2:{
        Network_Client client; 
        break; 
    }
    case 3: system("cls");  return 3;
    case 4: exit(0);
    default:
        break;
    }

    return 3;
}

static void MenuNavigation() {
    //Information();
    int mode;
    do {
        std::cout << "РЕЖИМЫ\nОдиночный >>> 1\nМногопользовательский >>> 2\nВыход >>> 4\n";
        do
        {
            std::cin >> mode;
        } while (mode != 1 && mode != 2 && mode != 4);

        if (mode == 1) {
            mode = InformationGameSingle();
        }
        else if (mode == 2) {
            mode = InformationGameMultiUser();
        }
    } while (mode != 4);
}


int main() {
	system("color a");
    system("chcp 1251");
    system("cls");
    MenuNavigation();
    //TestMessage();

	return 0;
}