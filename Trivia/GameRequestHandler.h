#pragma once
#include "IRequestHandler.h"
#include "Game.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(int roomId, const LoggedUser& user, RequestHandlerFactory& factory);

	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
	std::string getLoggedUsername() const override { return m_user.getUsername(); }

private:
	int m_roomId;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	time_t m_questionStartTime;
	bool m_statsSubmitted;

	RequestResult leaveGame(const RequestInfo& requestInfo);
	RequestResult getQuestion(const RequestInfo& requestInfo);
	RequestResult submitAnswer(const RequestInfo& requestInfo);
	RequestResult getGameResults(const RequestInfo& requestInfo);
	void submitStatsOnce();
};
