#ifndef userMenu_h
#define userMenu_h

#include "headers/hashing.h"
#include "headers/io.h"
#include "headers/md5.h"

class userMenu {
  io_hander &io;
  // TODO: reference instead maybe?
  hashing *hash;

public:
  userMenu(io_hander &io) : io(io){};
  bool attemptLogin(void);
  bool createUser(void);
  bool selectHash(void);
};

#endif // !userMenu_h
