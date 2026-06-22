#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <map>
#include <mutex>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class Communicator
{
public:
	Communicator(RequestHandlerFactory* factory);
	~Communicator();

	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	std::mutex m_clientsMutex;
	RequestHandlerFactory* m_handlerFactory;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

	static bool recvAll(SOCKET sock, char* buffer, int length);
	static bool sendAll(SOCKET sock, const char* buffer, int length);
};