#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
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
	try
	{
		if (info.id == LOGIN_CODE)
			return login(info);
		return signup(info);
	}
	catch (std::exception& e)
	{
		std::cout << "Error handling request: " << e.what() << std::endl;
		RequestResult result;
		ErrorResponse err;
		err.message = "Server error";
		result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
		result.newHandler = nullptr;
		return result;
	}
}

RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
	RequestResult result;
	LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	std::cout << "Login attempt: " << req.username << std::endl;

	bool ok = m_handlerFactory->getLoginManager().login(req.username, req.password);
	if (ok == true)
	{
		LoginResponse res;
		res.status = 1;
		result.response = JsonResponsePacketSerializer::serializeLoginResponse(res);
		result.newHandler = m_handlerFactory->createMenuRequestHandler();
		return result;
	}

	ErrorResponse err;
	err.message = "Wrong username or password";
	result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
	result.newHandler = m_handlerFactory->createLoginRequestHandler();
	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
	RequestResult result;
	SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	std::cout << "Signup attempt: " << req.username << std::endl;

	bool ok = m_handlerFactory->getLoginManager().signup(req.username, req.password, req.email);
	if (ok == true)
	{
		SignupResponse res;
		res.status = 1;
		result.response = JsonResponsePacketSerializer::serializeSignupResponse(res);
		result.newHandler = m_handlerFactory->createMenuRequestHandler();
		return result;
	}

	ErrorResponse err;
	err.message = "Username already exists";
	result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
	result.newHandler = m_handlerFactory->createLoginRequestHandler();
	return result;
}
