#pragma once
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "GameManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class GameRequestHandler;
class LoggedUser;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* db);

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);
	GameRequestHandler* createGameRequestHandler(const LoggedUser& user, int roomId);

	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	GameManager& getGameManager();

	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, int roomId);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, int roomId);

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
	GameManager m_gameManager;
};