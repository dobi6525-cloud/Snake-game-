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
#include "Message.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

class MyltiplaySnake;

class Network_Server {
public:
    Network_Server();
    void ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr);

    void InitMemory();

    void RecvMessageClient(SOCKET socket);
    void SendMessageClient(char* message, SOCKET socket) const;

    void RemoveSockeInArray(SOCKET socket);
    int GetCountConnection();
    std::string GetIpAddress();
    
    void Lobbi();
    void StartGame();
    //void ConvertData(int** data, char* result, int indexConvert);

    void SendMessageOnlyOneUser(char* message, int index);

    bool CheckCommandStartGame(std::string& command);
    void FillArray(std::string str, char* output, int StartPosition = 0);
    void Clear(char* str, int position);

    void GetNameHeadTail(char* name, SOCKET& socket);

    void SettingsSnake();

    void RecvMessagePlay(SOCKET& socket, MyltiplaySnake* snake);

private:

    //friend void MyltiplaySnake::SendMessageServer(Network_Server& server, SOCKET*& connection);

    int countConnectionsUsers = 0;
    int countReadyPlayers = 0;

    SOCKET CopeSlisten;
    SOCKET sListen;
    SOCKET connection[60]{};
    std::string name;
    std::mutex SendClientMutex;
    std::mutex RemoveClientMutex;
    std::mutex PrintConnection;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;

    bool flagListen = true;
    bool flagSettings = false;

    std::vector<std::vector<int>> PositionsSnake;
    std::vector<std::vector<int>> PositionApple;
    char sendData[256]{};
    char** ViewOthreSnake = NULL;

    std::vector<std::string> nameUsers;
    std::vector<std::vector<char>> fashonSnake;

    char** convertNameSnake[256]{};

    char fashonHead;
    char fashonTail;
};