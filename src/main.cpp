#include "headers/io.h"
#include "headers/md5.h"
#include "headers/userMenu.h"
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>

#define filePath "filePath"

std::string md5(const std::string &str){
  unsigned char hash[MD5_DIGEST_LENGTH];

  MD5_CTX md5;
  MD5_Init(&md5);
  MD5_Update(&md5, str.c_str(), str.size());
  MD5_Final(hash, &md5);

  std::stringstream ss;

  for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(
    hash[i] );
  }
  return ss.str();
}

int main(void) {
  io_hander io = *new io_hander(filePath);
  userMenu menu = *new userMenu(io);
  std::string input = "The quick brown fox jumps over the lazy dog";
  std::string hashed = md5(input);

  std::cout << "Original: " << input << "\n";
  std::cout << "Hashed: " << hashed << "\n";

  return 0;
}
