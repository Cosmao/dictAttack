#include "headers/io_file.h"
#include <format>
#include <ios>
#include <string>

io_file::io_file(const std::string &filePath) {
  this->fileHandle.open(filePath, std::ios::in | std::ios::out | std::ios::app | std::ios::ate);
}

io_file::~io_file(void) {
  this->fileHandle << "closing";
  this->fileHandle.close();
  std::cout << "Closed file"
            << "\n";
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
  // TODO:Check if stream is good;
  int c = this->fileHandle.peek();
  return c != EOF;
}

bool io_file::writeLine(const std::string &strToWrite) {
  if (this->fileHandle.is_open()) {
    this->fileHandle.seekp(std::ios_base::end);
    std::cout << "Writing: " + strToWrite << " " << strToWrite.size() << "\n";
    this->fileHandle << std::format("{}", strToWrite);
    this->fileHandle.flush();
  }
  return true;
}

void io_file::reset(void) { this->fileHandle.seekg(0, this->fileHandle.beg); }
