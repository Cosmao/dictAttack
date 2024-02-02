#include "headers/io_file.h"
#include "headers/openSSL_EVP.h"
#include <format>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define filePath_usr "../../users.txt"
#define filePath_commonPW "../../rockyou.txt"
#define filePath_hashedPW "../../hashedPW.txt"

void crack(hashMethods hashMethod, const std::string &hashedPW,
           const std::vector<std::string> &commonPW, int start, int end,
           volatile bool &foundPW, std::mutex &boolMutex);
void hashFile(hashMethods hashMethod, io_file &fileToHash,
              io_file &fileToStore);

int main(void) {
  io_file commonPW(filePath_commonPW);
  io_file hashedPW(filePath_hashedPW);
  const int numThreads = 16;

  // NOTE: ONLY RUN ONCE
  //  hashFile(MD5_Hash, commonPW, hashedPW);

  std::vector<std::string> pw;
  commonPW.resetStreamPos();
  hashedPW.resetStreamPos();
  while (commonPW.hasLine()) {
    std::string line = commonPW.readLine();
    pw.push_back(line);
  }

  int amountPerThread = pw.size() / numThreads;
  std::mutex boolMutex;
  std::vector<std::thread> threads(numThreads);
  int i = 0;
  while (hashedPW.hasLine()) {
    volatile bool foundPW = false;
    int start = 0;
    int end = start + amountPerThread;
    std::string hash = hashedPW.readLine();

    for (auto &vThread : threads) {
      vThread = std::thread(crack, MD5_Hash, hash, pw, start, end,
                            std::ref(foundPW), std::ref(boolMutex));
      start += amountPerThread;
    }
    std::cout << std::format("Created {} threads\n", threads.size());

    for (auto &vThread : threads) {
      vThread.join();
    }
    if (i > 10) {
      break;
    }
    i++;
  }

  return 0;
}

void hashFile(hashMethods hashMethod, io_file &fileToHash,
              io_file &fileToStore) {

  // Ingest
  std::vector<std::string> pw;
  fileToHash.resetStreamPos();
  fileToStore.resetStreamPos();
  while (fileToHash.hasLine()) {
    std::string line = fileToHash.readLine();
    pw.push_back(line);
  }
  EVP_Hash hashClass(hashMethod);
  for (auto &password : pw) {
    fileToStore.writeLine(hashClass.hashString(password));
  }
  std::cout << std::format("Hashed {} lines\n", pw.size());
  return;
}

void crack(hashMethods hashMethod, const std::string &hashedPW,
           const std::vector<std::string> &commonPW, int start, int end,
           volatile bool &foundPW, std::mutex &boolMutex) {
  EVP_Hash hasher(hashMethod);
  std::cout << std::format("Start: {} End: {} Bool: {}\n", start, end, foundPW ? "True" : "False");
  for (int i = start; i < end; i++) {
    if (hashedPW == hasher.hashString(commonPW.at(i))) {
      std::cout << std::format("Hash: {}\nPassword: {}\n", hashedPW,
                               commonPW.at(i));
      boolMutex.lock();
      foundPW = true;
      boolMutex.unlock();
      return;
    }
    if (foundPW == true) {
      std::cout << "early exit\n";
      return;
    }
  }
  return;
}
