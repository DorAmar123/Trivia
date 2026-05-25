#include "SqliteDataBase.h"
#include <iostream>

static int foundRowCallback(void* data, int argc, char** argv, char** azColName)
{
	bool* found = (bool*)data;
	*found = true;
	return 0;
}

SqliteDataBase::SqliteDataBase(const std::string& dbName)
{
	_db = nullptr;
	int res = sqlite3_open(dbName.c_str(), &_db);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		throw std::exception("Failed to open database");
	}

	std::string create = "CREATE TABLE IF NOT EXISTS USERS (USERNAME TEXT PRIMARY KEY NOT NULL, PASSWORD TEXT NOT NULL, EMAIL TEXT NOT NULL);";
	char* err = nullptr;
	int result = sqlite3_exec(_db, create.c_str(), nullptr, nullptr, &err);
	if (result != SQLITE_OK)
	{
		std::cout << "SQL error: " << err << std::endl;
		sqlite3_free(err);
	}
}

SqliteDataBase::~SqliteDataBase()
{
	if (_db != nullptr)
		sqlite3_close(_db);
}

bool SqliteDataBase::doesUserExist(const std::string& username)
{
	bool exists = false;
	std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";
	char* err = nullptr;

	int res = sqlite3_exec(_db, sql.c_str(), foundRowCallback, &exists, &err);
	if (res != SQLITE_OK)
	{
		std::cout << "SQL error: " << err << std::endl;
		sqlite3_free(err);
	}
	return exists;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	bool match = false;
	std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";
	char* err = nullptr;

	int res = sqlite3_exec(_db, sql.c_str(), foundRowCallback, &match, &err);
	if (res != SQLITE_OK)
	{
		std::cout << "SQL error: " << err << std::endl;
		sqlite3_free(err);
	}
	return match;
}

void SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	if (username.empty() || password.empty() || email.empty())
	{
		throw std::runtime_error("Username, password, or email cannot be empty");
	}

	std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "', '" + email + "');";
	char* err = nullptr;

	int res = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err);
	if (res != SQLITE_OK)
	{
		std::cout << "SQL error: " << err << std::endl;
		sqlite3_free(err);
	}
}
