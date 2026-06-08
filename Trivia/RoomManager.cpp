#include "RoomManager.h"
#include <stdexcept>

void RoomManager::createRoom(const LoggedUser& creator, const RoomData& roomData)
{
	RoomData data = roomData;
	data.id = m_nextRoomId++;
	data.status = ROOM_OPEN;
	m_rooms.emplace(data.id, Room(creator, data));
}

void RoomManager::deleteRoom(int roomId)
{
	m_rooms.erase(roomId);
}

RoomStatus RoomManager::getRoomState(int roomId)
{
	auto it = m_rooms.find(roomId);
	if (it == m_rooms.end())
		throw std::runtime_error("Room not found");
	return it->second.getMetadata().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> rooms;
	for (const auto& pair : m_rooms)
		rooms.push_back(pair.second.getMetadata());
	return rooms;
}

Room& RoomManager::getRoom(int roomId)
{
	auto it = m_rooms.find(roomId);
	if (it == m_rooms.end())
		throw std::runtime_error("Room not found");
	return it->second;
}