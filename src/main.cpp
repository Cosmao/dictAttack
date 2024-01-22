#include "headers/io.h"
#include "headers/openSSL_MD5.h"
#include "headers/userMenu.h"
#include <iostream>

#define filePath "filePath"


int main(void) {
  io_hander io = *new io_hander(filePath);
  userMenu menu = *new userMenu(io);
  openSSL_MD5 hash;
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string hashed = hash.hashString(input);

  std::cout << "Original: " << input << "\n";
  std::cout << "Hashed: " << hashed << "\n";

  return 0;
}
