#ifndef io_file_h
#define io_file_h

#include "io.h"
#include <fstream>
#include <string>

class io_file : public io {
private:
  std::fstream fileHandle;

public:
  explicit io_file(const std::string &filePath);
  std::string readLine(void) override;
  bool writeLine(const std::string &strToWrite) override;
  bool hasLine(void) override;
  void resetStreamPos(void) override;
  bool is_good(void);
  ~io_file(void);
};

#endif // !io_file_h
