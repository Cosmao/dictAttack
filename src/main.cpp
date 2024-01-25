#include "headers/io_file.h"
#include "headers/openSSL_EVP.h"
#include "headers/userMenu.h"
#include <format>
#include <iostream>

#define filePath "filePath"

int main(void) {
  io_file file(filePath);
  EVP_Hash EVP_Hasher(MD5_Hash);
  userMenu menu(file, EVP_Hasher);
  menu.menu();
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string evpHash = EVP_Hasher.hashString(input);
  std::string evpHashTwo = EVP_Hasher.hashString(input);
  EVP_Hasher.switchHashMethod(SHA256_Hash);
  std::string evpSwitchTest = EVP_Hasher.hashString(input);
  EVP_Hasher.switchHashMethod(MD5_Hash);
  std::string md5Again = EVP_Hasher.hashString(input);

  std::cout << std::format(
                   "Original: {}\nEVP: {}\nEVP: {}\nSHASWAP: {}\nMD5: {}",
                   input, evpHash, evpHashTwo, evpSwitchTest, md5Again)
            << std::endl;
  return 0;
}
