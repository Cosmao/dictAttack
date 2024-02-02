#include "headers/io_file.h"
#include "headers/openSSL_EVP.h"
#include "headers/userMenu.h"
#include <chrono>
#include <iostream>
#include <thread>

#define filePath_usr "../../users.txt"

int main(void) {
  io_file users_list(filePath_usr);
  EVP_Hash EVP_Hasher(MD5_Hash);
  userMenu menu(users_list, EVP_Hasher);
  // menu.menu();

  const int numThreads = 16;
  const int numOfHashesh = 10000000;
  std::cout << std::format("Number of hashes to compute {}\n", numOfHashesh);

  std::vector<std::thread> threads(numThreads);
  const std::string hashToCrack = "WOW";

  auto lambda = [](int loops, const std::string &hash) {
    EVP_Hash hasher(SHA512_Hash);
    for (int i = 0; i < loops; i++) {
      if (hash == hasher.hashString("XD")) {
        std::cout << "Match found\n";
        return;
      }
    }
  };

  auto startThread = std::chrono::system_clock::now();
  for (auto &vThread : threads) {
    vThread = std::thread(lambda, numOfHashesh / numThreads, hashToCrack);
  }

  for (auto &vThread : threads) {
    vThread.join();
  }
  auto endThread = std::chrono::system_clock::now();
  auto elapsedThread = std::chrono::duration_cast<std::chrono::milliseconds>(
      endThread - startThread);
  std::cout << std::format("Time: {} milliseconds with {} threads\n",
                           elapsedThread.count(), numThreads);
  EVP_Hasher.switchHashMethod(SHA512_Hash);
  auto start = std::chrono::system_clock::now();
  for (int i = 0; i < numOfHashesh; i++) {
    if (hashToCrack == EVP_Hasher.hashString("XD")) {
      std::cout << "Match found\n";
    }
  }
  auto end = std::chrono::system_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << std::format("Time: {} milliseconds without threads\n",
                           elapsed.count());
  return 0;
}
