#pragma once
#include <map>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class Communicator
{
public:
<<<<<<< HEAD
	Communicator(RequestHandlerFactory* factory);
	~Communicator();

=======
>>>>>>> origin/feature/v2-menu-handler
	void startHandleRequests();

private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
<<<<<<< HEAD
	RequestHandlerFactory* m_handlerFactory;
=======
>>>>>>> origin/feature/v2-menu-handler

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
};
