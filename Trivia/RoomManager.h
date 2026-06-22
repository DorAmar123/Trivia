#pragma once
#include <map>
#include <mutex>
#include "Room.h"

class RoomManager
{
public:
	int createRoom(const LoggedUser& creator, const RoomData& roomData);
	void deleteRoom(int roomId);
	RoomStatus getRoomState(int roomId);
	std::vector<RoomData> getRooms() const;
	Room& getRoom(int roomId);
	void setRoomState(int roomId, RoomStatus status);

private:
	std::map<int, Room> m_rooms;
	int m_nextRoomId = 1;
	mutable std::mutex m_mutex;
};