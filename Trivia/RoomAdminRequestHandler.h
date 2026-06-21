#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
    RoomAdminRequestHandler(Room& room, const LoggedUser& user, RequestHandlerFactory& handlerFactory);

    virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    Room& m_room;
    LoggedUser m_user;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult closeRoom(const RequestInfo& requestInfo);
    RequestResult startGame(const RequestInfo& requestInfo);
    RequestResult getRoomState(const RequestInfo& requestInfo);
};