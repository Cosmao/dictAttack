#ifndef io_h
#define io_h

#include <string>

class io {
public:
  virtual std::string readLine(void) = 0;
  virtual bool writeLine(const std::string &string) = 0;
  virtual bool hasLine(void) = 0;
  virtual void resetStreamPos(void) = 0;
  virtual ~io(void);
};

#endif // !io_h
