#pragma once
#include <vector>
#include <mutex>
#include "LoggedUser.h"
#include "Responses.h"

class Room
{
public:
	Room(const LoggedUser& creator, const RoomData& metadata);

	void addUser(const LoggedUser& user);
	void removeUser(const std::string& username);
	std::vector<std::string> getAllUsers() const;
	RoomData getMetadata() const;

private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	mutable std::mutex m_mutex;
};