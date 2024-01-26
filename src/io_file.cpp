#include "headers/io_file.h"
#include <string>

io_file::io_file(const std::string &filePath) {
  this->fileHandle =
      std::fstream(filePath, std::ios::in | std::ios::out | std::ios::app);
  if (!this->fileHandle.good()) {
    // TODO: Probably throw a error here instead
    std::cout << "filepath bad\n";
  }
  std::cout << "Filepath good " << filePath << "\n";
}

io_file::~io_file(void) {
  this->fileHandle.close();
  std::cout << "Closed file"
            << "\n";
  return;
}

std::string io_file::readLine(void) {
  std::string line;
  // TODO: Check if stream good
  std::getline(this->fileHandle, line);
  return line;
}

bool io_file::hasLine(void) {
  // TODO:Check if stream is good;
  int c = this->fileHandle.peek();
  return c != EOF;
}

bool io_file::writeLine(const std::string &strToWrite){
  std::cout << "Writing: " + strToWrite << "\n";
  this->fileHandle.write(strToWrite.c_str(), strToWrite.size());

  return true;
}

void io_file::reset(void) { this->fileHandle.seekg(0, this->fileHandle.beg); }
