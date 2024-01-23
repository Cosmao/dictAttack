#ifndef __openSSL_MD5_H
#define __openSSL_MD5_H

#include "headers/hashing.h"
#include <iomanip>
#include <openssl/md5.h>
#include <string>

class openSSL_MD5 : public hashing {
public:
  std::string hashString(const std::string &str) override;
};

#endif // !__openSSL_MD5_H
