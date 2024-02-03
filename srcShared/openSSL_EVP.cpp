#include "openSSL_EVP.h"

EVP_Hash::EVP_Hash(hashMethods hashMethod) {
  switch (hashMethod) {
  case MD5_Hash:
    this->hashMethod = EVP_get_digestbyname("md5");
    this->currentHashMethod = MD5_Hash;
    break;
  case SHA256_Hash:
    this->hashMethod = EVP_get_digestbyname("sha256");
    this->currentHashMethod = SHA256_Hash;
    break;
  case SHA512_Hash:
    this->hashMethod = EVP_get_digestbyname("sha512");
    this->currentHashMethod = SHA512_Hash;
    break;
  }
}

void EVP_Hash::switchHashMethod(hashMethods hashMethod) {
  switch (hashMethod) {
  case MD5_Hash:
    this->hashMethod = EVP_get_digestbyname("md5");
    this->currentHashMethod = MD5_Hash;
    break;
  case SHA256_Hash:
    this->hashMethod = EVP_get_digestbyname("sha256");
    this->currentHashMethod = SHA256_Hash;
    break;
  case SHA512_Hash:
    this->hashMethod = EVP_get_digestbyname("sha512");
    this->currentHashMethod = SHA512_Hash;
    break;
  }
}

std::string EVP_Hash::hashString(const std::string &stringToHash) {
  EVP_MD_CTX *mdctx;
  std::string result;
  std::vector<unsigned char> digest(EVP_MD_size(this->hashMethod));
  mdctx = EVP_MD_CTX_new();
  EVP_DigestInit(mdctx, this->hashMethod);
  EVP_DigestUpdate(mdctx, stringToHash.c_str(), stringToHash.size());
  EVP_DigestFinal_ex(mdctx, digest.data(), nullptr);

  result = this->stringifyDigest(digest);
  EVP_MD_CTX_free(mdctx);
  return result;
}

hashMethods EVP_Hash::getCurrentHashMethod(void){
  return this->currentHashMethod;
}

std::string EVP_Hash::stringifyDigest(const std::vector<unsigned char> &charVec) {
  std::string hash;
  const char *arr = "0123456789abcdef";
  for (const auto &v : charVec) {
    unsigned char nibble;
    nibble = v >> 4;
    hash.push_back(arr[nibble]);
    nibble = v & 0x0F;
    hash.push_back(arr[nibble]);
  }
  return hash;
}
