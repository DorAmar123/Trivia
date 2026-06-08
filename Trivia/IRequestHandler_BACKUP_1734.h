#pragma once
#include <vector>
<<<<<<< HEAD
#include "Requests.h"
=======
#include "JsonRequestPacketDeserializer.h"
>>>>>>> origin/feature/v2-menu-handler

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
<<<<<<< HEAD
	virtual bool isRequestRelevant(const RequestInfo& info) = 0;
	virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
=======
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};
>>>>>>> origin/feature/v2-menu-handler
