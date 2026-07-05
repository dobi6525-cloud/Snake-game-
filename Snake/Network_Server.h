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
#include "Snake.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

class Network_Server {
public:
	inline Network_Server();

	inline void ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr);

	inline void Game();

	inline void RecvMessageClient(SOCKET socket);
	inline void SendMessageClient(char* message) const;

	inline void RemoveSockeInArray(SOCKET socket);

	inline void ChangeGame();
	inline void StartGame();

	inline int GetCountConnection();

	inline std::string GetIpAddress();

private:

	static int countConnectionsUsers;
	SOCKET CopeSlisten;
	SOCKET sListen;
	SOCKET connection[100]{};
	std::string name;
	std::mutex SendClientMutex;
	std::mutex RemoveClientMutex;
};