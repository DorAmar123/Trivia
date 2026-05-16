#include "Room.h"

Room::Room(RoomData data) : m_metadata(data)
{
}

void Room::addUser(LoggedUser user)
{
    m_users.push_back(user);
}

void Room::removeUser(LoggedUser user)
{
    for (auto it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->getUsername() == user.getUsername())
        {
            m_users.erase(it);
            break;
        }
    }
}

std::vector<std::string> Room::getAllUsers() const
{
    std::vector<std::string> userNames;
    for (const auto& user : m_users)
    {
        userNames.push_back(user.getUsername());
    }
    return userNames;
}

RoomData Room::getRoomData() const
{
    return m_metadata;
}