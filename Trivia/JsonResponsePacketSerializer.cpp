#include "JsonResponsePacketSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

#define LOGIN_CODE               1
#define SIGNUP_CODE              2
#define ERROR_CODE               3
#define LOGOUT_CODE              4
#define GET_ROOMS_CODE           5
#define GET_PLAYERS_IN_ROOM_CODE 6
#define JOIN_ROOM_CODE           7
#define CREATE_ROOM_CODE         8
#define GET_HIGH_SCORE_CODE      9
#define GET_PERSONAL_STATS_CODE  10
#define CLOSE_ROOM_CODE          11
#define START_GAME_CODE          12
#define LEAVE_ROOM_CODE          13
#define GET_ROOM_STATE_CODE      14
#define LEAVE_GAME_CODE          15
#define GET_QUESTION_CODE        16
#define SUBMIT_ANSWER_CODE       17
#define GET_GAME_RESULTS_CODE    18
							     
std::vector<unsigned char> JsonResponsePacketSerializer::buildPacket(unsigned char code, const std::string& jsonStr)
{
	std::vector<unsigned char> buffer;
	buffer.push_back(code);

	int size = (int)jsonStr.size();
	for (int i = 3; i >= 0; i--)
		buffer.push_back((size >> (i * 8)) & 0xFF);

	for (unsigned char c : jsonStr)
		buffer.push_back(c);

	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(LOGIN_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(SIGNUP_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& response)
{
	json j;
	j["message"] = response.message;
	return buildPacket(ERROR_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLogoutResponse(const LogoutResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(LOGOUT_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomsResponse(const GetRoomsResponse& response)
{
	json j;
	j["status"] = response.status;
	json rooms = json::array();
	for (const RoomData& room : response.rooms)
	{
		json r;
		r["id"] = room.id;
		r["name"] = room.name;
		r["maxPlayers"] = room.maxPlayers;
		r["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
		r["timePerQuestion"] = room.timePerQuestion;
		r["status"] = room.status;
		rooms.push_back(r);
	}
	j["Rooms"] = rooms;
	return buildPacket(GET_ROOMS_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(const GetPlayersInRoomResponse& response)
{
	json j;
	j["PlayersInRoom"] = response.players;
	return buildPacket(GET_PLAYERS_IN_ROOM_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeJoinRoomResponse(const JoinRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(JOIN_ROOM_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCreateRoomResponse(const CreateRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(CREATE_ROOM_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetHighScoreResponse(const GetHighScoreResponse& response)
{
	json j;
	j["status"] = response.status;
	j["HighScores"] = response.statistics;
	return buildPacket(GET_HIGH_SCORE_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(const GetPersonalStatsResponse& response)
{
	json j;
	j["status"] = response.status;
	j["UserStatistics"] = response.statistics;
	return buildPacket(GET_PERSONAL_STATS_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeCloseRoomResponse(const CloseRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(CLOSE_ROOM_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeStartGameResponse(const StartGameResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(START_GAME_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLeaveRoomResponse(const LeaveRoomResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(LEAVE_ROOM_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetRoomStateResponse(const GetRoomStateResponse& response)
{
	json j;
	j["status"] = response.status;
	j["hasGameBegun"] = response.hasGameBegun;
	j["players"] = response.players;
	j["answerCount"] = response.answerCount;
	j["answerTimeout"] = response.answerTimeout;
	return buildPacket(GET_ROOM_STATE_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLeaveGameResponse(const LeaveGameResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(LEAVE_GAME_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSubmitAnswerResponse(const SubmitAnswerResponse& response)
{
	json j;
	j["status"] = response.status;
	return buildPacket(SUBMIT_ANSWER_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetQuestionResponse(const GetQuestionResponse& response)
{
	json j;
	j["status"] = response.status;
	j["question"] = response.question;
	json answers;
	for (const auto& pair : response.answers)
		answers[std::to_string(pair.first)] = pair.second;
	j["answers"] = answers;
	return buildPacket(GET_QUESTION_CODE, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeGetGameResultsResponse(const GetGameResultsResponse& response)
{
	json j;
	j["status"] = response.status;
	json results = json::array();
	for (const PlayerResult& pr : response.results)
	{
		json r;
		r["username"] = pr.username;
		r["correctAnswerCount"] = pr.correctAnswerCount;
		r["wrongAnswerCount"] = pr.wrongAnswerCount;
		r["averageAnswerTime"] = pr.averageAnswerTime;
		results.push_back(r);
	}
	j["results"] = results;
	return buildPacket(GET_GAME_RESULTS_CODE, j.dump());
}