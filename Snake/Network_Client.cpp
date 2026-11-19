#include "Network_Client.h"

#pragma warning(disable: 4996)


Network_Client::Network_Client() {
	std::cout << "инициализация клиента...\n";
	std::cout << "Подключаем нужную версию библиотеки\n";
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error load network!!!\n";
		system("pause");
		exit(1);
	}
	char internetProtocol[9];
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
	
	bool flag = 1;
	do {
		if (connect(Connection, (SOCKADDR*)&sockaddr, sizeofSockaddr) != 0) {
			std::cout << "Error to connect!!!\n";
		}
		else {
			char message[256] = "Клиент сообщает что подключился :)";
			send(Connection, message, sizeof(message), NULL);
			flag = 0;
		}

	} while (flag);
	std::thread RecvMessage([&]() {
		RecvMessageServer(Connection);
	});

	std::cout << "Подключение успешно!!!\n";

	RecvMessage.join();

	system("pause");
}

inline void Network_Client::RecvMessageServer(SOCKET server){
	char message[256]{};
	int bytesRecived;
	while (true) {
		bytesRecived = recv(server, message, sizeof(message), NULL);
		if (bytesRecived > 0) {
			std::cout << std::endl << message << std::endl;
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

inline bool Network_Client::isCheckingIpAdress(char* InternetProtocol){
	int countDotnet = 0, lenghtIp = 0;
	for (int i = 0; InternetProtocol[i] != '\0'; i++) {
		if (InternetProtocol[i] == '.') {
			countDotnet++;
		}
		lenghtIp++;
	}

	if (lenghtIp >= 9 && lenghtIp <= 12 && countDotnet == 3) {
		return true;
	}

	return false;
}



