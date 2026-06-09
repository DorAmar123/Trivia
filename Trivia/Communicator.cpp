#include "Communicator.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginRequestHandler.h"
#include <iostream>
#include <thread>
#include <exception>
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"

#define PORT 8826

Communicator::Communicator(RequestHandlerFactory* factory) : m_handlerFactory(factory), m_serverSocket(INVALID_SOCKET)
{
}
Communicator::~Communicator()
{
	try
	{
		closesocket(m_serverSocket);
		std::map<SOCKET, IRequestHandler*>::iterator it;
		for (it = m_clients.begin(); it != m_clients.end(); it++)
		{
			delete it->second;
		}
		m_clients.clear();
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	bindAndListen();
}

void Communicator::bindAndListen()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception("Failed to create socket");

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (bind(m_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		throw std::exception("Failed to bind socket");

	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception("Failed to listen");

	std::cout << "Server listening on port " << PORT << std::endl;

	while (true)
	{
		struct sockaddr_in clientAddress;
		int clientAddressLen = sizeof(clientAddress);

		SOCKET clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
		if (clientSocket == INVALID_SOCKET)
			continue;

		std::cout << "Client connected." << std::endl;
		m_clients[clientSocket] = m_handlerFactory->createLoginRequestHandler();

		std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
		clientThread.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		while (true)
		{
			char codeBuf[1] = { 0 };
			if (recv(clientSocket, codeBuf, 1, 0) <= 0)
			{
				std::cout << "Client disconnected." << std::endl;
				break;
			}

			char sizeBuf[4] = { 0 };
			if (recv(clientSocket, sizeBuf, 4, 0) <= 0)
				break;

			int size = (sizeBuf[0] << 24) | (sizeBuf[1] << 16) | (sizeBuf[2] << 8) | sizeBuf[3];

			std::vector<unsigned char> data(size);
			if (size > 0 && recv(clientSocket, (char*)data.data(), size, 0) <= 0)
				break;

			RequestInfo reqInfo;
			reqInfo.id = codeBuf[0];
			reqInfo.receivalTime = time(NULL);
			reqInfo.buffer = data;

			IRequestHandler* handler = m_clients[clientSocket];

			if (handler->isRequestRelevant(reqInfo) == false)
			{
				ErrorResponse err;
				err.message = "Request not relevant to current state";
				std::vector<unsigned char> errBytes = JsonResponsePacketSerializer::serializeErrorResponse(err);
				int sentBytes = send(clientSocket, (char*)errBytes.data(), errBytes.size(), 0);
				if (sentBytes == SOCKET_ERROR)
				{
					std::cout << "Error sending message to client" << std::endl;
					break;
				}
				continue;
			}

			RequestResult result = handler->handleRequest(reqInfo);
			int sentBytes = send(clientSocket, (char*)result.response.data(), result.response.size(), 0);
			if (sentBytes == SOCKET_ERROR)
			{
				std::cout << "Error sending message to client" << std::endl;
				break;
			}

			if (result.newHandler != nullptr)
			{
				delete m_clients[clientSocket];
				m_clients[clientSocket] = result.newHandler;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Error in client thread: " << e.what() << std::endl;
	}

	delete m_clients[clientSocket];
	m_clients.erase(clientSocket);
	closesocket(clientSocket);
}
