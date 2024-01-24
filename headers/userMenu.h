#ifndef userMenu_h
#define userMenu_h

#include "headers/openSSL_EVP.h"
#include "headers/io.h"
#include <iostream>
#include <string>

class userMenu {
  io &io_handler;
  EVP_Hash &hash;

  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
  int validateInput(std::string input);

public:
  userMenu(io &io, EVP_Hash &hash) : io_handler(io), hash(hash){};
  ~userMenu(void);
  void menu(void);
  void changeHashType(void);
};

#endif // !userMenu_h
