#ifndef openSSL_EVP_h
#define openSSL_EVP_h

#include <iomanip>
#include <openssl/evp.h>
#include <string>

enum hashMethods { MD5_Hash, SHA256_Hash, SHA512_Hash };

class EVP_Hash {
private:
  int currentHashMethod = 0;
  const EVP_MD *hashMethod;

public:
  explicit EVP_Hash(hashMethods hashMethod);
  void switchHashMethod(hashMethods hashMethod);
  int getCurrentHashMethod(void);
  std::string hashString(const std::string &stringToHash);
};

#endif // !openSSL_EVP_h
