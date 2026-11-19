#pragma once

#include <WinSock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <conio.h>
#include <codecvt>
#include <algorithm>
#include "Snake.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma warning(disable: 4996)

class MultitaskPositionServer {
public:
    MultitaskPositionServer(std::string ipv4, SOCKET* connections, int countConnections);


private:
    void ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr);
    bool AcceptDisconnect(SOCKET& socket);
    void RemoveSockeInArray(SOCKET& socket);
    char* ConvertIntToChar(int** convert);

    void RecvMessageClient(SOCKET& socket);
    void SendMessageClient(char* message, SOCKET& socket);

    void CheckStatusCode(char* buffer, SOCKET& socket);

    SOCKET* connections;
    SOCKET sListen;


    int countConnections;
    int** place = NULL;
    char* convertData = NULL;
};
