#include "RoomManager.h"
#include <stdexcept>

void RoomManager::createRoom(const LoggedUser& creator, const RoomData& roomData)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	RoomData data = roomData;
	data.id = m_nextRoomId++;
	data.status = ROOM_OPEN;
	m_rooms.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(data.id),
		std::forward_as_tuple(creator, data)
	);
}

void RoomManager::deleteRoom(int roomId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_rooms.erase(roomId);
}

RoomStatus RoomManager::getRoomState(int roomId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_rooms.find(roomId);
	if (it == m_rooms.end())
		throw std::runtime_error("Room not found");
	return it->second.getMetadata().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::vector<RoomData> rooms;
	for (const auto& pair : m_rooms)
		rooms.push_back(pair.second.getMetadata());
	return rooms;
}

Room& RoomManager::getRoom(int roomId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_rooms.find(roomId);
	if (it == m_rooms.end())
		throw std::runtime_error("Room not found");
	return it->second;
}
