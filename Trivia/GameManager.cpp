#include "GameManager.h"

GameManager::GameManager(IDatabase* db) : m_database(db)
{
}

Game& GameManager::createGame(Room& room)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	RoomData metadata = room.getMetadata();
	std::vector<Question> questions = m_database->getQuestions(metadata.numOfQuestionsInGame);
	std::vector<std::string> players = room.getAllUsers();

	m_games.emplace(metadata.id, Game(questions, players));
	return m_games.at(metadata.id);
}

void GameManager::deleteGame(int roomId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_games.erase(roomId);
}

Game& GameManager::getGame(int roomId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_games.at(roomId);
}

bool GameManager::doesGameExist(int roomId) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_games.find(roomId) != m_games.end();
}
