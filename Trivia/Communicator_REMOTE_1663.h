#pragma once
#include <map>
#include <WinSock2.h>
#include "IRequestHandler.h"

class Communicator
{
public:
	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
};
