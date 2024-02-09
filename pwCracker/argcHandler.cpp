#include "pwCracker/argcHandler.h"
#include "pwCracker/cracker.h"
#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"
#include <format>
#include <iostream>

// NOTE: If I had more time I'd clean this up and make some proper error
// handling and flags for everything but it'll work for now

void hashFile(char **argv) {
  std::cout << "Hashing file\n";
  std::string filePath = argv[1];
  io_file fileToHash(filePath);
  io_file outputFile(filePath.substr(0, filePath.find_last_of('.')) +
                     "Hashed.txt");
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
  std::cout << "Hashing done\n";
  return;
}

void crackFile(char **argv) {
  std::cout << "Cracking file\n";
  std::string argOne = argv[1];
  std::string argTwo = argv[2];
  io_file commonPW(argTwo);
  io_file hashedPW(argOne);
  if (!commonPW.is_good()) {
    std::cout << "PW filepath bad!\n";
    return;
  } else {
    std::cout << "PW filepath good!\n";
  }
  if (!hashedPW.is_good()) {
    std::cout << "Hash filepath bad!\n";
    return;
  } else {
    std::cout << "Hash filepath good!\n";
  }
  const int numThreads = std::stoi(argv[3]);
  const hashMethods hashMethod = (hashMethods)std::stoi(argv[4]);
  crackThreadHandler(hashedPW, commonPW, numThreads, hashMethod);
  std::cout << "Cracking done\n";
  return;
}

void invalidArgs(void) {
  std::cout << "Invalid arguments\n";
  return;
}
