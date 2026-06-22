#pragma once
#include <map>
#include <mutex>
#include "Game.h"
#include "Room.h"
#include "IDatabase.h"

class GameManager
{
public:
	GameManager(IDatabase* db);

	Game& createGame(Room& room);
	void deleteGame(int roomId);
	Game& getGame(int roomId);
	bool doesGameExist(int roomId) const;

private:
	IDatabase* m_database;
	std::map<int, Game> m_games;
	mutable std::mutex m_mutex;
};
