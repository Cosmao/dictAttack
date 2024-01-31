#include "headers/io_file.h"

io_file::io_file(const std::string &filePath) {
  this->fileHandle.open(filePath, std::ios::in | std::ios::out | std::ios::app |
                                      std::ios::ate);
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
  }
  return true;
}

void io_file::reset(void) {
  this->fileHandle.seekg(0, this->fileHandle.beg);
  if (this->fileHandle.eof() && !this->fileHandle.fail() &&
      !this->fileHandle.bad()) {
    this->fileHandle.clear();
  }
}
