#include "headers/io_file.h"
#include "headers/openSSL_MD5.h"
#include "headers/userMenu.h"
#include <iostream>
#include <format>

#define filePath "filePath"

int main(void) {
  io_hander io(filePath);
  openSSL_MD5 md5Hasher;
  userMenu menu(io, md5Hasher);
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string hashedInput = md5Hasher.hashString(input);

  std::cout << std::format("Original: {}\nHashed: {}", input, hashedInput) << std::endl;
  return 0;
}
