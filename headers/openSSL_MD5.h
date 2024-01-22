#ifndef __openSSL_MD5_H
#define __openSSL_MD5_H

#include "headers/hashing.h"
#include <string>
#include <iomanip>
#include <openssl/md5.h>

class openSSL_MD5 : public hashing {
public:
  std::string hashString(const std::string &str);
};

#endif // !__openSSL_MD5_H
