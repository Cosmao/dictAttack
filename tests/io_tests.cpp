#include "headers/io_file.h"
#include <catch2/catch_test_macros.hpp>

#define filePath "text.txt"

TEST_CASE("Create and read file") {
  {
    io_file test_file(filePath);
    REQUIRE(test_file.hasLine() == false);
    test_file.resetStreamPos();
    REQUIRE(test_file.writeLine("Testing") == true);
    test_file.resetStreamPos();
    REQUIRE(test_file.hasLine() == true);
    REQUIRE(test_file.readLine() == "Testing");
    REQUIRE(test_file.hasLine() == false);
  }
  REQUIRE(std::remove(filePath) == 0);
}
