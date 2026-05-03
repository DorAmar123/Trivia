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

struct RequestInfo
{
	int id;
	time_t receivalTime;
	std::vector<unsigned char> buffer;
};
