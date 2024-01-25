#ifndef userMenu_h
#define userMenu_h

#include "headers/io.h"
#include "headers/openSSL_EVP.h"
#include <iostream>
#include <string>

class userMenu {
private:
  io &io_hashed_list;
  io &io_common_PW_list;
  EVP_Hash &hash;

  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
  int validateInput(std::string input);
  void findPW(void);
  void crackPW(const std::string &hashStr, const std::string &saltStr);

public:
  userMenu(io &io_hashed, io &io_pw_list, EVP_Hash &hash)
      : io_hashed_list(io_hashed), io_common_PW_list(io_pw_list), hash(hash){};
  ~userMenu(void);
  void menu(void);
  void changeHashType(void);
};

#endif // !userMenu_h
