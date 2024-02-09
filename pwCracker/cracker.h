#ifndef cracker_h
#define cracker_h

#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

typedef struct {
  std::string *password;
  int loops;
} crackResult;

typedef struct {
  std::unique_ptr<crackResult> *result;
  std::unique_ptr<std::mutex> *tInfoMutex;
  std::unique_ptr<std::mutex> *condMutex;
  std::unique_ptr<std::shared_mutex> *nextHashSharedMutex;
  std::unique_ptr<std::shared_mutex> *threadWorkingMutex;
  std::unique_ptr<std::condition_variable> *condThreadStartedWorking;
  std::unique_ptr<std::condition_variable_any> *condAnyNextHashAvailable;
  std::string *hash;
  std::string *salt;
  volatile bool contine;
  volatile bool foundPW;
} threadInfo;

void crackThreadHandler(io_file &hashedPW, io_file &commonPW, const int threads,
                        const hashMethods hashMethod);
void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               const int start, const int end,
               std::shared_ptr<threadInfo> tInfo);
void crackPW(EVP_Hash &hasher, const std::vector<std::string> &commonPW,
             const int start, const int end, std::shared_ptr<threadInfo> tInfo);

#endif // !cracker_h
