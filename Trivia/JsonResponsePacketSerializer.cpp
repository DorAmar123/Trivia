#include "JsonResponsePacketSerializer.h"

#include "json.hpp"
using json = nlohmann::json;

#define LOGIN_CODE 1
#define SIGNUP_CODE 2
#define ERROR_CODE 3

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response)
{
    std::vector<unsigned char> buffer;

    json j;
    j["status"] = response.status;

    std::string jsonStr = j.dump();
    // code
    buffer.push_back(LOGIN_CODE);

    // size (4 bytes)
    int size = jsonStr.size();
    for (int i = 3; i >= 0; i--)
    {
        buffer.push_back((size >> (i * 8)) & 0xFF);
    }

    // data
    for (unsigned int i = 0; i < jsonStr.size(); i++)
    {
        buffer.push_back(jsonStr[i]);
    }

    return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response)
{
    std::vector<unsigned char> buffer;

    json j;
    j["status"] = response.status;

    std::string jsonStr = j.dump();

    buffer.push_back(SIGNUP_CODE);

    int size = jsonStr.size();
    for (int i = 3; i >= 0; i--)
    {
        buffer.push_back((size >> (i * 8)) & 0xFF);
    }

    for (unsigned int i = 0; i < jsonStr.size(); i++)
    {
        buffer.push_back(jsonStr[i]);
    }

    return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& response)
{
    std::vector<unsigned char> buffer;

    json j;
    j["message"] = response.message;

    std::string jsonStr = j.dump();
    buffer.push_back(ERROR_CODE);

    int size = jsonStr.size();
    for (int i = 3; i >= 0; i--)
    {
        buffer.push_back((size >> (i * 8)) & 0xFF);
    }

    for (unsigned int i = 0; i < jsonStr.length(); i++)
    {
        buffer.push_back(jsonStr[i]);
    }

    return buffer;
}