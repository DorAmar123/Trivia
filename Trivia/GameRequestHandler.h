#pragma once
#include "IRequestHandler.h"
#include "Game.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(Game& game, const LoggedUser& user, RequestHandlerFactory& factory);

	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	Game& m_game;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	time_t m_questionStartTime;

	RequestResult leaveGame(const RequestInfo& requestInfo);
	RequestResult getQuestion(const RequestInfo& requestInfo);
	RequestResult submitAnswer(const RequestInfo& requestInfo);
	RequestResult getGameResults(const RequestInfo& requestInfo);
};
