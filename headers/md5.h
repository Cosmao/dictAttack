#ifndef md5_h
#define md5_h

#include "headers/hashing.h"

class md5 : public hashing{
private:

public:
  std::string hashString(std::string);
};


#endif // !md5_h
