#include "Server.h"
#include "SqliteDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"
#include <iostream>
#include <string>
#include <exception>
#include <thread>

Server::Server()
{
	WSADATA wsa_data = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		throw std::exception("Error");

	m_database = new SqliteDatabase();
	m_handlerFactory = new RequestHandlerFactory(m_database);
	m_communicator = new Communicator(m_handlerFactory);
}

Server::~Server()
{
	try
	{
		WSACleanup();
	}
	catch (...) {}
}

void Server::run()
{
	std::thread communicatorThread(&Communicator::startHandleRequests, m_communicator);
	communicatorThread.detach();

	std::string cmd;
	while (std::cin >> cmd)
	{
		if (cmd == "EXIT")
			break;
	}
}

int main()
{
	try
	{
		Server server;
		server.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}