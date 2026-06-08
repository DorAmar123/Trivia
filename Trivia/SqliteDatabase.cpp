#include "SqliteDatabase.h"
#include "json.hpp"
#include <windows.h>
#include <winhttp.h>
#include <stdexcept>
#include <vector>

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

SqliteDatabase::SqliteDatabase() : m_db(nullptr)
{
	open();
	createTables();
	if (isQuestionsTableEmpty())
		addQuestionsFromApi();
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}

bool SqliteDatabase::open()
{
	return sqlite3_open(m_dbFileName.c_str(), &m_db) == SQLITE_OK;
}

bool SqliteDatabase::close()
{
	if (m_db)
	{
		int rc = sqlite3_close(m_db);
		m_db = nullptr;
		return rc == SQLITE_OK;
	}
	return true;
}

void SqliteDatabase::createTables()
{
	const char* createUsers =
		"CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";

	const char* createQuestions =
		"CREATE TABLE IF NOT EXISTS questions (id INTEGER PRIMARY KEY AUTOINCREMENT, question TEXT NOT NULL, answer1 TEXT NOT NULL, answer2 TEXT NOT NULL, answer3 TEXT NOT NULL, answer4 TEXT NOT NULL, correctAnswerId INTEGER NOT NULL);";

	const char* createStatistics =
		"CREATE TABLE IF NOT EXISTS statistics (username TEXT PRIMARY KEY, correctAnswers INTEGER DEFAULT 0, totalAnswers INTEGER DEFAULT 0, avgAnswerTime REAL DEFAULT 0, numGames INTEGER DEFAULT 0, score INTEGER DEFAULT 0);";

	sqlite3_exec(m_db, createUsers, nullptr, nullptr, nullptr);
	sqlite3_exec(m_db, createQuestions, nullptr, nullptr, nullptr);
	sqlite3_exec(m_db, createStatistics, nullptr, nullptr, nullptr);
}

bool SqliteDatabase::doesUserExist(const std::string& username)
{
	const char* sql = "SELECT COUNT(*) FROM users WHERE username = ?;";
	sqlite3_stmt* stmt;
	bool exists = false;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			exists = sqlite3_column_int(stmt, 0) > 0;
		sqlite3_finalize(stmt);
	}
	return exists;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	const char* sql = "SELECT COUNT(*) FROM users WHERE username = ? AND password = ?;";
	sqlite3_stmt* stmt;
	bool match = false;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			match = sqlite3_column_int(stmt, 0) > 0;
		sqlite3_finalize(stmt);
	}
	return match;
}

bool SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	const char* sql = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";
	sqlite3_stmt* stmt;
	bool ok = false;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
		ok = sqlite3_step(stmt) == SQLITE_DONE;
		sqlite3_finalize(stmt);
	}
	return ok;
}

std::vector<Question> SqliteDatabase::getQuestions(int count)
{
	std::vector<Question> questions;
	const char* sql =
		"SELECT question, answer1, answer2, answer3, answer4, correctAnswerId "
		"FROM questions ORDER BY RANDOM() LIMIT ?;";

	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_int(stmt, 1, count);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::string q = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			std::vector<std::string> answers = {
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
				reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))
			};
			int correctId = sqlite3_column_int(stmt, 5);
			questions.emplace_back(q, answers, correctId);
		}
		sqlite3_finalize(stmt);
	}
	return questions;
}

bool SqliteDatabase::isQuestionsTableEmpty()
{
	const char* sql = "SELECT COUNT(*) FROM questions;";
	sqlite3_stmt* stmt;
	bool empty = true;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
			empty = sqlite3_column_int(stmt, 0) == 0;
		sqlite3_finalize(stmt);
	}
	return empty;
}

void SqliteDatabase::addQuestionsFromApi()
{
	std::string response = httpGet(L"opentdb.com", L"/api.php?amount=10&type=multiple");
	if (response.empty())
		return;

	json data = json::parse(response, nullptr, false);
	if (data.is_discarded() || data["response_code"] != 0)
		return;

	const char* sql =
		"INSERT INTO questions (question, answer1, answer2, answer3, answer4, correctAnswerId) "
		"VALUES (?, ?, ?, ?, ?, ?);";

	for (const auto& item : data["results"])
	{
		std::string question = decodeHtml(item["question"].get<std::string>());
		std::string correctAnswer = decodeHtml(item["correct_answer"].get<std::string>());
		auto incorrectAnswers = item["incorrect_answers"].get<std::vector<std::string>>();

		// Correct answer always stored at index 0
		std::vector<std::string> answers = { correctAnswer };
		for (const auto& a : incorrectAnswers)
			answers.push_back(decodeHtml(a));

		sqlite3_stmt* stmt;
		if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
		{
			sqlite3_bind_text(stmt, 1, question.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, answers[0].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, answers[1].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, answers[2].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 5, answers[3].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 6, 0);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}
}

std::string SqliteDatabase::httpGet(const std::wstring& host, const std::wstring& path)
{
	std::string result;

	HINTERNET hSession = WinHttpOpen(L"TriviaServer/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) return result;

	HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (!hConnect) { WinHttpCloseHandle(hSession); return result; }

	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(),
		nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return result; }

	if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
		WINHTTP_NO_REQUEST_DATA, 0, 0, 0) && WinHttpReceiveResponse(hRequest, nullptr))
	{
		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		do
		{
			dwSize = 0;
			WinHttpQueryDataAvailable(hRequest, &dwSize);
			if (!dwSize) break;
			std::vector<char> buf(dwSize);
			WinHttpReadData(hRequest, buf.data(), dwSize, &dwDownloaded);
			result.append(buf.data(), dwDownloaded);
		} while (dwSize > 0);
	}

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
	return result;
}

std::string SqliteDatabase::decodeHtml(const std::string& html)
{
	std::string s = html;
	size_t pos;
	auto replace = [&](const std::string& from, const std::string& to)
		{
			while ((pos = s.find(from)) != std::string::npos)
				s.replace(pos, from.size(), to);
		};
	replace("&amp;", "&");
	replace("&lt;", "<");
	replace("&gt;", ">");
	replace("&quot;", "\"");
	replace("&#039;", "'");
	replace("&apos;", "'");
	return s;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	const char* sql = "SELECT avgAnswerTime FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	float result = 0.0f;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = (float)sqlite3_column_double(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	const char* sql = "SELECT correctAnswers FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int result = 0;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	const char* sql = "SELECT totalAnswers FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int result = 0;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	const char* sql = "SELECT numGames FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int result = 0;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

int SqliteDatabase::getPlayerScore(const std::string& username)
{
	const char* sql = "SELECT score FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int result = 0;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			result = sqlite3_column_int(stmt, 0);
		sqlite3_finalize(stmt);
	}
	return result;
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
	const char* sql = "SELECT username, score FROM statistics ORDER BY score DESC LIMIT 5;";
	sqlite3_stmt* stmt;
	std::vector<std::string> results;

	if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			int score = sqlite3_column_int(stmt, 1);
			results.push_back(username + ": " + std::to_string(score));
		}
		sqlite3_finalize(stmt);
	}
	return results;
}
