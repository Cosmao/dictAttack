#include "headers/userMenu.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
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

bool userMenu::attemptLogin(void) {
  std::string userName = this->getUserLine("Enter UserName: ");
  std::string line = this->findUser(userName);
  if (line == "") {
    std::cout << "Invalid username\n";
    return false;
  }
  std::string password = this->getUserLine("Enter Password: ");
  std::string salt = line.substr(line.find_last_of(',') + 1);
  std::string hash = line.substr(line.find_first_of(',') + 1,
                                 line.find_last_of(',') - userName.size() - 1);
  if (this->verifyPW(hash, salt, password)) {
    std::cout << "Valid login\n";
    return true;
  } else {
    std::cout << "Invalid password\n";
    return false;
  }
}

std::string userMenu::getUserLine(const std::string &outputText) {
  std::string inLine;
  std::cout << outputText;
  (void)std::getline(std::cin, inLine);
  return inLine;
}

// NOTE: can I use multiple predicate?
// not a huge fan of these loops, surely something better here
// single loop but idfk about it either
bool userMenu::validatePW(const std::string &password) {
#define uppercaseFlag 0x1
#define lowercaseFlag 0x2
#define numberFlag 0x4
#define specialFlag 0x8
  uint8_t flags = 0;
  if (password.size() < 8) {
    std::cout << "Password requires at least 8 characters\n";
    return false;
  }
  for (auto &c : password) {
    if (isupper(c)) {
      flags |= uppercaseFlag;
    } else if (isalpha(c) && !isupper(c)) {
      flags |= lowercaseFlag;
    } else if (isdigit(c)) {
      flags |= numberFlag;
    } else if (!(isdigit(c) || isalpha(c))) {
      flags |= specialFlag;
    }
    if (((flags & uppercaseFlag) == uppercaseFlag) &&
        ((flags & lowercaseFlag) == lowercaseFlag) &&
        ((flags & numberFlag) == numberFlag) &&
        ((flags & specialFlag) == specialFlag)) {
      return true;
    }
  }
  if (!((flags & uppercaseFlag) == uppercaseFlag)) {
    std::cout << "Password requires a upper case character\n";
  }
  if (!((flags & lowercaseFlag) == lowercaseFlag)) {
    std::cout << "Password requires a lower case characer\n";
  }
  if (!((flags & numberFlag) == numberFlag)) {
    std::cout << "Password requires a number\n";
  }
  if (!((flags & specialFlag) == specialFlag)) {
    std::cout << "Password requires a special character\n";
  }
  return false;
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
