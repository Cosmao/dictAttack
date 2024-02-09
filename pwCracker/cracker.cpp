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

  std::unique_ptr<std::mutex> tMutex(new std::mutex());
  std::unique_ptr<std::mutex> condMutex(new std::mutex());
  std::unique_ptr<std::condition_variable> cvHashFound(
      new std::condition_variable());
  std::unique_ptr<std::shared_mutex> sMutex(new std::shared_mutex());
  std::unique_ptr<std::condition_variable_any> cvAny(
      new std::condition_variable_any());

  tInfo->condHashFound = &cvHashFound;
  tInfo->condAnyNextHash = &cvAny;
  tInfo->tInfoMutex = &tMutex;
  tInfo->condMutex = &condMutex;
  tInfo->sharedMutex = &sMutex;
  tInfo->contine = true;
  tInfo->foundPW = false;
  tInfo->result = &result;

  std::vector<std::thread> threads(numThreads);
  int id = 0;
  int start = 0;
  int amountPerThread = pw.size() / numThreads;
  int end = start + amountPerThread;
  std::cout << std::format("Num PW: {}\n", pw.size());
  for (auto &t : threads) {
    t = std::thread(crackFunc, hashMethod, std::ref(pw), start, end, tInfo, id);
    std::cout << std::format("Thread {}: {}-{}\n", id, start, end);
    id++;
    start += amountPerThread + 1;
    end += amountPerThread + 1;
  }
  int limiter = 0;
  // NOTE: Cba to make a proper signal that all threads are ready to start
  // recieving data so just a small sleep
  sleep(1);
  auto startTime = std::chrono::high_resolution_clock::now();
  while (hashedPW.hasLine()) {
    std::unique_lock<std::mutex> cLock(*tInfo->condMutex->get());
    std::string line = hashedPW.readLine();
    std::string sHash = line.substr(0, line.find_first_of(','));
    std::string sSalt = line.substr(line.find_first_of(',') + 1);
    {
      std::cout << "locking tInfo\n";
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex->get());
      tInfo->foundPW = false;
      tInfo->hash = &sHash;
      tInfo->salt = &sSalt;
      std::cout << "unlocking tInfo\n";
    }
    std::cout << "Main thread release\n";
    tInfo->condAnyNextHash->get()->notify_all();
    std::cout << "Main thread awaiting found release\n";
    tInfo->condHashFound->get()->wait(cLock);
    std::cout << "Main thread got cond\nAwaiting exclusive lock\n";
    tInfo->sharedMutex->get()->lock();
    std::cout << "Got exclusive lock\n";
    std::cout << std::format("Hash: {}\nPW: {}\nLoops: {}\nFrom main\n", sHash,
                             *tInfo->result->get()->password,
                             tInfo->result->get()->loops);
    free(tInfo->result->get()->password);
    tInfo->sharedMutex->get()->unlock();
    std::cout << "Unlocked shared mutex\n";
    limiter++;
    if (limiter == limit) {
      std::cout << "Testing limit hit terminating threads\n";
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex->get());
      tInfo->contine = false;
      tInfo->foundPW = true;
      break;
      ;
    }
  }

  {
    std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex->get());
    tInfo->contine = false;
    tInfo->foundPW = true;
  }

  std::cout << "Notified all threads and waiting for them to terminate\n";
  tInfo->condAnyNextHash->get()->notify_all();
  for (auto &t : threads) {
    t.join();
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      endTime - startTime);
  std::cout << std::format("Cracked {} passwords in {}\n", limiter, duration);

  return;
}

void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               int start, int end, std::shared_ptr<threadInfo> tInfo,
               int threadID) {
  EVP_Hash hasher(hashMethod);
  std::shared_lock<std::shared_mutex> lock(*tInfo->sharedMutex->get());
  while (true) {
    std::cout << std::format("Thread {} waiting signal\n", threadID);
    tInfo->condAnyNextHash->get()->wait(lock);
    if (tInfo->contine) {
      std::cout << std::format("Thread {} starting cracking hash {}\n",
                               threadID, *tInfo->hash);
      crackPW(hasher, commonPW, start, end, tInfo);
    } else {
      return;
    }
  }
  return;
}

void crackPW(EVP_Hash &hasher, const std::vector<std::string> &commonPW,
             int start, int end, std::shared_ptr<threadInfo> tInfo) {
  for (int i = start; i < end; i++) {
    if (*tInfo->hash == hasher.hashString(commonPW.at(i) + *tInfo->salt)) {
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex->get());
      tInfo->foundPW = true;
      tInfo->result->get()->password = new std::string(commonPW.at(i));
      tInfo->result->get()->loops = i - start;
      tInfo->condHashFound->get()->notify_one();
      return;
    } else if (tInfo->foundPW == true) {
      return;
    }
  }
}
