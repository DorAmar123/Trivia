#pragma once
#include "IDatabase.h"
#include "LoginManager.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* db);

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();

	LoginManager& getLoginManager();
	IDatabase* getDatabase();

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
};
