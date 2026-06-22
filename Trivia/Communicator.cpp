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
		std::lock_guard<std::mutex> lock(m_clientsMutex);
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
		{
			std::lock_guard<std::mutex> lock(m_clientsMutex);
			m_clients[clientSocket] = m_handlerFactory->createLoginRequestHandler();
		}

		std::thread clientThread(&Communicator::handleNewClient, this, clientSocket);
		clientThread.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	const unsigned int MAX_MESSAGE_SIZE = 10u * 1024u * 1024u;

	try
	{
		while (true)
		{
			unsigned char codeBuf[1] = { 0 };
			if (!recvAll(clientSocket, (char*)codeBuf, 1))
			{
				std::cout << "Client disconnected." << std::endl;
				break;
			}

			unsigned char sizeBuf[4] = { 0 };
			if (!recvAll(clientSocket, (char*)sizeBuf, 4))
				break;

			unsigned int size =
				((unsigned int)sizeBuf[0] << 24) |
				((unsigned int)sizeBuf[1] << 16) |
				((unsigned int)sizeBuf[2] << 8) |
				((unsigned int)sizeBuf[3]);

			if (size > MAX_MESSAGE_SIZE)
			{
				std::cout << "Message too large (" << size << " bytes), dropping client." << std::endl;
				break;
			}

			std::vector<unsigned char> data(size);
			if (size > 0 && !recvAll(clientSocket, (char*)data.data(), (int)size))
				break;

			RequestInfo reqInfo;
			reqInfo.id = codeBuf[0];
			reqInfo.receivalTime = time(NULL);
			reqInfo.buffer = data;

			IRequestHandler* handler = nullptr;
			{
				std::lock_guard<std::mutex> lock(m_clientsMutex);
				handler = m_clients[clientSocket];
			}

			RequestResult result{};
			result.newHandler = nullptr;
			try
			{
				if (handler->isRequestRelevant(reqInfo) == false)
				{
					ErrorResponse err;
					err.message = "Request not relevant to current state";
					result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
				}
				else
				{
					result = handler->handleRequest(reqInfo);
				}
			}
			catch (const std::exception& e)
			{
				ErrorResponse err;
				err.message = std::string("Invalid request: ") + e.what();
				result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
				result.newHandler = nullptr;
			}

			if (!sendAll(clientSocket, (const char*)result.response.data(), (int)result.response.size()))
			{
				std::cout << "Error sending message to client" << std::endl;
				break;
			}

			if (result.newHandler != nullptr)
			{
				std::lock_guard<std::mutex> lock(m_clientsMutex);
				if (result.newHandler != m_clients[clientSocket])
				{
					delete m_clients[clientSocket];
					m_clients[clientSocket] = result.newHandler;
				}
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Error in client thread: " << e.what() << std::endl;
	}

	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);
		auto it = m_clients.find(clientSocket);
		if (it != m_clients.end())
		{
			std::string username = it->second->getLoggedUsername();
			if (!username.empty())
				m_handlerFactory->getLoginManager().logout(username);
			delete it->second;
			m_clients.erase(it);
		}
	}
	closesocket(clientSocket);
}

bool Communicator::recvAll(SOCKET sock, char* buffer, int length)
{
	int total = 0;
	while (total < length)
	{
		int bytes = recv(sock, buffer + total, length - total, 0);
		if (bytes <= 0)
			return false;
		total += bytes;
	}
	return true;
}

bool Communicator::sendAll(SOCKET sock, const char* buffer, int length)
{
	int total = 0;
	while (total < length)
	{
		int bytes = send(sock, buffer + total, length - total, 0);
		if (bytes == SOCKET_ERROR)
			return false;
		total += bytes;
	}
	return true;
}
