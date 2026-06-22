#include "Game.h"

Game::Game(const std::vector<Question>& questions, const std::vector<std::string>& players)
	: m_questions(questions)
{
	for (const std::string& player : players)
		m_players[player] = { 0, 0, 0, 0.0f };
}

GetQuestionResponse Game::getQuestionForUser(const std::string& username)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	GetQuestionResponse res;
	auto it = m_players.find(username);
	if (it == m_players.end())
	{
		res.status = 0;
		return res;
	}
	PlayerGameData& data = it->second;

	if (data.currentQuestionIndex >= m_questions.size())
	{
		res.status = 0;
		return res;
	}

	const Question& q = m_questions[data.currentQuestionIndex];
	res.status = 1;
	res.question = q.getQuestion();

	std::vector<std::string> answers = q.getPossibleAnswers();
	for (unsigned int i = 0; i < answers.size(); i++)
		res.answers[i + 1] = answers[i];

	return res;
}

SubmitAnswerResponse Game::submitAnswer(const std::string& username, unsigned int answerId, float timeSpent)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	SubmitAnswerResponse res;
	auto it = m_players.find(username);
	if (it == m_players.end())
	{
		res.status = 0;
		return res;
	}
	PlayerGameData& data = it->second;

	if (data.currentQuestionIndex >= m_questions.size())
	{
		res.status = 0;
		return res;
	}

	const Question& q = m_questions[data.currentQuestionIndex];

	if ((int)answerId == q.getCorrectAnswerId())
		data.correctAnswers++;
	else
		data.wrongAnswers++;

	data.totalAnswerTime += timeSpent;
	data.currentQuestionIndex++;

	res.status = 1;
	res.correctAnswerId = q.getCorrectAnswerId();
	return res;
}

GetGameResultsResponse Game::getResults() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	GetGameResultsResponse res;

	if (!isOver())
	{
		res.status = 0;
		return res;
	}

	res.status = 1;
	for (const auto& pair : m_players)
	{
		const PlayerGameData& data = pair.second;
		unsigned int answered = data.correctAnswers + data.wrongAnswers;

		PlayerResult pr;
		pr.username = pair.first;
		pr.correctAnswerCount = data.correctAnswers;
		pr.wrongAnswerCount = data.wrongAnswers;
		pr.averageAnswerTime = (answered > 0) ? (data.totalAnswerTime / answered) : 0.0f;
		res.results.push_back(pr);
	}

	return res;
}

PlayerGameData Game::getPlayerData(const std::string& username) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_players.find(username);
	if (it == m_players.end())
		return { 0, 0, 0, 0.0f };
	return it->second;
}

bool Game::isOver() const
{
	if (m_players.empty())
		return false;

	for (const auto& pair : m_players)
		if (pair.second.currentQuestionIndex < m_questions.size())
			return false;
	return true;
}
