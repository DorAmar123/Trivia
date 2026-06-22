#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
	: m_database(db), m_loginManager(db), m_statisticsManager(db), m_gameManager(db)
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(m_loginManager, *this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
	return new MenuRequestHandler(user, *this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return m_statisticsManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room& room)
{
	return new RoomAdminRequestHandler(room, user, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room& room)
{
	return new RoomMemberRequestHandler(room, user, *this);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, Game& game)
{
	return new GameRequestHandler(game, user, *this);
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return m_gameManager;
}