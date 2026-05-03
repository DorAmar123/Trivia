#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include <iostream>

#define LOGIN_CODE  1
#define SIGNUP_CODE 2

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* factory) : m_handlerFactory(factory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info)
{
	return info.id == LOGIN_CODE || info.id == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
	if (info.id == LOGIN_CODE)
		return login(info);
	return signup(info);
}

RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
	LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	std::cout << "Login attempt: " << req.username << std::endl;

	if (m_handlerFactory->getLoginManager().login(req.username, req.password))
	{
		LoginResponse res;
		res.status = 1;
		return { JsonResponsePacketSerializer::serializeLoginResponse(res),
				 m_handlerFactory->createMenuRequestHandler() };
	}

	ErrorResponse err;
	err.message = "Wrong username or password";
	return { JsonResponsePacketSerializer::serializeErrorResponse(err),
			 m_handlerFactory->createLoginRequestHandler() };
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
	SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	std::cout << "Signup attempt: " << req.username << std::endl;

	if (m_handlerFactory->getLoginManager().signup(req.username, req.password, req.email))
	{
		SignupResponse res;
		res.status = 1;
		return { JsonResponsePacketSerializer::serializeSignupResponse(res),
				 m_handlerFactory->createMenuRequestHandler() };
	}

	ErrorResponse err;
	err.message = "Username already exists";
	return { JsonResponsePacketSerializer::serializeErrorResponse(err),
			 m_handlerFactory->createLoginRequestHandler() };
}
