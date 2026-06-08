#include "Room.h"
#include <algorithm>

Room::Room(const LoggedUser& creator, const RoomData& metadata)
	: m_metadata(metadata)
{
	m_users.push_back(creator);
}

void Room::addUser(const LoggedUser& user)
{
	m_users.push_back(user);
}

void Room::removeUser(const std::string& username)
{
	m_users.erase(
		std::remove_if(m_users.begin(), m_users.end(),
			[&username](const LoggedUser& u) { return u.getUsername() == username; }),
		m_users.end());
}

std::vector<std::string> Room::getAllUsers() const
{
	std::vector<std::string> names;
	for (const auto& user : m_users)
		names.push_back(user.getUsername());
	return names;
}

RoomData Room::getMetadata() const
{
	return m_metadata;
}