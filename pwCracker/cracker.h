#ifndef cracker_h
#define cracker_h

#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <vector>

typedef struct {
  std::string *password;
  int loops;
} crackResult;

typedef struct {
  crackResult *result;
  std::mutex *tInfoMutex;
  std::mutex *condMutex;
  std::shared_mutex *sharedMutex;
  std::condition_variable *condNextHash;
  std::condition_variable *condHashFound;
  std::condition_variable_any *condAnyNextHash;
  std::string *hash;
  volatile bool contine;
  volatile bool foundPW;
} threadInfo;

void crackThreadHandler(io_file &hashedPW, io_file &commonPW,
                        const int threads);
void crackFunc(hashMethods hashMethod, const std::vector<std::string> &commonPW,
               int start, int end, threadInfo *tInfo, int threadID);
void crackPW(EVP_Hash &hasher, const std::vector<std::string> &commonPW,
             int start, int end, threadInfo *tInfo);

#endif // !cracker_h
