#include <catch2/catch_test_macros.hpp>
#include "headers/openSSL_EVP.h"

TEST_CASE("EVP md5 Hash"){
  EVP_Hash evp_test(MD5_Hash);
  std::string empty = "";
  std::string helloWorld = "Hello World";
  REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == evp_test.hashString(empty));
  REQUIRE("b10a8db164e0754105b7a99be72e3fe5" == evp_test.hashString(helloWorld));
}

TEST_CASE("EVP sha256 Hash"){
  EVP_Hash evp_test(SHA256_Hash);
  std::string empty = "";
  std::string helloWorld = "Hello World";
  REQUIRE("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" == evp_test.hashString(empty));
  REQUIRE("a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e" == evp_test.hashString(helloWorld));
}

TEST_CASE("EVP hash swapping"){
  EVP_Hash evp_test(MD5_Hash);
  std::string empty = "";
  std::string helloWorld = "Hello World";
  REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == evp_test.hashString(empty));
  REQUIRE("b10a8db164e0754105b7a99be72e3fe5" == evp_test.hashString(helloWorld));
  evp_test.switchHashMethod(SHA256_Hash);
  REQUIRE("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" == evp_test.hashString(empty));
  REQUIRE("a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e" == evp_test.hashString(helloWorld));
  evp_test.switchHashMethod(MD5_Hash);
  REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == evp_test.hashString(empty));
  REQUIRE("b10a8db164e0754105b7a99be72e3fe5" == evp_test.hashString(helloWorld));
}
