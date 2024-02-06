#include "pwCracker/cracker.h"
#include "srcShared/openSSL_EVP.h"
#include <format>
#include <iostream>
#include <thread>

#define limit 4

//TODO: Dont delete the threads, maybe just give new info and signal that its ready
// Dont need to recreate the EVP_HASH as well then
void crackThreadHandler(io_file &hashedPW, io_file &commonPW,
                        const int numThreads) {
  std::vector<std::string> pw;
  hashedPW.resetStreamPos();
  commonPW.resetStreamPos();
  while (commonPW.hasLine()) {
    std::string line = commonPW.readLine();
    pw.push_back(line);
  }
  int amountPerThread = pw.size() / numThreads;
  int limiter = 0;
  std::mutex boolMutex;
  std::vector<std::thread> threads(numThreads);
  while (hashedPW.hasLine()) {
    volatile bool foundPW = false;
    int start = 0;
    int end = start + amountPerThread;
    std::string hash = hashedPW.readLine();
    for (auto &vThread : threads) {
      vThread = std::thread(crackFunc, MD5_Hash, hash, pw, start, end,
                            std::ref(foundPW), std::ref(boolMutex));
      start += amountPerThread;
      end += amountPerThread;
    }
    for (auto &vThread : threads) {
      vThread.join();
    }
    if (limiter == limit) {
      return;
    }
    limiter++;
  }
  return;
}

void crackFunc(hashMethods hashMethod, const std::string &hashedPW,
               const std::vector<std::string> &commonPW, int start, int end,
               volatile bool &foundPW, std::mutex &boolMutex) {
  EVP_Hash hasher(hashMethod);
  for (int i = start; i < end; i++) {
    if (hashedPW == hasher.hashString(commonPW.at(i))) {
      std::cout << std::format("Hash: {}\nPassword: {}\nLoops: {}\n", hashedPW,
                               commonPW.at(i), i - start);
      std::scoped_lock lock(boolMutex);
      foundPW = true;
      return;
    } else if (foundPW == true) {
      return;
    }
  }
  return;
}
