#ifndef userMenu_h
#define userMenu_h

#include "headers/io.h"
#include "headers/openSSL_EVP.h"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <format>
#include <iostream>
#include <regex>
#include <string>

class userMenu {
private:
  io &usersFile;
  EVP_Hash &hasher;

  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
  int validateInput(std::string input);
  void findPW(void);
  bool validateUserName(const std::string &userName);
  bool validatePW(const std::string &password);
  std::string generateSalt(void);
  std::string getUserLine(const std::string &outputText);
  std::string findUser(const std::string &userName);
  bool verifyPW(const std::string &hash, const std::string &salt,
                const std::string &password);

public:
  userMenu(io &usersFilePath, EVP_Hash &hash)
      : usersFile(usersFilePath), hasher(hash){};
  void menu(void);
};

#endif // !userMenu_h
