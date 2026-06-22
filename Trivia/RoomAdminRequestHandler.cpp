#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"

#define CLOSE_ROOM_CODE      11
#define START_GAME_CODE      12
#define LEAVE_ROOM_CODE      13
#define GET_ROOM_STATE_CODE  14

RoomAdminRequestHandler::RoomAdminRequestHandler(Room& room, const LoggedUser& user, RequestHandlerFactory& handlerFactory)
    : m_room(room), m_user(user), m_handlerFactory(handlerFactory)
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
    m_handlerFactory.getRoomManager().setRoomState(m_room.getMetadata().id, ROOM_CLOSE);

    CloseRoomResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeCloseRoomResponse(res),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    m_handlerFactory.getRoomManager().setRoomState(m_room.getMetadata().id, ROOM_ACTIVE);

    StartGameResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeStartGameResponse(res),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    GetRoomStateResponse res;

    RoomData metadata = m_room.getMetadata();
    RoomStatus state = m_handlerFactory.getRoomManager().getRoomState(metadata.id);

    res.status = 1;
    res.hasGameBegun = (state == ROOM_ACTIVE);
    res.players = m_room.getAllUsers();
    res.answerCount = metadata.numOfQuestionsInGame;
    res.answerTimeout = metadata.timePerQuestion;

    return { JsonResponsePacketSerializer::serializeGetRoomStateResponse(res), this };
}