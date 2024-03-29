#ifndef userMenu_h
#define userMenu_h

#include "srcShared/io.h"
#include "srcShared/openSSL_EVP.h"
#include <optional>
#include <string>

class userMenu {
private:
  io &usersFile;
  EVP_Hash &hasher;

  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
  int validateInput(std::string input);
  bool validateUserName(const std::string &userName);
  bool validatePW(const std::string &password);
  std::string generateSalt(const std::string &userName);
  std::string getUserLine(const std::string &outputText);
  std::optional<std::string> findUser(const std::string &userName);
  bool verifyPW(const std::string &hash, const std::string &salt,
                const std::string &password);

public:
  userMenu(io &usersFilePath, EVP_Hash &hash)
      : usersFile(usersFilePath), hasher(hash){};
  void menu(void);
};

#endif // !userMenu_h
