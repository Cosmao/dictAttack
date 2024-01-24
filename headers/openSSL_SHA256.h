#ifndef openSSL_SHA256_h
#define openSSL_SHA256_h

#include "headers/hashing.h"
#include <string>
#include <openssl/sha.h>
#include <iostream>
#include <iomanip>

class openSSL_SHA256 : public hashing {
public:
  std::string hashString(const std::string &stringToHash) override;
};

#endif // !openSSL_SHA256_h
