#ifndef hashing_h
#define hashing_h

#include <string>

class hashing {
  virtual std::string hashString(const std::string &str) = 0;
};

#endif // !hashing_h
