#include "headers/io_file.h"
#include "headers/openSSL_MD5.h"
#include "headers/openSSL_SHA256.h"
#include "headers/userMenu.h"
#include <iostream>
#include <format>

#define filePath "filePath"

int main(void) {
  io_hander io(filePath);
  openSSL_MD5 md5Hasher;
  openSSL_SHA256 sha256;
  userMenu menu(io, md5Hasher);
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string md5Hash = md5Hasher.hashString(input);
  std::string shaHash = sha256.hashString(input);

  std::cout << std::format("Original: {}\nMD5: {}\nSHA256: {}\n", input, md5Hash, shaHash) << std::endl;
  return 0;
}
