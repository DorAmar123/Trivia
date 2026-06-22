#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "MenuRequestHandler.h"

#define LEAVE_GAME_CODE       15
#define GET_QUESTION_CODE     16
#define SUBMIT_ANSWER_CODE    17
#define GET_GAME_RESULTS_CODE 18

GameRequestHandler::GameRequestHandler(int roomId, const LoggedUser& user, RequestHandlerFactory& factory)
	: m_roomId(roomId), m_user(user), m_handlerFactory(factory), m_questionStartTime(0), m_statsSubmitted(false)
{
}

void GameRequestHandler::submitStatsOnce()
{
	if (m_statsSubmitted)
		return;
	m_statsSubmitted = true;
	try
	{
		m_handlerFactory.getGameManager().submitPlayerStats(m_roomId, m_user.getUsername());
	}
	catch (...)
	{
	}
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.id == LEAVE_GAME_CODE ||
		requestInfo.id == GET_QUESTION_CODE ||
		requestInfo.id == SUBMIT_ANSWER_CODE ||
		requestInfo.id == GET_GAME_RESULTS_CODE;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (requestInfo.id == LEAVE_GAME_CODE)
		return leaveGame(requestInfo);
	if (requestInfo.id == GET_QUESTION_CODE)
		return getQuestion(requestInfo);
	if (requestInfo.id == SUBMIT_ANSWER_CODE)
		return submitAnswer(requestInfo);
	if (requestInfo.id == GET_GAME_RESULTS_CODE)
		return getGameResults(requestInfo);

	ErrorResponse err = { "Unknown request for Game" };
	return { JsonResponsePacketSerializer::serializeErrorResponse(err), nullptr };
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& requestInfo)
{
	submitStatsOnce();

	LeaveGameResponse res = { 1 };
	return { JsonResponsePacketSerializer::serializeLeaveGameResponse(res),
			 m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo)
{
	m_questionStartTime = time(nullptr);
	GetQuestionResponse res;
	try
	{
		Game& game = m_handlerFactory.getGameManager().getGame(m_roomId);
		res = game.getQuestionForUser(m_user.getUsername());
	}
	catch (...)
	{
		res.status = 0;
	}
	return { JsonResponsePacketSerializer::serializeGetQuestionResponse(res), this };
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
{
	float timeSpent = (m_questionStartTime > 0)
		? (float)(time(nullptr) - m_questionStartTime)
		: 0.0f;
	m_questionStartTime = 0;

	SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);
	SubmitAnswerResponse res;
	try
	{
		Game& game = m_handlerFactory.getGameManager().getGame(m_roomId);
		res = game.submitAnswer(m_user.getUsername(), req.answerId, timeSpent);
	}
	catch (...)
	{
		res.status = 0;
	}
	return { JsonResponsePacketSerializer::serializeSubmitAnswerResponse(res), this };
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
	submitStatsOnce();

	GetGameResultsResponse res;
	try
	{
		Game& game = m_handlerFactory.getGameManager().getGame(m_roomId);
		res = game.getResults();
	}
	catch (...)
	{
		res.status = 0;
	}
	return { JsonResponsePacketSerializer::serializeGetGameResultsResponse(res), this };
}
