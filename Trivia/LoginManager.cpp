#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db)
{
	m_database = db;
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
	if (isUsernameValid(username) == false)
		return false;

	if (m_database->doesUserExist(username) == false)
		return false;

	if (m_database->doesPasswordMatch(username, password) == false)
		return false;

	if (isUserLoggedIn(username) == true)
		return false;

	LoggedUser user(username);
	m_loggedUsers.push_back(user);
	return true;
}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (isUsernameValid(username) == false)
		return false;

	if (m_database->doesUserExist(username) == true)
		return false;

	m_database->addNewUser(username, password, email);
	return true;
}

void LoginManager::logout(const std::string& username)
{
	for (unsigned int i = 0; i < m_loggedUsers.size(); i++)
	{
		if (m_loggedUsers[i].getUsername() == username)
		{
			m_loggedUsers.erase(m_loggedUsers.begin() + i);
			return;
		}
	}
}

bool LoginManager::isUserLoggedIn(const std::string& username) const
{
	for (unsigned int i = 0; i < m_loggedUsers.size(); i++)
	{
		if (m_loggedUsers[i].getUsername() == username)
			return true;
	}
	return false;
}

bool LoginManager::isUsernameValid(const std::string& username) const
{
	if (username.empty() == true)
		return false;

	if (username.length() > 20)
		return false;

	for (unsigned int i = 0; i < username.length(); i++)
	{
		char c = username[i];
		bool isLetter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		bool isDigit = (c >= '0' && c <= '9');
		bool isUnderscore = (c == '_');

		if (isLetter == false && isDigit == false && isUnderscore == false)
			return false;
	}
	return true;
}
