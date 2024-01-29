#ifndef userMenu_h
#define userMenu_h

#include "headers/io.h"
#include "headers/openSSL_EVP.h"
#include <iostream>
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
  std::string getUserLine(const std::string &outputText);
  std::string findUser(const std::string &userName);
  bool verifyPW(const std::string &hash, const std::string &salt,
                const std::string &password);
  // void crackPW(const std::string &hashStr, const std::string &saltStr);

public:
  userMenu(io &usersFilePath, EVP_Hash &hash)
      : usersFile(usersFilePath), hasher(hash){};
  ~userMenu(void);
  void menu(void);
  void changeHashType(void);
};

#endif // !userMenu_h
