#include "Network_Server.h"
#pragma warning(disable: 4996)

Network_Server::Network_Server() {
    //system("chcp 1251");
    InitMemory();
    std::cout << "инициализация сервера...\n";
    std::cout << "Подключаем нужную версию библиотеки\n";
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Ошибка загрузки бибоиотеки!!!\n";
        system("pause");
        exit(1);
    }
    std::cout << "Получаем ip адрес\n";
    std::string IPv4;

    std::cout << GetIpAddress() << "\nВыбирите на каком ip будет работать сервер: ";
    std::cin >> IPv4;

    SOCKADDR_IN sockaddr{};
    int sizeofSockaddr = sizeof(sockaddr);
    sockaddr.sin_addr.s_addr = inet_addr(IPv4.c_str());
    sockaddr.sin_port = htons(8080);
    sockaddr.sin_family = AF_INET;
    std::cout << "Сервер запущен на ip адресе: " << IPv4 << std::endl;


    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    CopeSlisten = sListen;
    bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
    listen(sListen, SOMAXCONN);

    std::thread newConnection([&]() {
        ListenConnectionAndAccept((SOCKADDR*)&sockaddr, sizeofSockaddr);
    });

    newConnection.detach();
    
    Lobbi();
}

void Network_Server::Lobbi(){
    PrintConnection.lock();
    std::cout << "Ожидание играков, запущено лобби" << std::endl;
    std::cout << "Вы можете не дожидаться всех играков и начать игру, для этого вам нужно написать start" << std::endl;
    std::cout << "Правила игры очень просты - убить других змеек и выжить при этом." << std::endl;
    std::cout << "Сейчас вы находитесь в лобби и оно предоставляет вам возможность писать другим игрокам, если вы как сообщение отправите start, то запуститься игра!!!." << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Перед началом игры вы должны придумать свой ник и произвести кастомизацию свое змейки." << std::endl;
    std::cout << "Введите выш ник:";

    std::cin >> name;
    nameUsers[0] = name;


    std::cout << "Прекрасный ник: " << name << std::endl;
    std::cout << "Чат запущен, можете вводить: " << std::endl;
    PrintConnection.unlock();


    int lenghtName = name.size();
    char concatenation[256]{};
    const int SIZE = 256;
    std::string command;    

    FillArray(name, concatenation);
    
    concatenation[lenghtName] = ':';
    concatenation[lenghtName + 1] = ' ';
    lenghtName += 2;
    
    std::cin.get();

    while (!CheckCommandStartGame(command)){
        command.clear();
        Clear(concatenation, lenghtName);

        std::getline(std::cin, command);

        FillArray(command, concatenation, lenghtName);
        SendMessageClient(concatenation, 0);

    };
    flagListen   = false;
    flagSettings = true;
    SettingsSnake();
    std::cout << "Ждем-с пока все игроки настроят свои змейки!\n";
    
    auto& readyRef = reinterpret_cast<std::atomic<int>&>(countReadyPlayers);
    auto& connRef  = reinterpret_cast<std::atomic<int>&>(countConnectionsUsers);

    while (readyRef.load() != connRef.load()) {
        std::this_thread::yield();
    } 
    
    flagSettings = false;
    StartGame();
}

void Network_Server::StartGame(){
    system("cls");
    bool YesOrNo;
    int countUsers = countConnectionsUsers + 1;
    MyltiplaySnake MyltiSnake(countUsers, true);
    MyltiplaySnake* snake = &MyltiSnake;
    MyltiSnake.InitOtherDataServer(this->connection);
    MyltiSnake.AlgorithmInitPositionsSnakeServer(PositionsSnake);

    PositionApple.resize(countConnectionsUsers + 1);
    for (int i = 0; i < countConnectionsUsers + 1; i++) {
        PositionApple[i].resize(2);
    }

    char* ResultMessage = new char[256];
    Message message(countConnectionsUsers + 1);
    

    for (int i = 0; i < countConnectionsUsers; i++) {

        message.DefoultDataCreate(PositionsSnake, fashonSnake, nameUsers, (char*&)ResultMessage, i+1, countConnectionsUsers+1);
        SendMessageOnlyOneUser(ResultMessage, i);
        Clear(ResultMessage, 0);
    }
    
    MyltiSnake.InitData(PositionsSnake, fashonSnake, nameUsers, 0, countConnectionsUsers+1);
    MyltiSnake.CreateAppleFullUsers(PositionApple);
    MyltiSnake.InitApple(PositionApple);
    
    message.DefoultDataAppleConvert(PositionApple, ResultMessage);
    SendMessageClient(ResultMessage, connection[countConnectionsUsers+1]);

    for (int i = 0; i < countConnectionsUsers; i++) {
        std::thread newPlayerStart([=]() {
            RecvMessagePlay(connection[i], snake);
            });

        newPlayerStart.detach();
    }

    MyltiSnake.StartGame();
}

void Network_Server::RecvMessagePlay(SOCKET& socket, MyltiplaySnake* snake){
    char buffer[256]{};
    int byte;
    while (true) {
        byte = recv(socket, buffer, 256, NULL);
        if (byte > 0) {
            if (buffer[0] != '\0') {
                snake->RecvMessage(buffer);
            }
        }
        else if (byte == 0) {
            std::cout << "Клиент отключился!\n";
            break;
        }
        else if(byte < 0){
            std::cout << "Ошибка получения данных от клиента" << std::endl;
            break;
        }
    }
}


void Network_Server::SendMessageOnlyOneUser(char* message, int index){
    send(connection[index], message, 256, NULL);
}

bool Network_Server::CheckCommandStartGame(std::string& command){
    if (command.find("start") != std::string::npos) {
        return true;
    }
    return false;
}

void Network_Server::FillArray(std::string str, char* output, int StartPosition){
    for (int i = 0; str[i] != '\0' && i < (256 - StartPosition); i++) {
        output[StartPosition + i] = str[i];
    }
}

void Network_Server::Clear(char* str, int position){
    for (int i = position; i < 255; i++) {
        str[i] = '\0';
    }
}

void Network_Server::GetNameHeadTail(char* name, SOCKET& socket){
    int indexWrite = NULL;
    for (int i = 0; i < countConnectionsUsers; i++) {
        if (connection[i] == socket) {
            indexWrite = i + 1;
        }
    }

    if (indexWrite == NULL) {
        return;
    }
    int i = 0;
    for (; name[i] != ':'; i++) {
        nameUsers[indexWrite][i] = name[i];
    }
    nameUsers[indexWrite][i] = '\0';

    fashonSnake[indexWrite][0] = name[++i];
    fashonSnake[indexWrite][1] = name[++i];

    countReadyPlayers++;
}

void Network_Server::SettingsSnake(){
    std::cout << "Как будет выглядеть голова змейки?\n>>> ";
    std::cin >> fashonHead;

    std::cout << "Как будет выглядеть тело змейки?\n>>> ";
    std::cin >> fashonTail;

    fashonSnake[0][0] = fashonHead;
    fashonSnake[0][1] = fashonTail;
}

void Network_Server::ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr) {
    SOCKET newConnection;
    while (flagListen) {
        if (sListen != CopeSlisten) {
            sListen = CopeSlisten;
        }
        newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sizeofSockaddr);
        if (newConnection == 0) {
            std::cout << "Ошибка подключения клиента!!!\n";
        }
        else if (newConnection == INVALID_SOCKET) {
            continue;
        }
        else if(flagListen) {
            PrintConnection.lock();
            connection[countConnectionsUsers] = newConnection;
            countConnectionsUsers++;

            std::cout << "Клиент подключился!\n";
            char msg[] = "Привет, это сообщение с сервера, вы смогли подключится к серверу";
            send(newConnection, msg, sizeof(msg), NULL);
            
            PrintConnection.unlock();

            std::thread RecvData([&]() {
                RecvMessageClient(newConnection);
            });

            RecvData.detach();
        }
    }
}

void Network_Server::InitMemory(){

    nameUsers.resize(32);
    fashonSnake.resize(32);
    for (int i = 0; i < 32; i++){
        fashonSnake[i].resize(2);
        nameUsers.resize(32);
    }
}

void Network_Server::RecvMessageClient(SOCKET socket) {
    char message[256]{};
    int bytesRecived;
    while (flagListen || flagSettings) {
        bytesRecived = recv(socket, message, sizeof(message), NULL);
        if (bytesRecived > 0 && flagSettings) {
            GetNameHeadTail(message, socket);
            return;
        }
        if (bytesRecived > 0 && flagListen) {
            std::cout << message << std::endl;
            SendMessageClient(message, socket);
        }
        else if (bytesRecived < 0) {
            
            std::cout << "\nПроизошла ошибка, клиент отпал от сервера!!!\n";
            std::cout << "\nКлиент отключился от сервера!!!\n";

            closesocket(socket);
            RemoveSockeInArray(socket);
            return;
        }
        else if (bytesRecived == 0) {
            std::cout << "\nКлиент закрыл соединение!!!" << std::endl;

            closesocket(socket);
            RemoveSockeInArray(socket);
            return;
        }
        for (int i = 0; message[i] != '\0'; i++) {
            message[i] = '\0';
        }
    }
}

void Network_Server::SendMessageClient(char* message, SOCKET socket) const {
    for (int i = 0; i < countConnectionsUsers; i++) {
        if (connection[i] != socket) {
            send(connection[i], message, 256, NULL);
        }
    }
}

inline void Network_Server::RemoveSockeInArray(SOCKET socket) {
    int lockationRemoveElement = 0;

    for (int i = 0; i < countConnectionsUsers; i++) {
        if (connection[i] == socket) {
            lockationRemoveElement = i;
            break;
        }
    }

    connection[lockationRemoveElement] = connection[countConnectionsUsers];
    connection[countConnectionsUsers] = 0;
    countConnectionsUsers--;

}

int Network_Server::GetCountConnection() {
    return countConnectionsUsers;
}

std::string Network_Server::GetIpAddress() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return "";
    }

    ULONG bufferSize = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    DWORD retVal;

    do {
        pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufferSize);
        if (!pAddresses) {
            WSACleanup();
            return "";
        }

        retVal = GetAdaptersAddresses(
            AF_UNSPEC,
            GAA_FLAG_INCLUDE_PREFIX,
            nullptr,
            pAddresses,
            &bufferSize
        );

        if (retVal == ERROR_BUFFER_OVERFLOW) {
            free(pAddresses);
            pAddresses = nullptr;
        }
        else {
            break;
        }
    } while (retVal == ERROR_BUFFER_OVERFLOW);

    if (retVal != NO_ERROR) {
        free(pAddresses);
        WSACleanup();
        return "";
    }

    std::string wifiIP;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    PIP_ADAPTER_ADDRESSES pCurr = pAddresses;
    while (pCurr) {
        std::string adapterName = converter.to_bytes(pCurr->FriendlyName);

        if (pCurr->OperStatus == IfOperStatusUp) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurr->FirstUnicastAddress;
            while (pUnicast) {
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    sockaddr_in* sa = (sockaddr_in*)pUnicast->Address.lpSockaddr;
                    char ipStr[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(sa->sin_addr), ipStr, INET_ADDRSTRLEN);

                    if (strcmp(ipStr, "127.0.0.1") != 0) {
                        wifiIP += ipStr;
                        wifiIP += "\n";
                        break;
                    }
                }
                pUnicast = pUnicast->Next;
            }
        }

        pCurr = pCurr->Next;
    }

    free(pAddresses);
    WSACleanup();

    return wifiIP;
}
