#include "pwCracker/cracker.h"
#include "srcShared/openSSL_EVP.h"
#include <chrono>
#include <condition_variable>
#include <format>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unistd.h>

#define limit 4000

void crackThreadHandler(io_file &hashedPW, io_file &commonPW,
                        const int numThreads, const hashMethods hashMethod) {
  std::vector<std::string> pw;
  hashedPW.resetStreamPos();
  commonPW.resetStreamPos();
  std::cout << "Reading in text to hash\n";
  while (commonPW.hasLine()) {
    std::string line = commonPW.readLine();
    pw.push_back(line);
  }

  std::unique_ptr<crackResult> result(new crackResult);
  std::shared_ptr<threadInfo> tInfo(new threadInfo);

  tInfo->result = new std::unique_ptr<crackResult>(new crackResult);
  tInfo->tInfoMutex = new std::unique_ptr<std::mutex>(new std::mutex());
  tInfo->condMutex = new std::unique_ptr<std::mutex>(new std::mutex());
  tInfo->nextHashSharedMutex =
      new std::unique_ptr<std::shared_mutex>(new std::shared_mutex());
  tInfo->threadWorkingMutex =
      new std::unique_ptr<std::shared_mutex>(new std::shared_mutex());
  tInfo->condThreadStartedWorking =
      new std::unique_ptr<std::condition_variable>(
          new std::condition_variable());
  tInfo->condAnyNextHashAvailable =
      new std::unique_ptr<std::condition_variable_any>(
          new std::condition_variable_any());
  tInfo->contine = true;
  tInfo->foundPW = false;

  std::vector<std::thread> threads(numThreads);
  int start = 0;
  int amountPerThread = pw.size() / numThreads;
  int end = start + amountPerThread;
  // NOTE: This will fail if you give it more threads than commonPW!
  for (auto &t : threads) {
    t = std::thread(crackFunc, hashMethod, std::ref(pw), start, end, tInfo);
    start += amountPerThread + 1;
    end += amountPerThread + 1;
    if ((unsigned long)end >= pw.size()) {
      end = pw.size() - 1;
    }
  }
  int numCrackedPW = 0;
  int numFailedCrackedPW = 0;
  // NOTE: Cba to make a proper signal that all threads are ready to start
  // recieving data so just a small sleep
  sleep(1);
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> cLock(*tInfo->condMutex->get());
  while (hashedPW.hasLine()) {
    std::string line = hashedPW.readLine();
    std::string sHash = line.substr(0, line.find_first_of(','));
    std::string sSalt = line.substr(line.find_first_of(',') + 1);
    {
      std::scoped_lock<std::mutex> lock(*tInfo->tInfoMutex->get());
      tInfo->foundPW = false;
      tInfo->hash = &sHash;
      tInfo->salt = &sSalt;
    }
    tInfo->condAnyNextHashAvailable->get()->notify_all();
    tInfo->condThreadStartedWorking->get()->wait(cLock);
    tInfo->nextHashSharedMutex->get()->lock();
    tInfo->threadWorkingMutex->get()->lock();
    if (tInfo->foundPW) {
      std::cout << std::format("Hash: {}\nPW: {}\nLoops: {}\n", sHash,
                               *tInfo->result->get()->password,
                               tInfo->result->get()->loops);
      free(tInfo->result->get()->password);
      numCrackedPW++;
    } else {
      std::cout << std::format("Couldnt find PW for hash: {}\n", sHash);
      numFailedCrackedPW++;
    }
    tInfo->threadWorkingMutex->get()->unlock();
    tInfo->nextHashSharedMutex->get()->unlock();
  }

  {
    std::scoped_lock<std::mutex> lock(*tInfo->tInfoMutex->get());
    tInfo->contine = false;
    tInfo->foundPW = true;
  }

  tInfo->condAnyNextHashAvailable->get()->notify_all();
  for (auto &t : threads) {
    t.join();
    // NOTE: Was having issues with threads getting stuck
    tInfo->condAnyNextHashAvailable->get()->notify_all();
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      endTime - startTime);
  std::cout << std::format("Cracked {} passwords in {}\nFailed to crack {}\n",
                           numCrackedPW, duration, numFailedCrackedPW);

  return;
}

void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               const int start, const int end,
               std::shared_ptr<threadInfo> tInfo) {
  EVP_Hash hasher(hashMethod);
  std::shared_lock<std::shared_mutex> lock(*tInfo->nextHashSharedMutex->get());
  while (true) {
    tInfo->condAnyNextHashAvailable->get()->wait(lock);
    if (tInfo->contine) {
      tInfo->threadWorkingMutex->get()->lock_shared();
      crackPW(hasher, commonPW, start, end, tInfo);
      // NOTE: moved the notify below to make sure all threads have started
      // working when it triggers
      tInfo->condThreadStartedWorking->get()->notify_one();
      tInfo->threadWorkingMutex->get()->unlock_shared();
    } else {
      return;
    }
  }
  return;
}

void crackPW(EVP_Hash &hasher, const std::vector<std::string> &commonPW,
             const int start, const int end,
             std::shared_ptr<threadInfo> tInfo) {
  for (int i = start; i < end; i++) {
    if (*tInfo->hash == hasher.hashString(commonPW.at(i) + *tInfo->salt)) {
      std::scoped_lock<std::mutex> lock(*tInfo->tInfoMutex->get());
      tInfo->foundPW = true;
      tInfo->result->get()->password = new std::string(commonPW.at(i));
      tInfo->result->get()->loops = i - start;
      return;
    } else if (tInfo->foundPW == true) {
      return;
    }
  }
}
