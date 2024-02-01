#include "headers/userMenu.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <format>
#include <iostream>
#include <regex>

#define addUser 1
#define login 2
#define quit 3

void userMenu::menu(void) {
  std::string input;
  while (1) {
    std::cout << "1. Add User\n2. Login\n3. Exit\n";
    (void)std::getline(std::cin, input);
    switch (this->validateInput(input)) {
    case addUser:
      if (this->createUser()) {
        std::cout << "Successfully created account\n";
      } else {
        std::cout << "Account creation failed\n";
      }
      break;
    case login:
      (void)this->attemptLogin();
      break;
    case quit:
      return;
      break;
    default:
      std::cout << "Invalid input\n";
      break;
    }
  }
  return;
}

int userMenu::validateInput(std::string input) {
  int result = 0;
  size_t index = input.find_first_of("0123456789");
  if (index != std::string::npos) {
    result = std::stoi(input.substr(index, 1));
  }
  return result;
}

// TODO: Clean this shit up
bool userMenu::attemptLogin(void) {
  std::string userName;
  std::cout << "Enter Username: ";
  (void)std::getline(std::cin, userName);
  std::string line = this->findUser(userName);
  if (line.size() > 0) {
    std::string hash =
        line.substr(line.find_first_of(',') + 1,
                    line.find_last_of(',') - userName.size() - 1);
    std::string salt = line.substr(line.find_last_of(',') + 1);
    std::string password;
    std::cout << "Enter password: ";
    (void)std::getline(std::cin, password);
    if (this->verifyPW(hash, salt, password)) {
      std::cout << "Valid login\n";
      return true;
    }
    std::cout << "Invalid credentials\n";
    return false;
  }
  return false;
}

std::string userMenu::getUserLine(const std::string &outputText) {
  std::string inLine;
  std::cout << outputText;
  (void)std::getline(std::cin, inLine);
  return inLine;
}

// NOTE: can I use multiple predicate?
// not a huge fan of these loops, surely something better here
bool userMenu::validatePW(const std::string &password) {
  if (password.size() < 8) {
    std::cout << "Password requires at least 8 characters\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return std::isupper(c); })) {
    std::cout << "Password requires a upper case character\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return !std::isupper(c); })) {
    std::cout << "Password requires a lower case character\n";
    return false;
  }
  if (!std::ranges::any_of(password,
                           [](const char &c) { return std::isdigit(c); })) {
    std::cout << "Password requires a number\n";
    return false;
  }
  if (!std::ranges::any_of(password, [](const char &c) {
        if (std::isdigit(c) || std::isalpha(c)) {
          return false;
        }
        return true;
      })) {
    std::cout << "Password requires a special character\n";
    return false;
  }
  return true;
}

bool userMenu::validateUserName(const std::string &userName) {
  if (this->findUser(userName) != "") {
    std::cout << "Username is taken\n";
    return false;
  }
  if (std::ranges::any_of(userName, [](const char &c) { return c == ','; })) {
    std::cout << "Disallowed character \",\" \n";
    return false;
  }
  // NOTE: Regex works for now, probably not very quick in the end
  std::regex emailRegex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
  if (!std::regex_search(userName, emailRegex)) {
    std::cout << "Username needs to be an email address\n";
    return false;
  }
  return true;
}

std::string userMenu::generateSalt(const std::string &userName) {
  auto oldHashMethod = this->hasher.getCurrentHashMethod();
  this->hasher.switchHashMethod(MD5_Hash);
  std::string salt = this->hasher.hashString(
      std::format("Salt for user {}, created {}", userName,
                  std::chrono::system_clock::now()));
  this->hasher.switchHashMethod(oldHashMethod);
  return salt;
}

// NOTE: I honestly dont like how you get stuck in a loop until you successfully
// make an account but I dont want you to have to retype and get sent back to
// menu all the time either
bool userMenu::createUser(void) {
  std::string userName;
  do {
    userName = getUserLine("Enter desired Username: ");
  } while (!this->validateUserName(userName));
  std::string password;
  do {
    password = getUserLine("Enter password: ");
  } while (!this->validatePW(password));
  std::string salt = this->generateSalt(userName);
  return usersFile.writeLine(std::format(
      "{},{},{}", userName, hasher.hashString(password + salt), salt));
}

std::string userMenu::findUser(const std::string &userName) {
  usersFile.resetStreamPos();
  while (usersFile.hasLine()) {
    std::string line = usersFile.readLine();
    std::string user = line.substr(0, line.find_first_of(','));
    if (user == userName) {
      usersFile.resetStreamPos();
      return line;
    }
  }
  usersFile.resetStreamPos();
  return "";
}

bool userMenu::verifyPW(const std::string &hash, const std::string &salt,
                        const std::string &password) {
  std::string strToHash = password + salt;
  if (hash == this->hasher.hashString(strToHash)) {
    return true;
  }
  return false;
}
