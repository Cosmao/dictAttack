#ifndef cracker_h
#define cracker_h

#include "srcShared/io_file.h"
#include "srcShared/openSSL_EVP.h"
#include <mutex>
#include <vector>

void crackFunc(hashMethods hashMethod, const std::string &hashedPW,
               const std::vector<std::string> &commonPW, int start, int end,
               volatile bool &foundPW, std::mutex &boolMutex);
void crackThreadHandler(io_file &hashedPW, io_file &commonPW,
                        const int threads);

#endif // !cracker_h
