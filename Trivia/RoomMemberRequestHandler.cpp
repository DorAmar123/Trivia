#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "Responses.h"
#include "GameRequestHandler.h"

#define LEAVE_ROOM_CODE      13
#define GET_ROOM_STATE_CODE  14

RoomMemberRequestHandler::RoomMemberRequestHandler(int roomId, const LoggedUser& user, RequestHandlerFactory& handlerFactory)
    : m_roomId(roomId), m_user(user), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LEAVE_ROOM_CODE ||
        requestInfo.id == GET_ROOM_STATE_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    if (requestInfo.id == LEAVE_ROOM_CODE)
        return leaveRoom(requestInfo);
    if (requestInfo.id == GET_ROOM_STATE_CODE)
        return getRoomState(requestInfo);

    ErrorResponse err = { "Unknown request for Room Member" };
    return { JsonResponsePacketSerializer::serializeErrorResponse(err), nullptr };
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
    try
    {
        m_handlerFactory.getRoomManager().getRoom(m_roomId).removeUser(m_user.getUsername());
    }
    catch (...)
    {
    }

    LeaveRoomResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeLeaveRoomResponse(res),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    GetRoomStateResponse res;

    RoomStatus state = ROOM_CLOSE;
    try
    {
        Room& room = m_handlerFactory.getRoomManager().getRoom(m_roomId);
        RoomData metadata = room.getMetadata();
        state = m_handlerFactory.getRoomManager().getRoomState(m_roomId);

        res.status = (state == ROOM_CLOSE) ? 0 : 1;
        res.hasGameBegun = (state == ROOM_ACTIVE);
        res.players = room.getAllUsers();
        res.answerCount = metadata.numOfQuestionsInGame;
        res.answerTimeout = metadata.timePerQuestion;
    }
    catch (...)
    {
        res.status = 0;
        res.hasGameBegun = false;
        return { JsonResponsePacketSerializer::serializeGetRoomStateResponse(res), this };
    }

    IRequestHandler* nextHandler = this;
    if (state == ROOM_ACTIVE && m_handlerFactory.getGameManager().doesGameExist(m_roomId))
    {
        nextHandler = m_handlerFactory.createGameRequestHandler(m_user, m_roomId);
    }

    return { JsonResponsePacketSerializer::serializeGetRoomStateResponse(res), nextHandler };
}