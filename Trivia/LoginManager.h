#pragma once
#include <vector>
#include <string>
#include <mutex>
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
	mutable std::mutex m_mutex;

	bool isUserLoggedIn(const std::string& username) const;
	bool isUsernameValid(const std::string& username) const;
};
