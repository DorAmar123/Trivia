#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <string>

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	~SqliteDatabase() override;

	bool open() override;
	bool close() override;

	bool doesUserExist(const std::string& username) override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	bool addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

	std::vector<Question> getQuestions(int count) override;

	float getPlayerAverageAnswerTime(const std::string& username) override;
	int getNumOfCorrectAnswers(const std::string& username) override;
	int getNumOfTotalAnswers(const std::string& username) override;
	int getNumOfPlayerGames(const std::string& username) override;
	int getPlayerScore(const std::string& username) override;
	std::vector<std::string> getHighScores() override;

private:
	sqlite3* m_db;
	const std::string m_dbFileName = "trivia.db";

	void createTables();
	bool isQuestionsTableEmpty();
	void addQuestionsFromApi();
	std::string httpGet(const std::wstring& host, const std::wstring& path);
	static std::string decodeHtml(const std::string& html);
};
