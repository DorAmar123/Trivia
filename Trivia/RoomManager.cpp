#include "RoomManager.h"
#include <stdexcept>

void RoomManager::createRoom(LoggedUser user, RoomData data)
{
    // Check if room ID already exists
    if (m_rooms.find(data.id) != m_rooms.end())
    {
        throw std::runtime_error("Room ID already exists");
    }

    Room newRoom(data);
    newRoom.addUser(user); // The creator is the first user in the room
    m_rooms.insert(std::pair<int, Room>(data.id, newRoom));
}

void RoomManager::deleteRoom(int ID)
{
    auto it = m_rooms.find(ID);
    if (it != m_rooms.end())
    {
        m_rooms.erase(it);
    }
    else
    {
        throw std::runtime_error("Room ID not found");
    }
}

unsigned int RoomManager::getRoomState(int ID) const
{
    auto it = m_rooms.find(ID);
    if (it != m_rooms.end())
    {
        return it->second.getRoomData().isActive;
    }
    throw std::runtime_error("Room ID not found");
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> allRooms;
    for (auto const& [id, room] : m_rooms)
    {
        allRooms.push_back(room.getRoomData());
    }
    return allRooms;
}

Room& RoomManager::getRoom(int ID)
{
    auto it = m_rooms.find(ID);
    if (it != m_rooms.end())
    {
        return it->second;
    }
    throw std::runtime_error("Room ID not found");
}