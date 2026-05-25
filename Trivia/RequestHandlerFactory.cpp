#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) : m_database(db), m_loginManager(db)
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler();
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

IDatabase* RequestHandlerFactory::getDatabase()
{
	return m_database;
}
