#pragma once
#include <vector>
#include "Requests.h"

class IRequestHandler;

struct RequestResult
{
	std::vector<unsigned char> response;
	IRequestHandler* newHandler;
};

class IRequestHandler
{
public:
	virtual ~IRequestHandler() = default;
	virtual bool isRequestRelevant(const RequestInfo& info) = 0;
	virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
