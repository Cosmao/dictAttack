#ifndef userMenu_h
#define userMenu_h

#include "headers/hashing.h"
#include "headers/io.h"
#include <iostream>
#include <string>

class userMenu {
  io_hander &io;
  hashing &hash;

  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
  int validateInput(std::string input);

public:
  userMenu(io_hander &io, hashing &hash) : io(io), hash(hash){};
  ~userMenu(void);
  void menu(void);
  void changeHashType(void);
};

#endif // !userMenu_h
