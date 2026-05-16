#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "RoomData.h"
#include "LoggedUser.h"

class Room 
{
public:
    Room(RoomData data);

    void addUser(LoggedUser user);
    void removeUser(LoggedUser user);
    std::vector<std::string> getAllUsers() const;

    RoomData getRoomData() const;

private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
};