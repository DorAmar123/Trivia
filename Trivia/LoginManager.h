#pragma once
#include <vector>
#include <string>
#include "IDatabase.h"
#include "LoggedUser.h"

class LoginManager
{
public:
	LoginManager(IDatabase* db);
	bool login(const std::string& username, const std::string& password);
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	void logout(const std::string& username);

private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	bool isUserLoggedIn(const std::string& username) const;
};
