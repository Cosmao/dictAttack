#include "pwCracker/cracker.h"
#include "srcShared/io_file.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Requires 3 arguments, hashed passwords, common PW file and "
                 "number of threads\n";
    std::cout << argc << "\n";
    return 0;
  }
  std::string argOne = argv[1];
  std::string argTwo = argv[2];
  std::string argThree = argv[3];
  io_file commonPW(argTwo);
  io_file hashedPW(argOne);
  if (!commonPW.is_good()) {
    std::cout << "PW filepath bad!\n";
    return 0;
  } else {
    std::cout << "PW filepath good!\n";
  }
  if (!hashedPW.is_good()) {
    std::cout << "Hash filepath bad!\n";
    return 0;
  } else {
    std::cout << "Hash filepath good!\n";
  }
  const int numThreads = std::stoi(argThree);

  crackThreadHandler(hashedPW, commonPW, numThreads);

  return 0;
}
