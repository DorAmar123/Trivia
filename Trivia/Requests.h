#pragma once
#include <string>
#include <vector>
#include <ctime>

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct CloseRoomRequest
{
};

struct StartGameRequest
{
};

struct GetRoomStateRequest
{
};

struct LeaveRoomRequest
{
<<<<<<< HEAD
=======
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
};

struct LeaveGameRequest
{
};

struct GetQuestionRequest
{
};

struct GetGameResultRequest
{
>>>>>>> d28f809 (Add v4 request structs and SubmitAnswer deserialization)
};