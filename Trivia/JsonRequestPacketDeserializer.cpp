#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
    std::string jsonStr(buffer.begin(), buffer.end());
    json j = json::parse(jsonStr);

    LoginRequest req;
    req.username = j["username"].get<std::string>();
    req.password = j["password"].get<std::string>();

    return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char>& buffer)
{
    std::string jsonStr(buffer.begin(), buffer.end());
    json j = json::parse(jsonStr);

    SignupRequest req;
    req.username = j["username"].get<std::string>();
    req.password = j["password"].get<std::string>();
    req.email = j["email"].get<std::string>();

    return req;
}
