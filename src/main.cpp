#include "headers/io_file.h"
#include "headers/openSSL_EVP.h"
#include "headers/openSSL_MD5.h"
#include "headers/openSSL_SHA256.h"
#include "headers/userMenu.h"
#include <format>
#include <iostream>

#define filePath "filePath"

int main(void) {
  io_hander io(filePath);
  openSSL_MD5 md5Hasher;
  openSSL_SHA256 sha256;
  EVP_Hash evpmd5(MD5_Hash);
  userMenu menu(io, md5Hasher);
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string md5Hash = md5Hasher.hashString(input);
  std::string shaHash = sha256.hashString(input);
  std::string evpHash = evpmd5.hashString(input);
  std::string evpHashTwo = evpmd5.hashString(input);
  evpmd5.switchHashMethod(SHA256_Hash);
  std::string evpSwitchTest = evpmd5.hashString(input);

  std::cout << std::format(
                   "Original: {}\nMD5: {}\nEVP: {}\nEVP: {}\nSHA256: {}\nSHASWAP: {}\n",
                   input, md5Hash, evpHash, evpHashTwo, shaHash, evpSwitchTest)
            << std::endl;
  return 0;
}
