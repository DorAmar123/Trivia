#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "MenuRequestHandler.h"

#define LEAVE_GAME_CODE       15
#define GET_QUESTION_CODE     16
#define SUBMIT_ANSWER_CODE    17
#define GET_GAME_RESULTS_CODE 18

GameRequestHandler::GameRequestHandler(Game& game, const LoggedUser& user, RequestHandlerFactory& factory)
	: m_game(game), m_user(user), m_handlerFactory(factory), m_questionStartTime(0)
{
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
	LeaveGameResponse res = { 1 };
	return { JsonResponsePacketSerializer::serializeLeaveGameResponse(res),
			 m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo)
{
	m_questionStartTime = time(nullptr);
	GetQuestionResponse res = m_game.getQuestionForUser(m_user.getUsername());
	return { JsonResponsePacketSerializer::serializeGetQuestionResponse(res), this };
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
{
	float timeSpent = (m_questionStartTime > 0)
		? (float)(time(nullptr) - m_questionStartTime)
		: 0.0f;
	m_questionStartTime = 0;

	SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);
	SubmitAnswerResponse res = m_game.submitAnswer(m_user.getUsername(), req.answerId, timeSpent);
	return { JsonResponsePacketSerializer::serializeSubmitAnswerResponse(res), this };
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
	GetGameResultsResponse res = m_game.getResults();
	return { JsonResponsePacketSerializer::serializeGetGameResultsResponse(res), this };
}
