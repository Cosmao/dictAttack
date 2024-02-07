#include "pwCracker/cracker.h"
#include "srcShared/openSSL_EVP.h"
#include <condition_variable>
#include <format>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unistd.h>

#define limit 4

// TODO: Dont delete the threads, maybe just give new info and signal that its
// ready
//  Dont need to recreate the EVP_HASH as well then
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

  crackResult *result = (crackResult *)calloc(1, sizeof(crackResult));
  threadInfo *tInfo = (threadInfo *)calloc(1, sizeof(threadInfo));
  std::mutex *tMutex = (std::mutex *)malloc(sizeof(std::mutex));
  new (tMutex) std::mutex();
  std::mutex *condMutex = (std::mutex *)malloc(sizeof(std::mutex));
  new (condMutex) std::mutex();
  std::condition_variable *cvNextHash =
      (std::condition_variable *)malloc(sizeof(std::condition_variable));
  new (cvNextHash) std::condition_variable();
  std::condition_variable *cvHashFound =
      (std::condition_variable *)malloc(sizeof(std::condition_variable));
  new (cvHashFound) std::condition_variable();
  std::shared_mutex *sMutex =
      (std::shared_mutex *)malloc(sizeof(std::shared_mutex));
  new (sMutex) std::shared_mutex();
  std::condition_variable_any *cvAny = (std::condition_variable_any *)malloc(
      sizeof(std::condition_variable_any));
  new (cvAny) std::condition_variable_any();
  tInfo->condNextHash = cvNextHash;
  tInfo->condHashFound = cvHashFound;
  tInfo->condAnyNextHash = cvAny; //
  tInfo->tInfoMutex = tMutex; //
  tInfo->condMutex = condMutex; //
  tInfo->sharedMutex = sMutex; //
  tInfo->contine = true;
  tInfo->foundPW = false;
  tInfo->result = result;

  std::vector<std::thread> threads(numThreads);
  int id = 0;
  int start = 0;
  int end = start + amountPerThread;
  std::cout << std::format("Num PW: {}\n", pw.size());
  for (auto &t : threads) {
    t = std::thread(crackFunc, MD5_Hash, std::ref(pw), start, end, tInfo, id);
    std::cout << std::format("Thread {}: {}-{}\n", id, start, end);
    id++;
    start += amountPerThread + 1;
    end += amountPerThread + 1;
  }
  int limiter = 0;
  while (hashedPW.hasLine()) {
    std::unique_lock<std::mutex> cLock(*tInfo->condMutex);
    std::string sHash = hashedPW.readLine();
    {
      std::cout << "locking tInfo\n";
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex);
      tInfo->foundPW = false;
      tInfo->hash = &sHash;
      std::cout << "unlocking tInfo\n";
    }
    std::cout << "Main thread release\n";
    tInfo->condAnyNextHash->notify_all();
    std::cout << "Main thread awaiting found release\n";
    tInfo->condHashFound->wait(cLock);
    std::cout << "Main thread got cond\nAwaiting exclusive lock\n";
    tInfo->sharedMutex->lock();
    std::cout << "Got exclusive lock\n";
    std::cout << std::format("Hash: {}\nPW: {}\nFrom main\n", sHash,
                             *tInfo->result->password);
    tInfo->sharedMutex->unlock();
    std::cout << "Unlocked shared mutex\n";
    limiter++;
    if (limiter == limit) {
      std::cout << "Testing limit hit terminating threads\n";
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex);
      tInfo->contine = false;
      tInfo->foundPW = true;
      break;
      ;
    }
  }

  std::cout << "Waiting for threads to terminate\n";
  tInfo->condAnyNextHash->notify_all();
  for (auto &t : threads) {
    t.join();
  }

  free(result);
  return;
}

void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               int start, int end, threadInfo *tInfo, int threadID) {
  EVP_Hash hasher(hashMethod);
  std::shared_lock<std::shared_mutex> lock(*tInfo->sharedMutex);
  while (true) {
    std::cout << std::format("Thread {} waiting signal\n", threadID);
    tInfo->condAnyNextHash->wait(lock);
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
             int start, int end, threadInfo *tInfo) {
  for (int i = start; i < end; i++) {
    if (*tInfo->hash == hasher.hashString(commonPW.at(i))) {
      std::lock_guard<std::mutex> lock(*tInfo->tInfoMutex);
      tInfo->foundPW = true;
      tInfo->result->password = new std::string(commonPW.at(i));
      tInfo->result->loops = i - start;
      tInfo->condHashFound->notify_one();
      return;
    } else if (tInfo->foundPW == true) {
      return;
    }
  }
}
