#include "headers/io_file.h"
#include <string>

io_file::io_file(const std::string &filePath) {
  this->infile = std::ifstream(filePath);
  if (!this->infile.good()) {
    // TODO: Probably throw a error here instead
    std::cout << "filepath bad\n";
  }
  std::cout << "Filepath good " << filePath << "\n";
}

io_file::~io_file(void) {
  this->infile.close();
  std::cout << "Closed file" << "\n";
  return;
}

std::string io_file::readLine(void) {
  std::string line;
  // TODO: Check if stream good
  std::getline(this->infile, line);
  return line;
}

bool io_file::hasLine(void) {
  // TODO:Check if stream is good;
  int c = this->infile.peek();
  return c != EOF;
}

void io_file::reset(void){
  this->infile.seekg(0, this->infile.beg);
}
