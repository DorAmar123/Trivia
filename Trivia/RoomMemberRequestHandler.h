#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:
    RoomMemberRequestHandler(int roomId, const LoggedUser& user, RequestHandlerFactory& handlerFactory);

    virtual bool isRequestRelevant(const RequestInfo& requestInfo) override;
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) override;
    std::string getLoggedUsername() const override { return m_user.getUsername(); }

private:
    int m_roomId;
    LoggedUser m_user;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult leaveRoom(const RequestInfo& requestInfo);
    RequestResult getRoomState(const RequestInfo& requestInfo);
};