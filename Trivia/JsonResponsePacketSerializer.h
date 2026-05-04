#pragma once
#include <vector>
#include "Responses.h"

class JsonResponsePacketSerializer
{
public:
    static std::vector<unsigned char> serializeLoginResponse(const LoginResponse& response);
    static std::vector<unsigned char> serializeSignupResponse(const SignupResponse& response);
    static std::vector<unsigned char> serializeErrorResponse(const ErrorResponse& response);
};