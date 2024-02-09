#include "pwCracker/argcHandler.h"

#define hashAFile 3
#define crackAFile 5

int main(int argc, char *argv[]) {
  switch (argc) {
  case hashAFile:
    hashFile(argv);
    break;
  case crackAFile:
    crackFile(argv);
    break;
  default:
    invalidArgs();
    break;
  }
  return 0;
}
