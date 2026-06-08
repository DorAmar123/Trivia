#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* db) : m_database(db)
{
}

std::vector<std::string> StatisticsManager::getHighScore()
{
	return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
	std::vector<std::string> stats;
	stats.push_back("Average answer time: " + std::to_string(m_database->getPlayerAverageAnswerTime(username)));
	stats.push_back("Correct answers: " + std::to_string(m_database->getNumOfCorrectAnswers(username)));
	stats.push_back("Total answers: " + std::to_string(m_database->getNumOfTotalAnswers(username)));
	stats.push_back("Number of games: " + std::to_string(m_database->getNumOfPlayerGames(username)));
	stats.push_back("Score: " + std::to_string(m_database->getPlayerScore(username)));
	return stats;
}
