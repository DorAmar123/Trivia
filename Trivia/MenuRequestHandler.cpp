#include "MenuRequestHandler.h"

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info)
{
	return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
	return { {}, nullptr };
}
