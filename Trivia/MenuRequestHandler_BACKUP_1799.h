#pragma once
#include "IRequestHandler.h"
<<<<<<< HEAD
=======
#include "LoggedUser.h"

class RequestHandlerFactory;
>>>>>>> origin/feature/v2-menu-handler

class MenuRequestHandler : public IRequestHandler
{
public:
<<<<<<< HEAD
	MenuRequestHandler() = default;
	~MenuRequestHandler() override = default;

	bool isRequestRelevant(const RequestInfo& info) override;
	RequestResult handleRequest(const RequestInfo& info) override;
=======
	MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory);
	~MenuRequestHandler() override = default;

	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestResult logout(const RequestInfo& requestInfo);
	RequestResult getRooms(const RequestInfo& requestInfo);
	RequestResult getPlayersInRoom(const RequestInfo& requestInfo);
	RequestResult joinRoom(const RequestInfo& requestInfo);
	RequestResult createRoom(const RequestInfo& requestInfo);
	RequestResult getPersonalStats(const RequestInfo& requestInfo);
	RequestResult getHighScore(const RequestInfo& requestInfo);

	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
>>>>>>> origin/feature/v2-menu-handler
};
