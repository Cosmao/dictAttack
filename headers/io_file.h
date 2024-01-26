#ifndef io_file_h
#define io_file_h

#include "headers/io.h"
#include <fstream>
#include <string>
#include <iostream>

class io_file : public io {
private:
  std::fstream fileHandle;

public:
  explicit io_file(const std::string &filePath);// : filePath(filePath){};
  std::string readLine(void) override;
  bool writeLine(const std::string &strToWrite) override;
  bool hasLine(void) override;
  void reset(void) override;
  ~io_file(void);
};

#endif // !io_file_h
