#include "Network_Client.h"

#pragma warning(disable: 4996)


Network_Client::Network_Client() {
    InitMemoryClinet();
    std::cout << "инициализация клиента...\n";
    std::cout << "Подключаем нужную версию библиотеки\n";
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error load network!!!\n";
        system("pause");
        exit(1);
    }
    char internetProtocol[16]{};
    std::cout << "Введите ip адресс сервера: ";
    do {
        std::cin >> internetProtocol;
    } while (!isCheckingIpAdress(internetProtocol));

    std::cout << "Инициализируем протокол подключения\n";
    SOCKADDR_IN sockaddr{};
    int sizeofSockaddr = sizeof(sockaddr);
    sockaddr.sin_addr.s_addr = inet_addr(internetProtocol);
    sockaddr.sin_port = htons(8080);
    sockaddr.sin_family = AF_INET;

    std::cout << "Попытка подключится к серверу...\n";

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
    server = Connection;
    bool flag = 1;
    do {
        if (connect(Connection, (SOCKADDR*)&sockaddr, sizeofSockaddr) != 0) {
            std::cout << "Ошибка подключения!!!\n";
        }
        else {
            char message[256] = "Клиент сообщает что подключился :)";
            send(Connection, message, sizeof(message), NULL);
            flag = 0;
        }

    } while (flag);

    Lobby(Connection);
}

void Network_Client::InitMemoryClinet(){
    allPositionsSnake.resize(64);
    for (int i = 0; i < 64; i++) {
        allPositionsSnake[i].resize(2);
    }
    
    namePlayers.resize(20);
    fashonSnake.resize(20);

    for (int i = 0; i < 20; i++) {
        namePlayers[i].resize(20);
        fashonSnake[i].resize(2);
    }

    //namePlayers = new char* [20];
    //fashonSnake = new char* [20];

    //for (int i = 0; i < 20; i++) {
    //    namePlayers[i] = new char[20];
    //    fashonSnake[i] = new char[2];
    //}
}

void Network_Client::RecvMessageServer(SOCKET server) {
    char message[256]{};
    
    int bytesRecived;
    while (flagListen) {
        bytesRecived = recv(server, message, 256, NULL);
        if (bytesRecived > 0) {
           
            if (CheckToRunGame(message)){
                flagSend = false;
                CancelIoEx(GetStdHandle(STD_INPUT_HANDLE), NULL);
                break;
            }

            std::cout << std::endl << message << std::endl;
            for (int i = 0; i < 256; i++) {
                message[i] = '\0';
            }
        }
        else if (bytesRecived == 0) {
            std::cout << "Сервер отключил вас либо забанил вас!!!" << std::endl;
            break;
        }
        else if (bytesRecived < 0) {
            std::cout << "У сервера отпало соединение!!!" << std::endl;
            break;
        }
        else {
            std::cout << "какая-то неизвестная ошибка, на которую Великий Михаил забил болт" << std::endl;
        }
    }
}

void Network_Client::SendMessageServer(SOCKET server, std::string message){
    send(server, message.c_str(), 256, NULL);
}

bool Network_Client::isCheckingIpAdress(char* InternetProtocol) {
    return true;
}

void Network_Client::Lobby(SOCKET server){
    system("cls");
    std::string message;
    std::string name;
    char concatenation[256]{};
    int lenghtName;

    std::cin.get();
    std::cout << "Введите ваш ник: ";
    std::getline(std::cin, name);
    lenghtName = name.size();
    Compound(name, concatenation, 0);

    concatenation[lenghtName] = ':';
    concatenation[lenghtName + 1] = ' ';

    lenghtName += 2;

    
    std::thread SendMessageL([&] {
        SendMessageLobby(flagSend, server, concatenation, lenghtName);
    });

    RecvMessageServer(server);
    SendMessageL.detach();

    flagSettings = true;
    flagListen = true;
    SettingsSnake();
    SendDataAboutSnake(name, server);

    WeitingForResult(server);

    MyltiplaySnake myltiplaySnake(countUsers, false);
    MyltiplaySnake* snake = &myltiplaySnake;
    myltiplaySnake.InitOtherDataClient(server);
    myltiplaySnake.InitData(allPositionsSnake, fashonSnake, namePlayers, myPosition, countUsers);
    myltiplaySnake.InitApple(positionApple);

    std::thread RecvMessagePlay([&] {
        RecvMessagePlay(server, snake);
    });


    myltiplaySnake.StartGame();
    RecvMessagePlay.detach();
}

void Network_Client::Compound(std::string message, char* concatenation, int position){
    for (int i = 0; message[i] != '\0'; ++i) {
        concatenation[i + position] = message[i];
    }
}

void Network_Client::Clear(char* array, int position) {
    for (int i = position; array[i] != '\0'; i++) {
        array[i] = '\0';
    }
}

bool Network_Client::CheckToRunGame(std::string str){
    if (str.find("start") != std::string::npos) {
        return true;
    }
    return false;
}

void Network_Client::SendMessageLobby(std::atomic<bool>& flagListen, SOCKET server, char* concatenation, int lengthName) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    std::string message;
    message.resize(255);

    while (flagListen) {
        DWORD bytesRead = 0;

        BOOL success = ReadFile(hStdin, &message[0], 254, &bytesRead, NULL);

        if (success && bytesRead > 0) {
            message.resize(bytesRead);

            while (!message.empty() && (message.back() == '\r' || message.back() == '\n')) {
                message.pop_back();
            }

            if (!message.empty()) {
                Compound(message, concatenation, lengthName);
                SendMessageServer(server, concatenation);

                Clear(concatenation, lengthName);
                message.assign(255, '\0');
            }
        }
        else {
            if (GetLastError() == ERROR_OPERATION_ABORTED) {
                break;
            }
        }
    }
}

void Network_Client::SettingsSnake() {
    std::cout << "Как будет выглядеть голова змейки?\n>>> ";
    std::cin >> fashonHead;
    
    std::cout << "Как будет выглядеть тело змейки?\n>>> ";
    std::cin >> fashonTail;

    fashonSnake[0][0] = fashonHead;
    fashonSnake[0][1] = fashonTail;
}

void Network_Client::SendDataAboutSnake(std::string& name, SOCKET& socket){
    std::string message;
    message.resize(256);
    message = name + ':' + Network_Client::fashonHead + Network_Client::fashonTail;

    send(socket, message.c_str(), 256, NULL);
}

void Network_Client::WeitingForResult(SOCKET& server){
    Message ResultFromTheServer;
    char* buffer = new char[256];
    int bytesRecived;
    int count = 0;


    std::cout << "Ожидаем пока все игроки настроят свои змейки...\n";


    while (true) {
        bytesRecived = recv(server, buffer, 256, NULL);
        if (bytesRecived > 0) {
            if (count == 0){
                ResultFromTheServer.DefoultDataDecode(allPositionsSnake, fashonSnake, namePlayers, myPosition, countUsers, (char*&)buffer);
                positionApple.resize(countUsers);
                for (int i = 0; i < countUsers; i++){
                    positionApple[i].resize(2);
                }
                count++;
            }
            else if (count == 1) {
                ResultFromTheServer.DefoultDataAppleDecode(positionApple, buffer);
                break;
            }
        }
        else if (bytesRecived == 0) {
            std::cout << "Сервер отключил вас либо забанил вас!!!" << std::endl;
            exit(1);
        }
        else if (bytesRecived < 0) {
            std::cout << "У сервера отпало соединение!!!" << std::endl;
            exit(1);
        }
        else {
            std::cout << "какая-то неизвестная ошибка, на которую Великий Михаил забил хуй" << std::endl;
            exit(1);
        }
    }
}

void Network_Client::RecvMessagePlay(SOCKET& server, MyltiplaySnake* snake) {
    char buffer[256]{};
    int byte;
    while (true) {
        byte = recv(server, buffer, 255, NULL);
        if (byte > 0) {
            if (buffer[0] != '\0') {
                snake->RecvMessage(buffer);
            }
        }
        else if (byte == 0) {
            std::cout << "Сервер отключился!\n";
            break;
        }
        else if (byte < 0) {
            std::cout << "Ошибка получения данных от сервера" << std::endl;
            break;
        }
    }
}