#pragma once

#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Network_Client{
public:
	inline Network_Client();

	inline void RecvMessageServer(SOCKET server);

	inline bool isCheckingIpAdress(char* InternetProtocol);
};

