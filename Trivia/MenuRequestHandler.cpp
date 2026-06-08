#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

#define LOGOUT_REQUEST_CODE 3
#define GET_ROOMS_REQUEST_CODE 4
#define GET_PLAYERS_IN_ROOM_CODE 5
#define JOIN_ROOM_REQUEST_CODE 6
#define CREATE_ROOM_REQUEST_CODE 7
#define GET_PERSONAL_STATS_CODE 8
#define GET_HIGH_SCORE_REQUEST_CODE 9

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory)
	: m_user(user), m_handlerFactory(handlerFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	int id = requestInfo.id;
	return id == LOGOUT_REQUEST_CODE ||
		id == GET_ROOMS_REQUEST_CODE ||
		id == GET_PLAYERS_IN_ROOM_CODE ||
		id == JOIN_ROOM_REQUEST_CODE ||
		id == CREATE_ROOM_REQUEST_CODE ||
		id == GET_PERSONAL_STATS_CODE ||
		id == GET_HIGH_SCORE_REQUEST_CODE;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	switch (requestInfo.id)
	{
	case LOGOUT_REQUEST_CODE:         return logout(requestInfo);
	case GET_ROOMS_REQUEST_CODE:      return getRooms(requestInfo);
	case GET_PLAYERS_IN_ROOM_CODE:    return getPlayersInRoom(requestInfo);
	case JOIN_ROOM_REQUEST_CODE:      return joinRoom(requestInfo);
	case CREATE_ROOM_REQUEST_CODE:    return createRoom(requestInfo);
	case GET_PERSONAL_STATS_CODE:     return getPersonalStats(requestInfo);
	case GET_HIGH_SCORE_REQUEST_CODE: return getHighScore(requestInfo);
	default:
		ErrorResponse err;
		err.message = "Unknown request";
		return { JsonResponsePacketSerializer::serializeErrorResponse(err), nullptr };
	}
}

RequestResult MenuRequestHandler::logout(const RequestInfo& requestInfo)
{
	m_handlerFactory.getLoginManager().logout(m_user.getUsername());

	LogoutResponse res;
	res.status = 1;
	return { JsonResponsePacketSerializer::serializeLogoutResponse(res),
			 m_handlerFactory.createLoginRequestHandler() };
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
	GetRoomsResponse res;
	res.status = 1;
	res.rooms = m_handlerFactory.getRoomManager().getRooms();
	return { JsonResponsePacketSerializer::serializeGetRoomsResponse(res), nullptr };
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
	GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(requestInfo.buffer);

	GetPlayersInRoomResponse res;
	res.players = m_handlerFactory.getRoomManager().getRoom(req.roomId).getAllUsers();
	return { JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(res), nullptr };
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
	JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);

	JoinRoomResponse res;
	try
	{
		m_handlerFactory.getRoomManager().getRoom(req.roomId).addUser(m_user);
		res.status = 1;
	}
	catch (...)
	{
		res.status = 0;
	}
	return { JsonResponsePacketSerializer::serializeJoinRoomResponse(res), nullptr };
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
	CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);

	RoomData data;
	data.name = req.roomName;
	data.maxPlayers = req.maxUsers;
	data.numOfQuestionsInGame = req.questionCount;
	data.timePerQuestion = req.answerTimeout;

	m_handlerFactory.getRoomManager().createRoom(m_user, data);

	CreateRoomResponse res;
	res.status = 1;
	return { JsonResponsePacketSerializer::serializeCreateRoomResponse(res), nullptr };
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo)
{
	GetPersonalStatsResponse res;
	res.status = 1;
	res.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUsername());
	return { JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(res), nullptr };
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo)
{
	GetHighScoreResponse res;
	res.status = 1;
	res.statistics = m_handlerFactory.getStatisticsManager().getHighScore();
	return { JsonResponsePacketSerializer::serializeGetHighScoreResponse(res), nullptr };
}