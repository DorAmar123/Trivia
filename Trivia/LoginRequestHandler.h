#pragma once
#include "IRequestHandler.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory* factory);
	~LoginRequestHandler() override = default;

	bool isRequestRelevant(const RequestInfo& info) override;
	RequestResult handleRequest(const RequestInfo& info) override;

private:
	RequestHandlerFactory* m_handlerFactory;

	RequestResult login(const RequestInfo& info);
	RequestResult signup(const RequestInfo& info);
};
