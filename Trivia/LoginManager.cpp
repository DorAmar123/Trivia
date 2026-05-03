#include "LoginManager.h"
#include <algorithm>

LoginManager::LoginManager(IDatabase* db) : m_database(db)
{
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
	if (!m_database->doesUserExist(username))
		return false;
	if (!m_database->doesPasswordMatch(username, password))
		return false;
	if (isUserLoggedIn(username))
		return false;

	m_loggedUsers.push_back(LoggedUser(username));
	return true;
}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (m_database->doesUserExist(username))
		return false;

	m_database->addNewUser(username, password, email);
	return true;
}

void LoginManager::logout(const std::string& username)
{
	m_loggedUsers.erase(
		std::remove_if(m_loggedUsers.begin(), m_loggedUsers.end(),
			[&username](const LoggedUser& u) { return u.getUsername() == username; }),
		m_loggedUsers.end());
}

bool LoginManager::isUserLoggedIn(const std::string& username) const
{
	for (const auto& u : m_loggedUsers)
		if (u.getUsername() == username)
			return true;
	return false;
}
