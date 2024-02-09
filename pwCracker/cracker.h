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
  std::unique_ptr<std::shared_mutex> *sharedMutex;
  std::unique_ptr<std::condition_variable> *condHashFound;
  std::unique_ptr<std::condition_variable_any> *condAnyNextHash;
  std::string *hash;
  std::string *salt;
  volatile bool contine;
  volatile bool foundPW;
} threadInfo;

void crackThreadHandler(io_file &hashedPW, io_file &commonPW,
                        const int threads, const hashMethods hashMethod);
void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               int start, int end, std::shared_ptr<threadInfo> tInfo, int threadID);
void crackPW(EVP_Hash &hasher, const std::vector<std::string> &commonPW,
             int start, int end, std::shared_ptr<threadInfo> tInfo);

#endif // !cracker_h
