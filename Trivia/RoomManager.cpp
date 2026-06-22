#include "RoomManager.h"
#include <stdexcept>

int RoomManager::createRoom(const LoggedUser& creator, const RoomData& roomData)
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
	return data.id;
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
	{
		RoomData data = pair.second.getMetadata();
		if (data.status == ROOM_OPEN)
			rooms.push_back(data);
	}
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

void RoomManager::setRoomState(int roomId, RoomStatus status)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_rooms.find(roomId);
	if (it != m_rooms.end())
	{
		it->second.setStatus(status);
	}
}
