#pragma once
#include <map>
#include <vector>
#include <string>
#include "Room.h"

class RoomManager 
{
public:
    void createRoom(LoggedUser user, RoomData data);
    void deleteRoom(int ID);
    unsigned int getRoomState(int ID) const;
    std::vector<RoomData> getRooms() const;
    Room& getRoom(int ID);

private:
    std::map<int, Room> m_rooms;
};