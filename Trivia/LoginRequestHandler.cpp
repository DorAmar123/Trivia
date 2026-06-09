#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoggedUser.h"

#define LOGIN_REQUEST_CODE  1
#define SIGNUP_REQUEST_CODE 2

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory)
	: m_loginManager(loginManager), m_handlerFactory(handlerFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.id == LOGIN_REQUEST_CODE || requestInfo.id == SIGNUP_REQUEST_CODE;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	if (requestInfo.id == LOGIN_REQUEST_CODE)
		return login(requestInfo);
	if (requestInfo.id == SIGNUP_REQUEST_CODE)
		return signup(requestInfo);

	ErrorResponse err;
	err.message = "Unknown request";
	return { JsonResponsePacketSerializer::serializeErrorResponse(err), nullptr };
}

RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
	LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);

	LoginResponse res;
	IRequestHandler* nextHandler = nullptr;

	if (m_loginManager.login(req.username, req.password))
	{
		res.status = 1;
		nextHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(req.username));
	}
	else
	{
		res.status = 0;
	}

	return { JsonResponsePacketSerializer::serializeLoginResponse(res), nextHandler };
}

RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
	SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);

	SignupResponse res;
	res.status = m_loginManager.signup(req.username, req.password, req.email) ? 1 : 0;

	return { JsonResponsePacketSerializer::serializeSignupResponse(res), nullptr };
}