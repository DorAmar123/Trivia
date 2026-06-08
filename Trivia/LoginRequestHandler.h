#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory);
	~LoginRequestHandler() override = default;

	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
	RequestResult login(const RequestInfo& requestInfo);
	RequestResult signup(const RequestInfo& requestInfo);

	LoginManager& m_loginManager;
	RequestHandlerFactory& m_handlerFactory;
};