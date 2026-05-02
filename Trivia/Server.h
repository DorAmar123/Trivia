#pragma once
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <map>
#include <thread>
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "IDatabase.h"

class Server {
public:
	Server();
	~Server();
	void run();

private:
	void acceptClients();
	void clientHandler(SOCKET clientSocket);

	SOCKET _serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;

	IDatabase* m_database;
};