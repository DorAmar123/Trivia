#pragma once
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include "Question.h"
#include "Responses.h"

struct PlayerGameData
{
	unsigned int currentQuestionIndex;
	unsigned int correctAnswers;
	unsigned int wrongAnswers;
	float totalAnswerTime;
};

class Game
{
public:
	Game(const std::vector<Question>& questions, const std::vector<std::string>& players);

	GetQuestionResponse getQuestionForUser(const std::string& username);
	SubmitAnswerResponse submitAnswer(const std::string& username, unsigned int answerId, float timeSpent);
	GetGameResultsResponse getResults() const;
	bool isOver() const;
	PlayerGameData getPlayerData(const std::string& username) const;

private:
	std::vector<Question> m_questions;
	std::map<std::string, PlayerGameData> m_players;
	mutable std::mutex m_mutex;
};
