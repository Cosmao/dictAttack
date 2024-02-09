#include "pwCracker/cracker.h"
#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"
#include <format>
#include <iostream>
#include <string>

//NOTE: I actually hate everything about this main function..
int main(int argc, char *argv[]) {
  std::cout << argc;
  if (argc == 3) {
    std::string filePath = argv[1];
    io_file fileToHash(filePath);
    io_file outputFile(filePath + "Hashed.txt");
    if (fileToHash.is_good() && outputFile.is_good()) {
      // NOTE: This is so awful, but lets me to the different hash methods
      hashMethods inputMethod = (hashMethods)std::stoi(argv[2]);
      fileToHash.resetStreamPos();
      outputFile.resetStreamPos();
      EVP_Hash hasher(inputMethod);
      while (fileToHash.hasLine()) {
        std::string line = fileToHash.readLine();
        std::string PW = line.substr(0, line.find_first_of(','));
        std::string sSalt = line.substr(line.find_first_of(',') + 1);
        std::string hash = hasher.hashString(PW + sSalt);
        outputFile.writeLine(std::format("{},{}", hash, sSalt));
      }
    }
    return 0;
  }
  if (argc != 5) {
    std::cout << "Requires 4 arguments, hashed passwords, common PW file,"
                 "number of threads and hashMethods(0-2)(0 = MD5, 1 = SHA256, 2 = SHA512)\n";
    std::cout << argc << "\n";
    return 0;
  }
  std::string argOne = argv[1];
  std::string argTwo = argv[2];
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
  const int numThreads = std::stoi(argv[3]);
  const hashMethods hashMethod = (hashMethods)std::stoi(argv[4]);

  crackThreadHandler(hashedPW, commonPW, numThreads, hashMethod);

  return 0;
}
