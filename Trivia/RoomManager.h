#pragma once
#include <map>
#include "Room.h"

class RoomManager
{
public:
	void createRoom(const LoggedUser& creator, const RoomData& roomData);
	void deleteRoom(int roomId);
	RoomStatus getRoomState(int roomId);
	std::vector<RoomData> getRooms() const;
	Room& getRoom(int roomId);

private:
	std::map<int, Room> m_rooms;
	int m_nextRoomId = 1;
};