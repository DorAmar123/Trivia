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

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	GetPlayersInRoomRequest req;
	req.roomId = j["roomId"].get<unsigned int>();

	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	JoinRoomRequest req;
	req.roomId = j["roomId"].get<unsigned int>();

	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	CreateRoomRequest req;
	req.roomName = j["roomName"].get<std::string>();
	req.maxUsers = j["maxUsers"].get<unsigned int>();
	req.questionCount = j["questionCount"].get<unsigned int>();
	req.answerTimeout = j["answerTimeout"].get<unsigned int>();

	return req;
}
