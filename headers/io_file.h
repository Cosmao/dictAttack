#ifndef io_file_h
#define io_file_h

#include "headers/io.h"
#include <string>

class io_file : public io {
private:
  const std::string filePath;

public:
  explicit io_file(const std::string &filePath) : filePath(filePath){};
  std::string readLine(void) override;
  bool hasLine(void) override;
  ~io_file(void);
};

#endif // !io_file_h
