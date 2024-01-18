#include "headers/io.h"
#include "headers/md5.h"
#include "headers/userMenu.h"

#define filePath "filePath"

int main(void){
  io_hander io = *new io_hander(filePath);
  userMenu menu = *new userMenu(io);
  // menu.menu();
md5 hash = *new md5();
  hash.hashString("wow");
  return 0;
}
