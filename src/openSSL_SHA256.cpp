#include "headers/openSSL_SHA256.h"

std::string openSSL_SHA256::hashString(const std::string &stringToHash) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, stringToHash.c_str(), stringToHash.size());
  SHA256_Final(hash, &sha256);
  std::stringstream ss;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return ss.str();
}
