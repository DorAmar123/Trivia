#pragma once
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <thread>
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"

class Server
{
public:
	Server();
	~Server();
	void run();

private:
	IDatabase* m_database;
	RequestHandlerFactory* m_handlerFactory;
	Communicator* m_communicator;
};
