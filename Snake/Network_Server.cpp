#include "Network_Server.h"
#pragma warning(disable: 4996)

int Network_Server::countConnectionsUsers = 0;

Network_Server::Network_Server(){
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


	std::cout << "Сервер ждет подключения...\t\tIp сервера: " << std::endl;
	
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	CopeSlisten = sListen;
	bind(sListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	listen(sListen, SOMAXCONN);

	std::thread newConnection([&]() {
		ListenConnectionAndAccept((SOCKADDR*)&sockaddr, sizeofSockaddr);
	});
	
	newConnection.detach();

}

inline void Network_Server::ListenConnectionAndAccept(SOCKADDR* sockaddr, int& sizeofSockaddr) {
	SOCKET newConnection;
	while (true) {
		if (sListen != CopeSlisten) {
			sListen = CopeSlisten;
		}
		newConnection = accept(sListen, (SOCKADDR*)&sockaddr, &sizeofSockaddr);
		if (newConnection == 0) {
			std::cout << "Error Connection Client!!!\n";
		}
		else if (newConnection == INVALID_SOCKET) {
			continue;
		}
		else {		
			connection[countConnectionsUsers] = newConnection;
			countConnectionsUsers++;
			std::cout << "Клиент подключился!\n";
			char msg[256] = "Привет, это сообщение с сервера вы смогли подключится к серверу";
			send(newConnection, msg, sizeof(msg), NULL);
			std::thread RecvData([&]() {
				RecvMessageClient(newConnection);
			});


			RecvData.detach();
		}
	}
	Game();
}

inline void Network_Server::Game(){
	SnakeSingl snake;
}


inline void Network_Server::RecvMessageClient(SOCKET socket){
	char message[256]{};
	int bytesRecived;
	while (true) {
		bytesRecived = recv(socket, message, sizeof(message), NULL);
		if (bytesRecived > 0) {
			std::cout << message << std::endl;
			SendMessageClient(message);
		}
		else if (bytesRecived < 0) {
			std::cout << "\nПроизошла ошибка, клиент отпал от сервера!!!\n";
			std::cout << "\n\t\t\tКлиент отключился от сервера!!!\n";
			closesocket(socket);
			RemoveSockeInArray(socket);
			break;
		}
		else if (bytesRecived == 0) {
			std::cout << "\nЭммм, какой-то бред! Сервер отключили(как буд-то не сервер управляет, а клиенты)!!!" << std::endl;
		}
		message[0] = '\0';
	}
}

inline void Network_Server::SendMessageClient(char* message) const {
	for (int i = 0; i < countConnectionsUsers; i++) {
		send(connection[i], message, sizeof(message), NULL);
	}
}

inline void Network_Server::RemoveSockeInArray(SOCKET socket){	
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

inline void Network_Server::ChangeGame(){
	StartGame();
}

inline void Network_Server::StartGame(){
	Game();
}

inline int Network_Server::GetCountConnection() {
	return countConnectionsUsers;
}


inline std::string Network_Server::GetIpAddress() {
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


