#include "headers/myMD5.h"
#include <cstdint>
#include <iostream>

const std::uint32_t myMD5::s[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

const std::uint32_t myMD5::k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

std::uint32_t myMD5::rotateLeft(std::uint32_t x, std::uint32_t n) {
  return (x << n) | (x >> (32 - n));
}

std::string myMD5::hashString(const std::string &stringToHash) {
  std::string hash;
  std::string letter = "a";
  const char *array = letter.c_str();
  std::uint32_t test = array[0] << (8*3 - 1);// | 1 << ((8 * 2) + 7);
  test = 0x61008000;
  std::cout << std::hex << test << "\n";

  #define len 14+16
  std::uint32_t m[len];
  for (int i = 1; i < len; i++) {
    m[i] = 0;
  }
  m[0] = test;//1 << 31;
  std::uint32_t a, b, c, d;
  d = this->c0;
  c = this->b0;
  b = this->a0;
  a = this->d0;

  for (int i = 0; i < 64; i++) {
    std::uint32_t f, g;
    if (i < 16) {
      f = (b & c) | ((~b) & d);
      g = i;
    } else if (i < 32) {
      f = (d & b) | ((~d) & c);
      g = ((5 * i) + 1) % 16;
    } else if (i < 48) {
      f = b ^ c ^ d;
      g = ((3 * i) + 5) % 16;
    } else {
      f = c ^ (b | (~d));
      g = ((7 * i)) % 16;
    }
    f = f + a + this->k[i] + m[g];
    a = d;
    d = c;
    c = b;
    b += this->rotateLeft(f, this->s[i]);
  }

  this->a0 = this->a0 + a;
  this->b0 = this->b0 + b;
  this->c0 = this->c0 + c;
  this->d0 = this->d0 + d;

  std::cout << std::hex << this->a0 << std::hex << this->b0 << std::hex
            << this->c0 << std::hex << this->d0 << "\n"
            << std::endl;

  return stringToHash; 
}
