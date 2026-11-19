#pragma once

#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <conio.h>
#include "Snake.h"
#include <String>
#include <atomic>  
#include "Message.h"
#pragma comment(lib, "ws2_32.lib")

class MyltiplaySnake;

class Network_Client {
public:
    Network_Client();

    void InitMemoryClinet();

    void RecvMessageServer(SOCKET server);
    void SendMessageServer(SOCKET server, std::string message);

    bool isCheckingIpAdress(char* InternetProtocol);

    void Lobby(SOCKET server);
    void Compound(std::string message, char* concatenation, int position);
    void Clear(char* array, int position);
    bool CheckToRunGame(std::string str);
    void SendMessageLobby(std::atomic<bool>& flagListen, SOCKET server, char* concatenation, int lengthName);

    void SettingsSnake();
    void SendDataAboutSnake(std::string& name, SOCKET& socket);

    void WeitingForResult(SOCKET& server);

    void RecvMessagePlay(SOCKET& socket, MyltiplaySnake* snake);

    //void StartGame(SOCKET& server);
    //void RecvPositionSnake(SOCKET& server);
    //bool ValidationDataRecv(char* buffer

private:
    std::atomic<bool> flagSend = true;

    bool flagListen = true;
    bool flagSettings = false;
    unsigned int countUsers;
    unsigned int myPosition;
    std::vector<std::vector<int>> allPositionsSnake;
    std::vector<std::vector<int>> positionApple;

    SOCKET server;

    std::vector<std::string> namePlayers;
    std::vector<std::vector<char>> fashonSnake;
    char fashonHead;
    char fashonTail;
};