#pragma once
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class LoggedUser;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* db);

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);

	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();

	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, Room& room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room& room);

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
};