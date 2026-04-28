#include "Server.h"
#include <iostream>
#include <string>
#include <exception>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

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
		while (true)
		{
			char codeBuf[1] = { 0 };
			if (recv(clientSocket, codeBuf, 1, 0) <= 0) {
				std::cout << "Client disconnected." << std::endl;
				break;
			}

			char sizeBuf[4] = { 0 };
			if (recv(clientSocket, sizeBuf, 4, 0) <= 0) {
				break;
			}
			int size = (sizeBuf[0] << 24) | (sizeBuf[1] << 16) | (sizeBuf[2] << 8) | sizeBuf[3];

			std::vector<unsigned char> data(size);
			if (size > 0 && recv(clientSocket, (char*)data.data(), size, 0) <= 0) {
				break;
			}

			RequestInfo reqInfo = { codeBuf[0], time(NULL), data };
			std::vector<unsigned char> responseBytes;

			if (reqInfo.id == 1) // Login
			{
				LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo.buffer);
				std::cout << "Server login req, user: " << req.username << " pass: " << req.password << std::endl;

				LoginResponse res;
				res.status = 1;
				responseBytes = JsonResponsePacketSerializer::serializeLoginResponse(res);
			}
			else if (reqInfo.id == 2) // Signup
			{
				SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(reqInfo.buffer);
				std::cout << "Server signup req, user: " << req.username << " pass: " << req.password << " mail: " << req.email << std::endl;

				SignupResponse res;
				res.status = 1;
				responseBytes = JsonResponsePacketSerializer::serializeSignupResponse(res);
			}

			if (!responseBytes.empty()) {
				send(clientSocket, (char*)responseBytes.data(), responseBytes.size(), 0);
			}
		}
	}
	catch (std::exception& e) {
		std::cout << "Error in client thread: " << e.what() << std::endl;
	}
	closesocket(clientSocket);
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
