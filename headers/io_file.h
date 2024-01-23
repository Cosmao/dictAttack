#ifndef io_file_h
#define io_file_h

#include <string>

class io_hander {
private:
  const std::string filePath;

public:
  explicit io_hander(const std::string &filePath) : filePath(filePath){};
  ~io_hander(void);
  bool closeFile(void);
  // template <typename T> T readFile(void) { return T; }
};

#endif // !io_file_h