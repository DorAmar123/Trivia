#include "Server.h"
#include <iostream>
#include <string>
#include <exception>

#define PORT 8826

Server::Server() {
	WSADATA wsa_data = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		throw std::exception("Error");
	}

	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverSocket == INVALID_SOCKET) {
		throw std::exception("Error");
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		throw std::exception("Error");
	}

	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		throw std::exception("Error");
	}
}

Server::~Server() {
	try {
		closesocket(_serverSocket);
		for (auto& client : m_clients) {
			delete client.second;
		}
		m_clients.clear();
		WSACleanup();
	}
	catch (...) {}
}

void Server::run() {
	std::thread acceptThread(&Server::acceptClients, this);
	acceptThread.detach();

	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "EXIT") {
			break;
		}
	}
}

void Server::acceptClients() {
	while (true) {
		struct sockaddr_in clientAddress;
		int clientAddressLen = sizeof(clientAddress);

		SOCKET clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
		if (clientSocket == INVALID_SOCKET) {
			continue;
		}

		m_clients[clientSocket] = new LoginRequestHandler();

		std::thread clientThread(&Server::clientHandler, this, clientSocket);
		clientThread.detach();
	}
}

void Server::clientHandler(SOCKET clientSocket) {
	try {
		std::string helloMsg = "Hello";
		if (send(clientSocket, helloMsg.c_str(), static_cast<int>(helloMsg.size()), 0) == SOCKET_ERROR) {
			throw std::exception("Error");
		}

		char buffer[6] = { 0 };
		int bytesRecv = recv(clientSocket, buffer, 5, 0);
		if (bytesRecv <= 0) {
			throw std::exception("Error");
		}

		std::cout << buffer << std::endl;
	}
	catch (...) {
	}
}

int main() {
	try {
		Server server;
		server.run();
	}
	catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
