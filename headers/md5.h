#ifndef md5_h
#define md5_h

#include "headers/hashing.h"
#include <cstdint>

class md5 : public hashing {
private:
  static const std::uint32_t s[64];
  static const std::uint32_t k[64];
  static const std::uint32_t a0;
  static const std::uint32_t b0;
  static const std::uint32_t c0;
  static const std::uint32_t d0;

public:
  std::string hashString(std::string);
};

#endif // !md5_h
