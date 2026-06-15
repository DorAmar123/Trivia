#include "SqliteDatabase.h"
#include "json.hpp"
#include <iostream>
#include <sstream>
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

SqliteDatabase::SqliteDatabase() : m_db(nullptr)
{
	open();
	createTables();
	if (queryInt("SELECT COUNT(*) FROM questions;") == 0)
		addQuestionsFromApi();
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}

bool SqliteDatabase::open()
{
	int res = sqlite3_open("trivia.db", &m_db);
	return res == SQLITE_OK;
}

bool SqliteDatabase::close()
{
	if (m_db)
	{
		sqlite3_close(m_db);
		m_db = nullptr;
	}
	return true;
}

void SqliteDatabase::createTables()
{
	const char* createUsers = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";
	const char* createQuestions = "CREATE TABLE IF NOT EXISTS questions (id INTEGER PRIMARY KEY AUTOINCREMENT, question TEXT NOT NULL, answer1 TEXT NOT NULL, answer2 TEXT NOT NULL, answer3 TEXT NOT NULL, answer4 TEXT NOT NULL, correctAnswerId INTEGER NOT NULL);";
	const char* createStatistics = "CREATE TABLE IF NOT EXISTS statistics (username TEXT PRIMARY KEY, correctAnswers INTEGER DEFAULT 0, totalAnswers INTEGER DEFAULT 0, avgAnswerTime REAL DEFAULT 0, numGames INTEGER DEFAULT 0, score INTEGER DEFAULT 0);";

	runQuery(createUsers);
	runQuery(createQuestions);
	runQuery(createStatistics);
}

bool SqliteDatabase::doesUserExist(const std::string& username)
{
	std::string q = "SELECT COUNT(*) FROM users WHERE username = '" + username + "';";
	return queryInt(q) > 0;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	std::string q = "SELECT COUNT(*) FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
	return queryInt(q) > 0;
}

bool SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string q = "INSERT INTO users (username, password, email) VALUES ('" + username + "', '" + password + "', '" + email + "');";
	int res = runQuery(q);
	if (res == SQLITE_OK)
	{
		std::string stats = "INSERT OR IGNORE INTO statistics (username) VALUES ('" + username + "');";
		runQuery(stats);
		return true;
	}
	return false;
}

std::vector<Question> SqliteDatabase::getQuestions(int count)
{
	std::vector<Question> questions;
	std::string q = "SELECT question, answer1, answer2, answer3, answer4, correctAnswerId FROM questions LIMIT " + std::to_string(count) + ";";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db, q.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return questions;

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string question = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		std::vector<std::string> answers = {
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
			reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))
		};
		int correctId = sqlite3_column_int(stmt, 5);
		questions.emplace_back(question, answers, correctId);
	}
	sqlite3_finalize(stmt);
	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	std::string q = "SELECT avgAnswerTime FROM statistics WHERE username = '" + username + "';";
	return queryFloat(q);
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	std::string q = "SELECT correctAnswers FROM statistics WHERE username = '" + username + "';";
	return queryInt(q);
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	std::string q = "SELECT totalAnswers FROM statistics WHERE username = '" + username + "';";
	return queryInt(q);
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	std::string q = "SELECT numGames FROM statistics WHERE username = '" + username + "';";
	return queryInt(q);
}

int SqliteDatabase::getPlayerScore(const std::string& username)
{
	std::string q = "SELECT score FROM statistics WHERE username = '" + username + "';";
	return queryInt(q);
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
	std::vector<std::string> scores;
	std::string q = "SELECT username, score FROM statistics ORDER BY score DESC LIMIT 5;";

	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_db, q.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		return scores;

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		int score = sqlite3_column_int(stmt, 1);
		scores.push_back(username + ": " + std::to_string(score));
	}
	sqlite3_finalize(stmt);
	return scores;
}

std::string SqliteDatabase::fetchQuestionsJson()
{
	std::string result;
	HINTERNET hSession = WinHttpOpen(L"TriviaClient/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) return result;

	HINTERNET hConnect = WinHttpConnect(hSession, L"opentdb.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (!hConnect) { WinHttpCloseHandle(hSession); return result; }

	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/api.php?amount=10&type=multiple", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return result; }

	if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
		WinHttpReceiveResponse(hRequest, nullptr))
	{
		DWORD bytesAvail = 0;
		while (WinHttpQueryDataAvailable(hRequest, &bytesAvail) && bytesAvail > 0)
		{
			std::vector<char> buf(bytesAvail + 1, 0);
			DWORD bytesRead = 0;
			WinHttpReadData(hRequest, buf.data(), bytesAvail, &bytesRead);
			result += buf.data();
		}
	}

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
	return result;
}

std::string SqliteDatabase::decodeHtml(const std::string& text)
{
	std::string out = text;
	struct { const char* from; const char* to; } replacements[] = {
		{"&amp;", "&"}, {"&lt;", "<"}, {"&gt;", ">"}, {"&quot;", "\""}, {"&#039;", "'"}, {"&apos;", "'"}
	};
	for (auto& r : replacements)
	{
		size_t pos = 0;
		while ((pos = out.find(r.from, pos)) != std::string::npos)
		{
			out.replace(pos, strlen(r.from), r.to);
			pos += strlen(r.to);
		}
	}
	return out;
}

void SqliteDatabase::addQuestionsFromApi()
{
	try
	{
		std::string jsonStr = fetchQuestionsJson();
		if (jsonStr.empty()) return;

		json data = json::parse(jsonStr);
		if (data["response_code"] != 0) return;

		for (auto& item : data["results"])
		{
			std::string question = decodeHtml(item["question"].get<std::string>());
			std::string correct = decodeHtml(item["correct_answer"].get<std::string>());

			std::vector<std::string> wrong;
			for (auto& w : item["incorrect_answers"])
				wrong.push_back(decodeHtml(w.get<std::string>()));

			while (wrong.size() < 3)
				wrong.push_back("N/A");

			std::string q = "INSERT INTO questions (question, answer1, answer2, answer3, answer4, correctAnswerId) VALUES ('" +
				question + "', '" + correct + "', '" + wrong[0] + "', '" + wrong[1] + "', '" + wrong[2] + "', 1);";
			runQuery(q);
		}
	}
	catch (...) {}
}

int SqliteDatabase::runQuery(const std::string& query)
{
	char* err = nullptr;
	int res = sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &err);
	if (err) sqlite3_free(err);
	return res;
}

bool SqliteDatabase::queryBool(const std::string& query)
{
	return queryInt(query) != 0;
}

int SqliteDatabase::queryInt(const std::string& query)
{
	sqlite3_stmt* stmt = nullptr;
	int result = 0;
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

float SqliteDatabase::queryFloat(const std::string& query)
{
	sqlite3_stmt* stmt = nullptr;
	float result = 0.0f;
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = (float)sqlite3_column_double(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

std::string SqliteDatabase::queryString(const std::string& query)
{
	sqlite3_stmt* stmt = nullptr;
	std::string result;
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			if (text) result = text;
		}
		sqlite3_finalize(stmt);
	}
	return result;
}
