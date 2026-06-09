#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <string>

class SqliteDataBase : public IDatabase
{
public:
    SqliteDataBase(const std::string& dbName);
    virtual ~SqliteDataBase();

    bool doesUserExist(const std::string& username) override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) override;
    void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

private:
    sqlite3* _db;
};