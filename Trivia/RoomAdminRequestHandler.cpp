#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"

#define CLOSE_ROOM_CODE      11
#define START_GAME_CODE      12
#define LEAVE_ROOM_CODE      13
#define GET_ROOM_STATE_CODE  14

RoomAdminRequestHandler::RoomAdminRequestHandler(int roomId, const LoggedUser& user, RequestHandlerFactory& handlerFactory)
    : m_roomId(roomId), m_user(user), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == CLOSE_ROOM_CODE ||
        requestInfo.id == START_GAME_CODE ||
        requestInfo.id == GET_ROOM_STATE_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.id == CLOSE_ROOM_CODE)
        return closeRoom(requestInfo);
    if (requestInfo.id == START_GAME_CODE)
        return startGame(requestInfo);
    if (requestInfo.id == GET_ROOM_STATE_CODE)
        return getRoomState(requestInfo);

    ErrorResponse err = { "Unknown request for Room Admin" };
    return { JsonResponsePacketSerializer::serializeErrorResponse(err), nullptr };
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    m_handlerFactory.getRoomManager().setRoomState(m_roomId, ROOM_CLOSE);

    CloseRoomResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeCloseRoomResponse(res),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    try
    {
        m_handlerFactory.getRoomManager().setRoomState(m_roomId, ROOM_ACTIVE);
        Room& room = m_handlerFactory.getRoomManager().getRoom(m_roomId);
        m_handlerFactory.getGameManager().createGame(room);
    }
    catch (...)
    {
        ErrorResponse err = { "Could not start game: room not found" };
        return { JsonResponsePacketSerializer::serializeErrorResponse(err), this };
    }

    StartGameResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeStartGameResponse(res),
             m_handlerFactory.createGameRequestHandler(m_user, m_roomId) };
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    GetRoomStateResponse res;

    try
    {
        Room& room = m_handlerFactory.getRoomManager().getRoom(m_roomId);
        RoomData metadata = room.getMetadata();
        RoomStatus state = m_handlerFactory.getRoomManager().getRoomState(m_roomId);

        res.status = 1;
        res.hasGameBegun = (state == ROOM_ACTIVE);
        res.players = room.getAllUsers();
        res.answerCount = metadata.numOfQuestionsInGame;
        res.answerTimeout = metadata.timePerQuestion;
    }
    catch (...)
    {
        res.status = 0;
    }

    return { JsonResponsePacketSerializer::serializeGetRoomStateResponse(res), this };
}