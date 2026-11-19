#include "MultitaskPositionServer.h"
#include "Network_Server.h"

MultitaskPositionServer::MultitaskPositionServer(std::string IPv4, SOCKET* connections, int countConnections){
    this->countConnections = countConnections;
    this->connections = connections;

    MyltiplaySnake snake(countConnections);
    place = snake.AlgorithmInitPositionsSnakeServer();

    convertData = ConvertIntToChar(place);

    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    SOCKADDR_IN sockaddr{};
    int sizeofSockaddr = sizeof(sockaddr);
    sockaddr.sin_addr.s_addr = inet_addr(IPv4.c_str());
    sockaddr.sin_port = htons(8080);
    sockaddr.sin_family = AF_INET;

    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
    listen(sListen, countConnections);

    std::thread newConnection([&]() {
        ListenConnectionAndAccept((SOCKADDR*)&sockaddr, sizeofSockaddr);
    });

    newConnection.detach();

    
}


void MultitaskPositionServer::ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr) {
    SOCKET newConnection;
    int countUsers = 0;
    while (countUsers < countConnections) {
        newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sizeofSockaddr);
        if (newConnection == 0) {
            std::cout << "Error Connection Client!!!\n";
        }
        else if (newConnection == INVALID_SOCKET) {
            continue;
        }
        else if (AcceptDisconnect(newConnection)) {
            connections[countUsers] = newConnection;
            countUsers++;

            std::thread RecvData([&]() {
                RecvMessageClient(newConnection);
                });

            RecvData.detach(); 
        }
        else {
            
        }
    }

    for (int i = 0; i < countConnections; i++) {
        send(connections[i], convertData, 256, NULL);
    }
}

bool MultitaskPositionServer::AcceptDisconnect(SOCKET& socket){
    for (int i = 0; connections[i] != 0; i++) {
        if (connections[i] == socket) {
            return true;
        }
    }
    return false;
}

void MultitaskPositionServer::RemoveSockeInArray(SOCKET& socket){
    for (int i = 0; i < countConnections; i++) {
        if (connections[i] == socket) {
            connections[i] = connections[countConnections - 1];
            connections[countConnections] = 0;
            countConnections--;
            break;
        }
    }
}

char* MultitaskPositionServer::ConvertIntToChar(int** convert){
    char convertPlace[256]{};
    for (int i = 0, j = 0; convert[i][0] > 0; i++, j++) {
        convertPlace[j]   = (char)convert[i][0];
        convertPlace[++j] = (char)convert[i][1];
    }
    return convertPlace;
}

void MultitaskPositionServer::RecvMessageClient(SOCKET& socket){
    char buffer[256]{};
    int byte;
    byte = recv(socket, buffer, 256, NULL);
    while (true) {
        if (byte < 0) {
            std::cout << "Клиент отпал" << std::endl;
        }
        else if (byte == 0) {
            std::cout << "Клиент оборвал соединение" << std::endl;
        }
        else if (byte > 0) {
            CheckStatusCode(buffer, socket);
        }
    }
}

void Network_Server::ConvertData(int** data, char* result)
{
}

void MultitaskPositionServer::SendMessageClient(char* message, SOCKET& socket){
    for (int i = 0; i < countConnections; i++) {
        if (connections[i] != socket) {
            send(connections[i], message, 256, NULL);
        }
    }
}

void MultitaskPositionServer::CheckStatusCode(char* buffer, SOCKET& socket){
    switch (buffer[0])
    {
    case char(1):
        send(socket, convertData, 256, NULL);
    default:
        std::cout << buffer << std::endl;
    }
}