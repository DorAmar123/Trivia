#pragma once
#include <map>
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
	RequestHandlerFactory* m_handlerFactory;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
};
