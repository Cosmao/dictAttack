#include "headers/openSSL_EVP.h"

EVP_Hash::EVP_Hash(hashMethods hashMethod) {
  switch (hashMethod) {
  case MD5_Hash:
    this->hashMethod = EVP_get_digestbyname("md5");
    break;
  case SHA256_Hash:
    this->hashMethod = EVP_get_digestbyname("sha256");
    break;
  case SHA512_Hash:
    this->hashMethod = EVP_get_digestbyname("sha512");
    break;
  }
}

void EVP_Hash::switchHashMethod(hashMethods hashMethod) {
  switch (hashMethod) {
  case MD5_Hash:
    this->hashMethod = EVP_get_digestbyname("md5");
    break;
  case SHA256_Hash:
    this->hashMethod = EVP_get_digestbyname("sha256");
    break;
  case SHA512_Hash:
    this->hashMethod = EVP_get_digestbyname("sha512");
    break;
  }
}

std::string EVP_Hash::hashString(const std::string &stringToHash) {
  EVP_MD_CTX *mdctx;
  std::vector<unsigned char> digest(EVP_MD_size(this->hashMethod));
  mdctx = EVP_MD_CTX_new();
  EVP_DigestInit(mdctx, this->hashMethod);
  EVP_DigestUpdate(mdctx, stringToHash.c_str(), stringToHash.size());
  EVP_DigestFinal_ex(mdctx, digest.data(), nullptr);

  std::ostringstream formattedHash;
  formattedHash << std::hex << std::setfill('0');

  for (const auto &i : digest) {
    formattedHash << std::setw(2) << static_cast<unsigned int>(i);
  }

  EVP_MD_CTX_free(mdctx);
  return formattedHash.str();
}
