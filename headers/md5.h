#ifndef md5_h
#define md5_h

#include "headers/hashing.h"
#include <cstdint>

class md5 : public hashing {
private:
  static const std::uint32_t s[64];
  static const std::uint32_t k[64];
  std::uint32_t a0 = 0x67452301;
  std::uint32_t b0 = 0xefcdab89;
  std::uint32_t c0 = 0x98badcfe;
  std::uint32_t d0 = 0x10325476;

  std::uint32_t rotateLeft(std::uint32_t x, std::uint32_t n);

public:
  std::string hashString(std::string stringToHash);
};

#endif // !md5_h
