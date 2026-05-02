#include "SqliteDataBase.h"
#include <stdexcept>

SqliteDataBase::SqliteDataBase() 
{
    int result = sqlite3_open("TriviaDB.sqlite", &_db);
    if (result != SQLITE_OK) 
    {
        _db = nullptr;
        throw std::runtime_error("Failed to open SQLite database");
    }

    std::string sqlStatement = "CREATE TABLE IF NOT EXISTS USERS (USERNAME TEXT PRIMARY KEY NOT NULL, PASSWORD TEXT NOT NULL, EMAIL TEXT NOT NULL);";
    executeSQL(sqlStatement);
}

SqliteDataBase::~SqliteDataBase() 
{
    sqlite3_close(_db);
}

bool SqliteDataBase::doesUserExist(const std::string& username) 
{
    bool exists = false;
    std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "';";

    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        *(bool*)data = true;
        return 0;
        };

    sqlite3_exec(_db, sql.c_str(), callback, &exists, nullptr);
    return exists;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password) 
{
    bool match = false;
    std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";

    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        *(bool*)data = true;
        return 0;
        };

    sqlite3_exec(_db, sql.c_str(), callback, &match, nullptr);
    return match;
}

void SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email) 
{
    std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "', '" + email + "');";
    executeSQL(sql);
}

void SqliteDataBase::executeSQL(const std::string& sql)
{
    char* errMessage = nullptr;
    int result = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errMessage);
    if (result != SQLITE_OK) 
    {
        std::string error = "SQL Error: ";
        error += errMessage;
        sqlite3_free(errMessage);
        throw std::runtime_error(error);
    }
}