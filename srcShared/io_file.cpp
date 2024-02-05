#include "io_file.h"
#include <format>

io_file::io_file(const std::string &filePath) {
  this->fileHandle.open(filePath, std::ios::in | std::ios::out | std::ios::app |
                                      std::ios::ate);
}

bool io_file::is_good(void){
  return this->fileHandle.good();
}

io_file::~io_file(void) {
  this->fileHandle.close();
  return;
}

std::string io_file::readLine(void) {
  std::string line;
  if (this->fileHandle.good()) {
    std::getline(this->fileHandle, line);
  }
  return line;
}

bool io_file::hasLine(void) {
  int c = EOF;
  if (this->fileHandle.good()) {
    c = this->fileHandle.peek();
  }
  return c != EOF;
}

bool io_file::writeLine(const std::string &strToWrite) {
  if (this->fileHandle.good()) {
    this->fileHandle.seekp(std::ios_base::end);
    this->fileHandle << std::format("{}\n", strToWrite);
    this->fileHandle.flush();
    return true;
  }
  return false;
}

void io_file::resetStreamPos(void) {
  this->fileHandle.seekg(0, this->fileHandle.beg);
  if (this->fileHandle.eof() && !this->fileHandle.fail() &&
      !this->fileHandle.bad()) {
    this->fileHandle.clear();
  }
}
