#include <catch2/catch_test_macros.hpp>
#include "headers/openSSL_EVP.h"

TEST_CASE("EVP md5 Hash"){
  EVP_Hash evp_test(MD5_Hash);
  std::string empty = "";
  REQUIRE("d41d8cd98f00b204e9800998ecf8427e" == evp_test.hashString(empty));
  std::string helloWorld = "Hello World";
  REQUIRE("b10a8db164e0754105b7a99be72e3fe5" == evp_test.hashString(helloWorld));
}
