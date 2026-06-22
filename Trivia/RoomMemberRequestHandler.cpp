#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "Responses.h"

#define LEAVE_ROOM_CODE      13
#define GET_ROOM_STATE_CODE  14

RoomMemberRequestHandler::RoomMemberRequestHandler(Room& room, const LoggedUser& user, RequestHandlerFactory& handlerFactory)
    : m_room(room), m_user(user), m_handlerFactory(handlerFactory)
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
    m_room.removeUser(m_user.getUsername());

    LeaveRoomResponse res = { 1 };

    return { JsonResponsePacketSerializer::serializeLeaveRoomResponse(res),
             m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
    GetRoomStateResponse res;

    RoomData metadata = m_room.getMetadata();
    RoomStatus state = m_handlerFactory.getRoomManager().getRoomState(metadata.id);

    res.status = (state == ROOM_CLOSE) ? 0 : 1;
    res.hasGameBegun = (state == ROOM_ACTIVE);
    res.players = m_room.getAllUsers();
    res.answerCount = metadata.numOfQuestionsInGame;
    res.answerTimeout = metadata.timePerQuestion;

    return { JsonResponsePacketSerializer::serializeGetRoomStateResponse(res), this };
}